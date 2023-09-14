#pragma once
/// <summary>
/// Ӧ�ó�����
/// </summary>
/// <returns>
/// ��������Ӧ�ó����������͹ر�ʱ������ʼ�����ͷŸ�������
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

	//������ͷ�ļ��Լ��̳�Application���������Ҫ���������ļ���ʵ�ֶ���
	Application* CreateApplication();
}



