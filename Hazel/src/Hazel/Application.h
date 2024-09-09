#pragma once

#include "Core.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"

#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/OrhographicCamera.h"

namespace Hazel
{
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& GetApp() { return *m_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& event);
		// bool OnKeyPressed(KeyPressedEvent& event);

		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running = true;

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		std::shared_ptr<VertexArray> m_SquareVA;
		std::unique_ptr<Shader> m_BlueShader;

		OrhographicCamera m_Camera;

		static Application* m_Instance;
	};

	Application* CreateApplication();

}
