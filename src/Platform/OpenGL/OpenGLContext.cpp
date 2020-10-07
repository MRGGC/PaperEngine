#include "p_pch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Paper
{
	OpenGLContext::OpenGLContext(GLFWwindow* win)
		: m_WindowHandle(win)
	{
		PAPER_CORE_ASSERT(m_WindowHandle, "WindowHandle not Initialized!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PAPER_CORE_ASSERT(success, "Glad not Initialized!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}