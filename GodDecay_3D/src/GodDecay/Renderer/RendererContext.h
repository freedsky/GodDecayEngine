#pragma once

struct GLFWwindow;

/// <summary>
/// ��Ⱦ�����ķ�װ����ͬƽ̨ͼ��API��ʵ�ֵ�ϸ�ڸ�����ͬ
/// �����Ҫ������Ⱦ���API�����ͽӿڽ��н�һ����װ��ʵ�ֿ�ƽ̨����
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
