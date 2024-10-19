#pragma once

#include "Core.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"
#include "OrhographicCamera.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

namespace Hazel
{
	class HAZEL_API Application // Signleton - single instance of this class supported
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
		bool OnWindowResize(WindowResizeEvent& event);

	private:
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* m_Instance;
	};

	Application* CreateApplication();

}
