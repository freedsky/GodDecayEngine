#pragma once
#include <imgui.h>

#include "GodDecay/Core/Base.h"
#include "GodDecay/Core/Logger.h"

/// <summary>
/// �������չ����Ӧ�ù���ģʽ��
/// ��Ϊ�˼򻯿�����ֱ���Է������й���
/// 
/// //����ģʽ[ȫ�ֲ��ֻ��Ҫ��һ����չ������]
/// </summary>
namespace GodDecay 
{
	class ExtensionPanel
	{
	public:
		enum class ExtensionWidnowType 
		{
			None = 0,
			SkyBox
		};

		static bool IsOpenSkyBox;

	public:
		static Ref<ExtensionPanel> GetInstance();

		~ExtensionPanel();

		//���ڿ�����ʾ����
		void DisPlayExtensionWindow(ExtensionWidnowType type);
		
	private:
		ExtensionPanel();


		//SkyBox�����ô���
		void SkyBoxSettingWindow();

		static Ref<ExtensionPanel> m_instance;
	};
}

