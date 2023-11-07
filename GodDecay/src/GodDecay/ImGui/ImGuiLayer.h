#pragma once

#include "GodDecay/Core/Base.h"
#include "GodDecay/Core/Layer.h"

/// <summary>
/// ImGui�㣬������������gui�Ĵ����͹����Լ�����
/// </summary>
namespace GodDecay 
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;
		virtual void OnEvents(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		//����������ɫ
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
	};
}



