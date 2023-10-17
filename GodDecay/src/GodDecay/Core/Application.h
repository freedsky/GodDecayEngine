#pragma once
/// <summary>
/// 应用程序类
/// </summary>
/// <returns>
/// 它可以让应用程序在启动和关闭时创建初始化和释放各种数据
/// </returns>
#include "Base.h"
#include "Window.h"
#include "Platform/PlatformTools/OpenGLTimeTool.h"

#include "GodDecay/Events/Event.h"
#include "GodDecay/Events/ApplicationEvent.h"

#include "LayerStack.h"
#include "GodDecay/ImGui/ImGuiLayer.h"

namespace GodDecay 
{
	class Application
	{
	public:
		Application(const std::string& name = "GodDecay Engine App");
		virtual ~Application();

		void OnEvents(Event& e);

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *m_Instance; }
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		Scope<Window> m_Window = nullptr;
		static Application* m_Instance;
		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		Ref<Time> m_Time;
	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);
	};

	//包含此头文件以及继承Application类的子类需要在其类外文件中实现定义
	Application* CreateApplication();
}



