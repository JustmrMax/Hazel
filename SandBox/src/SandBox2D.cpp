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
	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/checkerboard_1.png");
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

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Hazel::Renderer2D::DrawQuad(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.3f, 0.5f, 0.9f, 1.0f));
	Hazel::Renderer2D::DrawQuad(glm::vec2(2.0f, 7.0f), glm::vec2(0.3f, 1.1f), glm::vec4(0.9f, 0.5f, 0.9f, 1.0f));
	Hazel::Renderer2D::DrawQuad(glm::vec2(-1.0f, 0.5f), glm::vec2(0.8f, 0.8f), glm::vec4(0.1f, 0.1f, 0.9f, 1.0f));

	Hazel::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(100.0f, 100.0f), m_CheckerboardTexture);

	Hazel::Renderer2D::EndScene();
}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void SandBox2D::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}
