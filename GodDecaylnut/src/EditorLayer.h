#pragma once
#include <GodDecayTop.h>

#include <imgui.h>

#include "Panels/SceneHierarchyPanel.h"

/// <summary>
/// ÒýÇæ±à¼­²ã
/// </summary>
 
namespace GodDecay 
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer(std::string name);
		~EditorLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpDate(float deltaTime) override;
		void OnEvents(Event& e) override;
		void OnImGuiRender() override;
	private:
		Ref<GodDecay::PerspectiveCameraController> m_Camera;
		GodDecay::Ref<GodDecay::Framebuffer> m_Framebuffer;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		//entt test
		Ref<Scene> m_ActionScene;
		Entity m_FirstCamera;
		Entity m_SecondCamera;
		bool m_PrimaryCamera = true;

		
	};
}