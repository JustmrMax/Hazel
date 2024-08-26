#include "hzpch.h"
#include "WindowsWindow.h"

namespace Hazel
{
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown

			int succsess = glfwInit();	
			HZ_CORE_ASSERT(success, "Could not initialize GLFW!");

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int actions, int mods)
			{
				switch (actions)
				{
				case GLFW_PRESS:
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					KeyPressedEvent e(key, 0);
					
					data.EventCallBack(e);
					break;
				}

				
				case GLFW_RELEASE:
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					KeyReleasedEvent e(key);

					data.EventCallBack(e);
					break;
				}

				
				case GLFW_REPEAT:
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					KeyPressedEvent e(key, 1);

					data.EventCallBack(e);
					break;
				}
				}
			}
		);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowResizeEvent e(width, height);

				data.Width = width;
				data.Height = height;
				data.EventCallBack(e);
			}
		);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent e;
				data.EventCallBack(e);
			}
		);

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int actinos, int mods)
			{
				switch (actinos)
				{
				case GLFW_PRESS:
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					MouseButtonPressedEvent e(button);
					data.EventCallBack(e);
					break;
				}


				case GLFW_RELEASE:
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					MouseButtonReleasedEvent e(button);
					data.EventCallBack(e);
					break;
				}
				}
			}
		);

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent e((float)xPos, (float)yPos);
				data.EventCallBack(e);
			}
		);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent e((float)xOffset, (float)yOffset);
				data.EventCallBack(e);
			}
		);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}