#include "gdpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace GodDecay 
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		GD_ENGINE_ASSERT(windowHandle, "this Window Handle is NULL");
	}

	void OpenGLContext::ContextInit()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GD_ENGINE_ASSERT(status, "this glad load failed! ");

		//‰÷»æ–≈œ¢œ‘ æ
		GD_ENGINE_INFO("OpenGL Info:");
		GD_ENGINE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		GD_ENGINE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		GD_ENGINE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
