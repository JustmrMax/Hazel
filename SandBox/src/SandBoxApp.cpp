#include "SandBoxApp.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platforms/OpenGL/OpenGLShader.h" // YEAP

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
	{
		//// VertexBuffer
		//float vertices[3 * 7] = {	// (x, y, z, colol_r, color_g, color_b, color_a) per vertex
		//	-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 1.0f, 1.0f,
		//	0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f	, 1.0f,
		//	0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 1.0f, 1.0f
		//};
		//m_VertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));

		//// IndexBuffer
		//uint32_t indecies[3] = { 0, 1, 2 }; // sequintance of rendering verticies
		//m_IndexBuffer = Hazel::IndexBuffer::Create(indecies, sizeof(indecies) / sizeof(uint32_t));

		//// VertexArrayBuffer
		//m_VertexArray = Hazel::VertexArray::Create();

		//// Set layout to VA
		//Hazel::BufferLayout layout = {
		//	{ Hazel::ShaderDataType::Float3, "a_Position" },
		//	{ Hazel::ShaderDataType::Float4, "a_Color" }
		//};

		//m_VertexBuffer->SetLayout(layout);

		//m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		//m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// Square VertexBuffer
		float squareVertices[7 * 4] = { // (x, y, z, textCoord_x, textCoord_y) per vertex
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		Hazel::BufferLayout layout2 = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TextCord"}
		};

		squareVB->SetLayout(layout2);

		// IndexBuffer
		uint32_t squareIndecies[6] = { 0, 1, 2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB = Hazel::IndexBuffer::Create(squareIndecies, sizeof(squareIndecies) / sizeof(uint32_t));

		// Square VA
		m_SquareVA = Hazel::VertexArray::Create();

		m_SquareVA->AddVertexBuffer(squareVB);
		m_SquareVA->SetIndexBuffer(squareIB);

		// Example shader
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentxSrc = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}

		)";

		Hazel::Ref<Hazel::Shader> triangleShader = Hazel::Shader::Create("Triangle", vertexSrc, fragmentxSrc);
		m_ShaderLibrary.Add(triangleShader);

		// Flat color shader
		std::string flatColorVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string flatColorFragmentSrc = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0f);
			}

		)";

		Hazel::Ref<Hazel::Shader> flatColorShader = Hazel::Shader::Create("FlatColor", flatColorVertexSrc, flatColorFragmentSrc);
		m_ShaderLibrary.Add(flatColorShader);

		// TextureShader
		Hazel::Ref<Hazel::Shader> textureShader = m_ShaderLibrary.Load("assets/shaders/TextureShader.glsl"); // RefCount += 1 (Adding a texture increase a refcount)
		// When we load the texture we automatically add it to shaderLibrary

		// Loading textures
		m_Texture = Hazel::Texture2D::Create("assets/textures/CheckBoard.png");
		m_LogoTexture = Hazel::Texture2D::Create("assets/textures/TheChernoLogo.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind(); // YEAP dynamic_cast is not the best solution
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0); // YEAP
	}

	virtual void OnUpdate(Hazel::Timestep ts) override
	{
		// HZ_TRACE("Frame Time is {0}s | {1}ms", ts.GetTimeSeconds(), ts.GetTimeMilliseconds());

		// Update
		m_CameraController.OnUpdate(ts); // updating movement of camera and its rotation position

		// Render
		Hazel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));

		Hazel::Ref<Hazel::Shader> flatColorShader = m_ShaderLibrary.Get("FlatColor");
		flatColorShader->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		// Render a grid of squares
		// It's working so low because of rendering each square of the grid per a call submit
		// Is's need to be linked all squares in one big vertexbuffer
		// This mechanism must be implemented
		for (int j = 0; j < 50; ++j)
		{
			for (int i = 0; i < 10; ++i)
			{
				 glm::vec3 pos = glm::vec3((float)i * 0.11f, (float)j * 0.11f, 0.0f);
				 glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);
				 
				 Hazel::Renderer::Submit(flatColorShader, m_SquareVA, transform * scale);
			}
		}

		// Getting necessary shader from shaderLibrary - It's actually step backwards but it's such a comfortable way for storing shaders in one place
		Hazel::Ref<Hazel::Shader> textureShader = m_ShaderLibrary.Get("TextureShader"); // RefCount += 1

		m_Texture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.2f)));
		m_LogoTexture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.2f)));

		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	virtual void OnEvent(Hazel::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::VertexBuffer> m_VertexBuffer;
	Hazel::Ref<Hazel::IndexBuffer> m_IndexBuffer;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	// Hazel::Ref<Hazel::Shader> m_Shader;
	// Hazel::Ref<Hazel::Shader> m_FlatColorShader, m_TextureShader;
	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_LogoTexture;

	glm::vec3 m_SquareColor = glm::vec3(0.3f, 1.0f, 0.5f);

	// Hazel::OrhographicCamera m_Camera;
	Hazel::OrhographicCameraController m_CameraController;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.41f;

	float m_CameraRotationSpeed = 180.3f;
	float m_CameraRotation = 0.0f;
};

SandBox::SandBox()
{
	PushLayer(new ExampleLayer());
}

SandBox::~SandBox()
{

}

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBox();
}