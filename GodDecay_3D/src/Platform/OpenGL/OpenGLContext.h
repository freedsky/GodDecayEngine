#pragma once
#include "GodDecay/Renderer/RendererContext.h"

/// <summary>
/// OpenGL的上下文实现被GLFW封装的很好[其本身也不需要太多的设置]
/// 此类是对OpenGL上下文的进一步封装，方便上层调用
/// </summary>
namespace GodDecay 
{
	class OpenGLContext : public RendererContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void ContextInit() override;

		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}



