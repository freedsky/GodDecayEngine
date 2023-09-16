#pragma once

#include "GodDecay/Core.h"
#include "GodDecay/Layer.h"


/// <summary>
/// ImGui�㣬������������gui�Ĵ����͹����Լ�����
/// </summary>
namespace GodDecay 
{
	class GODDECAY_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpDate() override;
		void OnEvents(const Event& e) override;
	private:
		float m_Time = 0.0f;
	};
}



