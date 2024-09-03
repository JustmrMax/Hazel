#include "hzpch.h"
#include "Application.h"

#include "Log.h"
#include "Input.h"

#include <Glad/glad.h>

namespace Hazel
{
#undef BIND_EVENT_FN(x)
#define BIND_EVENT_FN(x)	std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::m_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return GL_NONE;
	}

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(BIND_EVENT_FN(OnEvent));

		if (m_Instance == nullptr)
			m_Instance = this;

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// VertexArrayBuffer
		glGenVertexArrays(1, &m_VertexArray); 
		glBindVertexArray(m_VertexArray); 

		// VertexBuffer
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.05, 1.0f,
			0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 1.0f, 1.0f
		};
		m_VertexBuffer.reset(VertexBuffer::CreateBuffer(vertices, sizeof(vertices)));

		// IndexBuffer
		uint32_t indecies[3] = {0, 1, 2};
		m_IndexBuffer.reset(IndexBuffer::CreateBuffer(indecies, sizeof(indecies) / sizeof(uint32_t)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		uint32_t index = 0;
		m_VertexBuffer->SetLayout(layout);

		for (auto& element : m_VertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(element.Type),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized,
				layout.GetStride(),
				(const void *)element.Offset);

			++index;
		}

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
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

		m_Shader.reset(new Shader(vertexSrc, fragmentxSrc));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);

			if (event.IsHandled() )
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			m_Shader->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;

		return true;
	}
}