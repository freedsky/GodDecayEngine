#include "EditorLayer.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		m_TextureColor = glm::vec4(1.0f);
		m_SquareTexture = GodDecay::Texture2D::Create("assets/texture/ChernoLogo.png");

		GodDecay::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = GodDecay::Framebuffer::Create(fbSpec);

		//-----------entt---test----------
		m_ActionScene = CreateRef<Scene>();

		auto bule = m_ActionScene->CreateEntity("Bule Square");
		bule.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		auto red = m_ActionScene->CreateEntity("Red Square");
		red.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		m_FirstCamera = m_ActionScene->CreateEntity("First");
		m_FirstCamera.AddComponent<CameraComponent>();
		m_FirstCamera.AddComponent<NativeScriptComponent>().Bind<CameraContorller>();


		m_SecondCamera = m_ActionScene->CreateEntity("Second");
		auto& c = m_SecondCamera.AddComponent<CameraComponent>();
		c.Primary = false;

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
			//给场景相机调整viewport大小
			m_ActionScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		//防止事件在相机和UI之间进行穿透
		/*if (m_ViewportFocused) 
		{
			m_Camera->OnUpdate(deltaTime);
		}*/
		m_Framebuffer->Bind();

		GodDecay::RenderCommand::SetClearColor(glm::vec4(0.1, 0.1, 0.1, 1.0));
		GodDecay::RenderCommand::Clear();

		GodDecay::Renderer2D::ResetStats();
			
		m_ActionScene->OnUpdata(deltaTime);

		m_Framebuffer->UnBind();
	}

	void EditorLayer::OnEvents(GodDecay::Event& e)
	{
		m_Camera->OnEvent(e);
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
		ImGui::End();
		ImGui::PopStyleVar();
		
		ImGui::End();
	}
}
