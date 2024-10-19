#include "SandBox2D.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platforms/OpenGL/OpenGLShader.h" // should be agnostic API


SandBox2D::SandBox2D()
	: Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void SandBox2D::OnAttach()
{
	// VertexBuffer
	float squareVertices[3 * 4] = { 
		-0.5f, -0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};

	Hazel::Ref<Hazel::VertexBuffer> squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

	Hazel::BufferLayout layout = {
		{ Hazel::ShaderDataType::Float3, "a_Position" }
	};
	squareVB->SetLayout(layout);

	// IndexBuffer
	uint32_t squareIndecies[6] = { 0, 1, 2, 2, 3, 0 };
	Hazel::Ref<Hazel::IndexBuffer> squareIB = Hazel::IndexBuffer::Create(squareIndecies, sizeof(squareIndecies) / sizeof(uint32_t));

	// Square VA
	m_SquareVA = Hazel::VertexArray::Create();

	m_SquareVA->AddVertexBuffer(squareVB);
	m_SquareVA->SetIndexBuffer(squareIB);

	// Square Shader
	m_FlatColorShader = Hazel::Shader::Create("assets/shaders/FlatColor.glsl");
}

void SandBox2D::OnDettach()
{

}

void SandBox2D::OnUpdate(Hazel::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts); // updating movement of camera and its rotation position

	// Render
	Hazel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	m_FlatColorShader->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
	Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.2f)));

	Hazel::Renderer::EndScene();
}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void SandBox2D::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}
