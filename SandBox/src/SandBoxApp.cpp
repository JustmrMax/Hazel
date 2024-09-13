#include "SandBoxApp.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		// VertexBuffer
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.05, 1.0f,
			0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 1.0f, 1.0f
		};
		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		// IndexBuffer
		uint32_t indecies[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(Hazel::IndexBuffer::Create(indecies, sizeof(indecies) / sizeof(uint32_t)));

		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};

		m_VertexBuffer->SetLayout(layout);

		// VertexArrayBuffer
		m_VertexArray.reset(Hazel::VertexArray::Create());
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// Square VB
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Hazel::BufferLayout layout2 = {
			{ Hazel::ShaderDataType::Float3, "a_Position"}
		};

		squareVB->SetLayout(layout2);

		// IndexBuffer
		uint32_t squareIndecies[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndecies, sizeof(squareIndecies) / sizeof(uint32_t)));

		// Square VA
		m_SquareVA.reset(Hazel::VertexArray::Create());
		m_SquareVA->AddVertexBuffer(squareVB);
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		std::string BlueVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}

		)";

		std::string BlueFragmentSrc = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.3f, 0.5f, 0.9f, 1.0f);
			}

		)";

		m_BlueShader.reset(new Hazel::Shader(BlueVertexSrc, BlueFragmentSrc));
		m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentxSrc));
	}

	virtual void OnUpdate(Hazel::Timestep ts) override
	{
		HZ_TRACE("Frame Time is {0}s | {1}ms", ts.GetTimeSeconds(), ts.GetTimeMilliseconds());

		// Handling Camera movement
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		// Handling Rotation Camera
		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_Camera);

		Hazel::Renderer::Submit(m_BlueShader, m_SquareVA);
		Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
	}

	virtual void OnEvent(Hazel::Event& event) override
	{
	}

private:
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Hazel::Shader> m_Shader;

	std::shared_ptr<Hazel::VertexArray> m_SquareVA;
	std::unique_ptr<Hazel::Shader> m_BlueShader;

	Hazel::OrhographicCamera m_Camera;

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