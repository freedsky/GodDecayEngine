#pragma once

#include "GodDecay/Core.h"
#include "GodDecay/Layer.h"

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

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}



