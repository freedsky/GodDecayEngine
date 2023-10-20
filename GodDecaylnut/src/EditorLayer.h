#pragma once
#include <GodDecayTop.h>

#include "Panels/SceneHierarchyPanel.h"

/// <summary>
/// ≤‚ ‘¿‡
/// </summary>

namespace GodDecay 
{
	class EditorLayer : public GodDecay::Layer
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
		Ref<OrthographicCameraController> m_Camera;
		GodDecay::Ref<GodDecay::Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f,0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		//entt test
		Ref<Scene> m_ActionScene;
		Entity m_SquareEntity;
		Entity m_FirstCamera;
		Entity m_SecondCamera;
		bool m_PrimaryCamera = true;

		glm::vec4 m_TextureColor;
		Ref<Texture2D> m_SquareTexture;
	};
}

