#pragma once
/// <summary>
/// Ӧ�ó�����
/// </summary>
/// <returns>
/// ��������Ӧ�ó����������͹ر�ʱ������ʼ�����ͷŸ�������
/// </returns>
#include "Core.h"
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
		Application();
		virtual ~Application() = default;

		void OnEvents(Event& e);

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Ref<Window> GetWindow() { return m_Window; }
		inline static Application& Get() { return *m_Instance; }
	private:
		Ref<Window> m_Window = nullptr;
		static Application* m_Instance;
		bool m_Running = true;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		Ref<Time> m_Time;

		bool OnWindowClose(WindowCloseEvent& e);
	};

	//������ͷ�ļ��Լ��̳�Application���������Ҫ���������ļ���ʵ�ֶ���
	Application* CreateApplication();
}



