#include "GameLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GameLayer::GameLayer(std::string name)
	:Layer(name), m_Camera(GodDecay::CreateRef<GodDecay::OrthographicCameraController>(1280.0f / 720.0f))
{
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	m_TextureColor = glm::vec4(1.0f);
	m_SquareTexture = GodDecay::Texture2D::Create("assets/texture/ChernoLogo.png");
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpDate(float deltaTime)
{
	GD_PROFILE_FUNCTION();
	{
		GD_PROFILE_SCOPE("CameraController::OnUpdate");
		m_Camera->OnUpdate(deltaTime);
	}
	
	{
		GD_PROFILE_SCOPE("Renderer Prep");
		GodDecay::RenderCommand::SetClearColor(glm::vec4(0.1, 0.1, 0.1, 1.0));
		GodDecay::RenderCommand::Clear();
	}
	
	GodDecay::Renderer2D::ResetStats();
	{
		GD_PROFILE_SCOPE("Renderer Draw");
		GodDecay::Renderer2D::BeginScene(*m_Camera->GetCamera().get());

		GodDecay::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), m_TextureColor);

		GodDecay::Renderer2D::DrawQuad(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0,0.0,0.0,1.0));


		GodDecay::Renderer2D::DrawRotatedQuad(glm::vec2(-2.0f, -2.0f), glm::vec2(1.0f, 1.0f), 45.0f,m_TextureColor);

		GodDecay::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(10.0f, 10.0f), m_SquareTexture, m_TextureColor, 20);

		GodDecay::Renderer2D::DrawRotatedQuad(glm::vec2(5.0f, 5.0f), glm::vec2(2.0f, 2.0f),45.0f ,m_SquareTexture, m_TextureColor);

		GodDecay::Renderer2D::EndScene();
	}
	
}

void GameLayer::OnEvents(GodDecay::Event& e)
{
	m_Camera->OnEvent(e);
}

void GameLayer::OnImGuiRender()
{
	static bool dockingEnabled = true;
	if (dockingEnabled)
	{
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

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) GodDecay::Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::Begin("Settings");

		auto stats = GodDecay::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("squareColor", glm::value_ptr(m_TextureColor));

		uint32_t textureID = m_SquareTexture->GetRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });
		ImGui::End();

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Status");

		auto stats = GodDecay::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();
	}
}
