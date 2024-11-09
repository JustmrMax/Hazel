#include "hzpch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platforms/OpenGL/OpenGLShader.h" // YEAP

namespace Hazel
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Texture2D> WhiteTexture;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		// VertexBuffer
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TextureCoord" }
		};
		squareVB->SetLayout(layout);

		// IndexBuffer
		uint32_t squareIndecies[6] = { 0, 1, 2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB = Hazel::IndexBuffer::Create(squareIndecies, sizeof(squareIndecies) / sizeof(uint32_t));

		// QuadVertexArray
		s_Data->QuadVertexArray = Hazel::VertexArray::Create();

		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		// TextureShader
		s_Data->TextureShader = Hazel::Shader::Create("assets/shaders/TextureShader.glsl");

		// WhiteTexture
		uint32_t color = 0xFFFFFFFF;
		s_Data->WhiteTexture = Hazel::Texture2D::Create(1, 1);
		s_Data->WhiteTexture->SetData(&color, sizeof(color));
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrhographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data->TextureShader->SetUniformInt("u_Texture", 0);
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(glm::vec2& position, glm::vec2& size, glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(glm::vec3& position, glm::vec2& size, glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* Rotation */ glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetUniformFloat4("u_Color", color);
		// Binding white shader for default
		s_Data->WhiteTexture->Bind();

		s_Data->TextureShader->SetUniformMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(glm::vec2& position, glm::vec2& size, Ref<Texture2D>& texture)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, texture);
	}

	void Renderer2D::DrawQuad(glm::vec3& position, glm::vec2& size, Ref<Texture2D>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* Rotation */ glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data->TextureShader->Bind();
		texture->Bind();
		// Set white color for default
		s_Data->TextureShader->SetUniformFloat4("u_Color", glm::vec4(1.0f, 0.8f, 1.0f, 0.5f));

		s_Data->TextureShader->SetUniformMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}