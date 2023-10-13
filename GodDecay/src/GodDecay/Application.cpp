#include "gdpch.h"
#include "Application.h"
#include "Logger.h"

#include "Renderer/Renderer.h"

#include <glad/glad.h>

namespace GodDecay 
{
	
#define BIND_EVENTS(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::m_Instance = nullptr;

	Application::Application()
	{
		GD_ENGINE_ASSERT(!m_Instance, "this Application already exitity");
		m_Instance = this;

		m_Window = Window::Create();
		//把Windowswindow的函数指针指向Application的事件进行绑定
		m_Window->SetEventCallback(BIND_EVENTS(OnEvents));

		m_Time = CreateRef<OpenGLTimeTool>(TimeType::DeltaTime);

		Renderer::Init();

		//因为在每次push时都会调用OnAttach，除非有什么功能层在application之上，要不然这里什么都不会执行
		//for (Layer* layer : m_LayerStack)
		//	layer->OnAttach();
		

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		//在这里释放层栈对象，
		for (Layer* layer : m_LayerStack) 
		{
			layer->OnDetach();
		}

		Renderer::Shutdown();
	}

	//===============================================
	void Application::PushLayer(Layer* layer) 
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) 
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}
	//===============================================

	void Application::Run()
	{
		while (m_Running)
		{
			float timeStep = m_Time->GetTime();

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpDate(timeStep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			//渲染窗口
			m_Window->OnUpDate();
		}
	}

	void Application::OnEvents(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENTS(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENTS(OnWindowResize));
		//GD_ENGINE_INFO("{0}", e);

		for (std::vector<Layer*>::reverse_iterator it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvents(e);
			if (e.GetHandle())
				break;
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}
}
