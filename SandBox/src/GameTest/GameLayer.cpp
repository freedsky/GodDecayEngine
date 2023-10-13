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
	ImGui::Begin("Starus");
	ImGui::ColorEdit4("TextureColor", glm::value_ptr(m_TextureColor));
	auto stats = GodDecay::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
}
