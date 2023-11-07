#pragma once
#include <GodDecayTop.h>

#include "GodDecay/Events/KeyEvent.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "GodDecay/Renderer/EditorCamera.h"

/// <summary>
/// ������
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
		bool OnKeyPressed(KeyRepetiedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		Ref<OrthographicCameraController> m_Camera;
		EditorCamera m_EditorCamera;
		
		GodDecay::Ref<GodDecay::Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f,0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;//ʵ��չʾ���
		ContentBrowserPanel m_ContentBrowserPanel;//�ļ�������

		Ref<Scene> m_ActionScene;

		//����ѡ����Ӧ����תλ��ģ��
		uint32_t m_GizmoType = -1;

		//��ȡ��Ļ�����µ�xy
		glm::vec2 m_ViewprotBounds[2];

		//��ȡ����ǰ�������ָ���entity
		Entity  m_HoveredEntity;
	};
}

