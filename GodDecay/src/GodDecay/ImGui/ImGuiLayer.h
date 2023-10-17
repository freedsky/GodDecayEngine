#pragma once

#include "GodDecay/Core/Base.h"
#include "GodDecay/Core/Layer.h"

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
		virtual void OnEvents(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}



