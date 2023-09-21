#pragma once
#include "GodDecay/Renderer/RendererContext.h"

/// <summary>
/// OpenGL��������ʵ�ֱ�GLFW��װ�ĺܺ�[�䱾��Ҳ����Ҫ̫�������]
/// �����Ƕ�OpenGL�����ĵĽ�һ����װ�������ϲ����
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



