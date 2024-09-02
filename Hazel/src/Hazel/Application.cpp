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
		glEnableVertexAttribArray(0); 

		// VertexBuffer
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
		};
		m_VertexBuffer.reset(VertexBuffer::CreateBuffer(vertices, sizeof(vertices)));

		// IndexBuffer
		uint32_t indecies[3] = {0, 1, 2};
		m_IndexBuffer.reset(IndexBuffer::CreateBuffer(indecies, sizeof(indecies) / sizeof(uint32_t)));

		glVertexAttribPointer(0, m_IndexBuffer->GetCount(), GL_FLOAT, GL_FALSE, m_IndexBuffer->GetCount() * sizeof(float), nullptr);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentxSrc = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
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