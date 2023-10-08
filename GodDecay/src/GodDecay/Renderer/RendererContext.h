#pragma once

struct GLFWwindow;

/// <summary>
/// 渲染上下文封装，不同平台图形API它实现的细节各不相同
/// 因此需要把与渲染相关API创建和接口进行进一步封装，实现跨平台调用
/// </summary>
namespace GodDecay 
{
	class RendererContext 
	{
	public:
		virtual void ContextInit() = 0;

		virtual void SwapBuffers() = 0;
	};
}
