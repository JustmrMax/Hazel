#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Layer.h"
#include "LayerStack.h"

#include "Window.h"

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
	private:
		bool OnWindowClose(WindowCloseEvent& event);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();

}
