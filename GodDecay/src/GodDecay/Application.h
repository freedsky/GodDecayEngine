#pragma once
/// <summary>
/// Ӧ�ó�����
/// </summary>
/// <returns>
/// ��������Ӧ�ó����������͹ر�ʱ������ʼ�����ͷŸ�������
/// </returns>
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

#include "GodDecay/Events/Event.h"
#include "GodDecay/Events/ApplicationEvent.h"

namespace GodDecay 
{
	class GODDECAY_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void OnEvents(Event& e);

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		Ref<Window> window = nullptr;
		bool m_Running = true;
		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& e);
	};

	//������ͷ�ļ��Լ��̳�Application���������Ҫ���������ļ���ʵ�ֶ���
	Application* CreateApplication();
}



