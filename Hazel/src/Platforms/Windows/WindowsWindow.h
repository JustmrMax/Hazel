#pragma once

#include "Hazel/Window.h"
#include "GLFW/glfw3.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

namespace Hazel
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline virtual unsigned int GetWidth() const override { return m_Data.Width; }
		inline virtual unsigned int GetHeight() const override { return m_Data.Height; }

		inline virtual void* GetNativeWindow() const override { return (void*)m_Window; };

		// Window Attributes
		inline virtual void SetEventCallBack(const EventCallBackFn& callback) override { m_Data.EventCallBack = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			unsigned int Width, Height;
			std::string Title;
			bool VSync;

			EventCallBackFn EventCallBack;
		};

		WindowData m_Data;
	};
}