#pragma once
#include <GodDecayTop.h>

#include <imgui.h>

#include "GodDecay/Renderer/EditorCamera.h"
#include "Panels/SceneHierarchyPanel.h"

/// <summary>
/// 引擎编辑层
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
		bool OnKeyPressed(KeyRepetiedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		Ref<GodDecay::PerspectiveCameraController> m_Camera;
		EditorCamera m_EditorCamera;
		
		GodDecay::Ref<GodDecay::Framebuffer> m_Framebuffer;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		//entt
		Ref<Scene> m_ActionScene;
		//ImGuizmo
		uint32_t m_GizmoType = -1;

		//获取屏幕坐标下的xy
		glm::vec2 m_ViewprotBounds[2];
		//获取entityID
		Entity  m_HoveredEntity;
	};
}