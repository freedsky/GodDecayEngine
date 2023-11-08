#pragma once
#include <GodDecayTop.h>

#include "GodDecay/Events/KeyEvent.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "GodDecay/Renderer/EditorCamera.h"

/// <summary>
/// 测试类
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
		//通过拖拽的方式对场景文件进行加载
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();

		//运行组件
		void OnScenePlay();
		void OnSceneStop();

		void UI_Toolbar();
	private:
		Ref<OrthographicCameraController> m_Camera;
		EditorCamera m_EditorCamera;
		
		GodDecay::Ref<GodDecay::Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f,0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;//实体展示面板
		ContentBrowserPanel m_ContentBrowserPanel;//文件浏览面板

		Ref<Scene> m_ActionScene;

		//用于选择相应的旋转位移模型
		uint32_t m_GizmoType = -1;

		//获取屏幕坐标下的xy
		glm::vec2 m_ViewprotBounds[2];

		//获取到当前的鼠标所指向的entity
		Entity  m_HoveredEntity;

		//定义编辑运行状态
		enum SceneState
		{
			Edit = 0, Play = 1
		};
		//默认状态设置为编辑状态
		SceneState m_SceneState = SceneState::Edit;

		//运行和暂停是的图标纹理设置
		Ref<Texture2D> m_IconPlay, m_IconStop;
	};
}

