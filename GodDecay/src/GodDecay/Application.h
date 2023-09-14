#pragma once
/// <summary>
/// 应用程序类
/// </summary>
/// <returns>
/// 它可以让应用程序在启动和关闭时创建初始化和释放各种数据
/// </returns>
#include "Core.h"
#include "GodDecay/Events/Event.h"
#include "Platform/Windows/WindowsWindow.h"

namespace GodDecay 
{
	class GODDECAY_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
	private:
		Ref<Window> window = nullptr;
		bool m_Running = true;
	};

	//包含此头文件以及继承Application类的子类需要在其类外文件中实现定义
	Application* CreateApplication();
}



