#pragma once

#include "GodDecay/Core.h"
#include "GodDecay/Layer.h"

/// <summary>
/// ImGui层，它管理着整个gui的创建和管理以及更新
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



