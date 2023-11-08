#include "EditorLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include "GodDecay/Scene/SceneSerializer.h"
#include "GodDecay/Utils/PlatformUtils.h"

#include "Scripts/CameraContorller.hpp"

#include "GodDecay/Math/Math.h"

namespace GodDecay
{
	//����һ��ȫ�ֵ�·������
	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer(std::string name)
		:Layer(name), m_Camera(GodDecay::CreateRef<GodDecay::OrthographicCameraController>(1280.0f / 720.0f))
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		m_IconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		m_IconStop = Texture2D::Create("Resources/Icons/StopButton.png");

		GodDecay::FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8,FramebufferTextureFormat::RED_INTEGER,FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = GodDecay::Framebuffer::Create(fbSpec);

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		//-----------entt
		m_ActionScene = CreateRef<Scene>();

		m_SceneHierarchyPanel.SetContext(m_ActionScene);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpDate(float deltaTime)
	{
		//��Updata��ÿ֡ȥ���viewport�Ƿ�����˸ı�
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			//m_CameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y);
			//����Ϊ�༭���
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			//�������������viewport��С
			m_ActionScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
		GodDecay::Renderer2D::ResetStats();
		m_Framebuffer->Bind();

		GodDecay::RenderCommand::SetClearColor(glm::vec4(0.1, 0.1, 0.1, 1.0));
		GodDecay::RenderCommand::Clear();
		//�������Red��ɫ����
		m_Framebuffer->ClearAttachment(1, -1);

		//�༭����״̬����
		switch (m_SceneState)
		{
		case SceneState::Edit:
		{
			if (m_ViewportFocused)
				m_Camera->OnUpdate(deltaTime);

			m_EditorCamera.OnUpdate(deltaTime);

			m_ActionScene->UpdateEditor(deltaTime, m_EditorCamera);
			break;
		}
		case SceneState::Play:
		{
			m_ActionScene->OnUpdateRuntime(deltaTime);
			break;
		}
		}
		//�������λ����viewprot�е�Position
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewprotBounds[0].x;
		my -= m_ViewprotBounds[0].y;
		glm::vec2 viewportSize = m_ViewprotBounds[1] - m_ViewprotBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		//��ֹ��viewport������Ҳ����ȡֵ
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActionScene.get());
			//GD_ENGINE_WARN("Pixel data {0} ", pixelData);
		}

		m_Framebuffer->UnBind();
	}

	void EditorLayer::OnEvents(GodDecay::Event& e)
	{
		m_Camera->OnEvent(e);

		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyRepetiedEvent>(std::bind(&EditorLayer::OnKeyPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&EditorLayer::OnMouseButtonPressed, this, std::placeholders::_1));
	}

	void EditorLayer::OnImGuiRender()
	{
		//���ø�������������
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
		//���ڵ�����
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				//�Գ����е����ݽ��б���ͼ���
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
		//�������
		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();
		//---------------------Test����--------------------------------------------
		ImGui::Begin("Stats");

		//��ʾ���ѡ���е��Ǹ�ʵ������
		std::string name = "None";
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = GodDecay::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		//ͨ�������ȡ���������ϵ��������
		ImGui::End();

		//View���ڣ�����������ƶ������������Ͳ��������С��ʱ���ڳ��ֱ��в�Э�������⣩
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });//����һ�������ʽ��ע��Ҫ��ImGui::PopStyleVar();����
		ImGui::Begin("Viewpoint");
		//�õ�view����Ĵ��ڵ���Сֵ�����ֵ���ꡣ
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		//��������ж���Ļ�ռ�����Ļ�ȡ
		auto viewportOffset = ImGui::GetWindowPos();
		//���µ�ǰ��view���ڵ������С������
		m_ViewprotBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewprotBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		//�жϵ�ǰ���ľ۽������Ƿ��ڸô��ڣ���ֹ�¼���͸
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
		//���Ի�ȡ����ǰbegine�������ڵĴ�С,�ڴ˴�����view��С����updataʵ�ʸ���framebuffer�Ĵ�С�������ڵ���ʱ�Ų����������������
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		//��Ⱦframebuffer
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x,m_ViewportSize.y }, { 0,1 }, { 1,0 });

		//������ק�����Ŀ������
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetPath) / path);
				//GD_ENGINE_INFO("DragDropFilepath = {0}", std::filesystem::path(g_AssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}


		//ImGuizmo
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			//�������۵�ͼʾ
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			//����zom��view���ڵ���С���������
			ImGuizmo::SetRect(m_ViewprotBounds[0].x, m_ViewprotBounds[0].y, m_ViewprotBounds[1].x - m_ViewprotBounds[0].x, m_ViewprotBounds[1].y - m_ViewprotBounds[0].y);

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

		UI_Toolbar();

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

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == GODDECAY_MOUSE_BUTTON_LEFT)
		{

			if (m_ViewportHovered && !ImGuizmo::IsOver() && Input::IsKeyPressed(GODDECAY_KEY_LEFT_ALT))
			{
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
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
		//ÿ����һ���ļ�ʱ�����ȴ���һ��[Ϊ�³������¸�ֵ]��Ȼ���ڷ����л�
		std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.gd)\0*.gd\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		//����ļ����ļ���׺���Ƿ���ȷ
		if (path.extension().string() != ".gd")
		{
			GD_ENGINE_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		m_ActionScene = CreateRef<Scene>();
		m_ActionScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActionScene);

		SceneSerializer serializer(m_ActionScene);
		serializer.Deserialize(path.string());
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

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;
	}

	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_SceneState == SceneState::Edit)
				OnScenePlay();
			else if (m_SceneState == SceneState::Play)
				OnSceneStop();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}
}
