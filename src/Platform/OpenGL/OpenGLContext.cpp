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

		PAPER_CORE_INFO("OpenGL Renderer:");
		PAPER_CORE_INFO("--Vendor: {0}", glGetString(GL_VENDOR));
		PAPER_CORE_INFO("--Renderer: {0}", glGetString(GL_RENDERER));
		PAPER_CORE_INFO("--Version: {0}", glGetString(GL_VERSION));

		#ifdef PAPER_ENABLE_ASSERTS
			int version_major;
			int version_minor;
			glGetIntegerv(GL_MAJOR_VERSION, &version_major);
			glGetIntegerv(GL_MINOR_VERSION, &version_minor);

			PAPER_CORE_ASSERT(version_major > 3 || (version_major == 3 && version_minor >= 3), "PaperEngine requires >=OpenGL 3.3");
		#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}