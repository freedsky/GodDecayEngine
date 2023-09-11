#pragma once

#include "Core.h"

namespace GodDecay 
{
	class GODDECAY_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
	private:
	};

	//包含此头文件以及继承Application类的子类需要在其类外文件中实现定义
	Application* CreateApplication();
}



