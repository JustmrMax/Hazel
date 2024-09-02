#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel
{
	OpenGLContext::OpenGLContext(GLFWwindow *window)
		: m_WindowHandle(window)
	{
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_ASSERT(status, "Glad don't initialized!");

		HZ_CORE_INFO("OpenGL Info:");
		HZ_CORE_INFO("  Vendor: {0}:", (const char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO("  Renderer: {0}:", (const char*)glGetString(GL_RENDERER ));
		HZ_CORE_INFO("  Version: {0}:", (const char*)glGetString(GL_VERSION) );
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}