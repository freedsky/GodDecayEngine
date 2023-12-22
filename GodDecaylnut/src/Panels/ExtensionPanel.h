#pragma once
#include <imgui.h>

#include "GodDecay/Core/Base.h"
#include "GodDecay/Core/Logger.h"

/// <summary>
/// 这里的扩展窗口应该工厂模式。
/// 但为了简化开发，直接以方法进行构造
/// 
/// //单例模式[全局差不多只需要有一个扩展窗口类]
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

		//用于控制显示窗口
		void DisPlayExtensionWindow(ExtensionWidnowType type);
		
	private:
		ExtensionPanel();


		//SkyBox的设置窗口
		void SkyBoxSettingWindow();

		static Ref<ExtensionPanel> m_instance;
	};
}

