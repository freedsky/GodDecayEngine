#include "gdpch.h"
#include "Application.h"
#include "Logger.h"

#include <GLFW/glfw3.h>

namespace GodDecay 
{

#define BIND_EVENTS(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		window = Window::Create();

		//把Windowswindow的函数指针指向Application的事件进行绑定
		window->SetEventCallback(BIND_EVENTS(OnEvents));
	}

	//===============================================
	void Application::PushLayer(Layer* layer) 
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer) 
	{
		m_LayerStack.PushOverlay(layer);
	}
	//===============================================

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpDate();

			//渲染窗口
			window->OnUpDate();
		}
	}

	void Application::OnEvents(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENTS(OnWindowClose));

		GD_ENGINE_INFO("{0}", e);

		for(std::vector<Layer*>::iterator it = m_LayerStack.end();it != m_LayerStack.begin();)
		{
			(*--it)->OnEvents(e);
			if (e.GetHandle())
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}
}
