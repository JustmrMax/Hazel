#include "hzpch.h"
#include "Application.h"

#include "Log.h"
#include "Input.h"
#include "KeyCodes.h" // YEAP

#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{
#undef BIND_EVENT_FN(x)
#define BIND_EVENT_FN(x)	std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::m_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(BIND_EVENT_FN(OnEvent));

		if (m_Instance == nullptr)
			m_Instance = this;

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// VertexBuffer
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.05, 1.0f,
			0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 1.0f, 1.0f
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		// IndexBuffer
		uint32_t indecies[3] = {0, 1, 2};
		m_IndexBuffer.reset(IndexBuffer::Create(indecies, sizeof(indecies) / sizeof(uint32_t)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		m_VertexBuffer->SetLayout(layout);

		// VertexArrayBuffer
		m_VertexArray.reset(VertexArray::Create());
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// Square VB
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f, 
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		BufferLayout layout2 = {
			{ ShaderDataType::Float3, "a_Position"}
		};

		squareVB->SetLayout(layout2);

		// IndexBuffer
		uint32_t squareIndecies[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndecies, sizeof(squareIndecies) / sizeof(uint32_t)) );

		// Square VA
		m_SquareVA.reset(VertexArray::Create());
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

		m_BlueShader.reset(new Shader(BlueVertexSrc, BlueFragmentSrc));
		m_Shader.reset(new Shader(vertexSrc, fragmentxSrc));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		// dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed)); // YEAP

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
			RenderCommand::SetClearColor( glm::vec4(0.1f, 0.1f, 0.1f, 1.0f) );
			RenderCommand::Clear();

			//m_Camera.SetPosition(glm::vec3(0.5f, 0.5f, 0.0f) );
			m_Camera.SetRotation(90.0f);

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_BlueShader, m_SquareVA);
			Renderer::Submit(m_Shader, m_VertexArray);
			
			Renderer::EndScene();

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

	//bool Application::OnKeyPressed(KeyPressedEvent& event)
	//{
	//	const float speedMotion = 0.04f;

	//	switch (event.GetKeyCode())
	//	{
	//	case HZ_KEY_W:
	//	{
	//		glm::vec3 oldPosition = m_Camera.GetPosition();
	//		oldPosition.y += speedMotion;
	//		m_Camera.SetPosition(oldPosition);
	//		break;
	//	}
	//	case HZ_KEY_S:
	//	{
	//		glm::vec3 oldPosition = m_Camera.GetPosition();
	//		oldPosition.y -= speedMotion;
	//		m_Camera.SetPosition(oldPosition);
	//		break;
	//	}
	//	case HZ_KEY_A:
	//	{
	//		glm::vec3 oldPosition = m_Camera.GetPosition();
	//		oldPosition.x -= speedMotion;
	//		m_Camera.SetPosition(oldPosition);
	//		break;
	//	}
	//	case HZ_KEY_D:
	//	{
	//		glm::vec3 oldPosition = m_Camera.GetPosition();
	//		oldPosition.x += speedMotion;
	//		m_Camera.SetPosition(oldPosition);
	//		break;
	//	}

	//	default:
	//		HZ_WARN("Unknown KeyCode for OnKeyPressedEvent()");
	//		return true;
	//	}

	//	return true;
	//}
}