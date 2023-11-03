#include "EditorLayer.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GodDecay/Math/Math.h"

#include "GodDecay/Scene/SceneSerializer.h"
#include "GodDecay/Utils/PlatformUtils.h"

#include "Scripts/CameraController.hpp"

namespace GodDecay 
{
	EditorLayer::EditorLayer(std::string name)
		:Layer(name), m_Camera(CreateRef<GodDecay::OrthographicCameraController>(1280.0f / 720.0f))
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		GodDecay::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = GodDecay::Framebuffer::Create(fbSpec);

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_ActionScene = CreateRef<Scene>();
		//在场景创建完成后才添加进面板
		m_SceneHierarchyPanel.SetContext(m_ActionScene);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpDate(float deltaTime)
	{
		//在Updata中每帧去检测viewport是否进行了改变
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			//m_CameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y);
			//调整为编辑相机
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			//给场景相机调整viewport大小
			m_ActionScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		m_EditorCamera.OnUpdate(deltaTime);
		//防止事件在相机和UI之间进行穿透
		/*if (m_ViewportFocused) 
		{
			m_Camera->OnUpdate(deltaTime);
		}*/
		m_Framebuffer->Bind();

		GodDecay::RenderCommand::SetClearColor(glm::vec4(0.1, 0.1, 0.1, 1.0));
		GodDecay::RenderCommand::Clear();

		GodDecay::Renderer2D::ResetStats();
			
		//m_ActionScene->OnUpdateRuntime(deltaTime);
		m_ActionScene->UpdateEditor(deltaTime, m_EditorCamera);

		m_Framebuffer->UnBind();
	}

	void EditorLayer::OnEvents(GodDecay::Event& e)
	{
		//m_Camera->OnEvent(e);
		
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyRepetiedEvent>(std::bind(&EditorLayer::OnKeyPressed, this, std::placeholders::_1));
	}

	void EditorLayer::OnImGuiRender()
	{
		//启用浮动和吸附窗口
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		//窗口导航栏
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				//对场景中的数据进行保存和加载
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit")) GodDecay::Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		//属性面板更新
		m_SceneHierarchyPanel.OnImGuiRender();
		//状态栏===============================================================
		ImGui::Begin("Status");
		auto stats = GodDecay::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		//通过组件获取到物体身上的组件引用
		ImGui::End();

		//View窗口（把这个窗口移动到最下面好像就不会出现最小化时窗口出现比列不协调的问题）
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });//推送一个风格样式，注意要以ImGui::PopStyleVar();结束
		ImGui::Begin("Viewpoint");
		//判断当前鼠标的聚焦区域是否在该窗口，防止事件穿透
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
		//可以获取到当前begine创建窗口的大小,在此处更新view大小，在updata实际更新framebuffer的大小，这样在调整时才不会出现闪屏的现象
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		//渲染framebuffer
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x,m_ViewportSize.y }, { 0,1 }, { 1,0 });
		
		//ImGuizmo
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1) 
		{
			//禁用正价的图示
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Editor Camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}
		
		ImGui::End();
		ImGui::PopStyleVar();
		
		ImGui::End();
	}

	//=======================================

	bool EditorLayer::OnKeyPressed(KeyRepetiedEvent& e)
	{
		bool control = Input::IsKeyPressed(GODDECAY_KEY_LEFT_CONTROL) || Input::IsKeyPressed(GODDECAY_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(GODDECAY_KEY_LEFT_SHIFT) || Input::IsKeyPressed(GODDECAY_KEY_RIGHT_SHIFT);
		switch (e.GetKetCode())
		{
			case GODDECAY_KEY_N:
			{
				if (control)
					NewScene();

				break;
			}
			case GODDECAY_KEY_O:
			{
				if (control)
					OpenScene();

				break;
			}
			case GODDECAY_KEY_S:
			{
				if (control && shift)
					SaveSceneAs();

				break;
			}
			//ImGuizmo
			case GODDECAY_KEY_Q:
				m_GizmoType = -1;
				break;
			case GODDECAY_KEY_W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case GODDECAY_KEY_E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case GODDECAY_KEY_R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActionScene = CreateRef<Scene>();
		m_ActionScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActionScene);
	}

	void EditorLayer::OpenScene()
	{
		//每当打开一个文件时，都先创建一个[为新场景重新赋值]，然后在反序列化
		std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.gd)\0*.gd\0");
		if (!filepath.empty())
		{
			m_ActionScene = CreateRef<Scene>();
			m_ActionScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActionScene);

			SceneSerializer serializer(m_ActionScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Hazel Scene (*.gd)\0*.gd\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActionScene);
			serializer.Serialize(filepath);
		}
	}
}
