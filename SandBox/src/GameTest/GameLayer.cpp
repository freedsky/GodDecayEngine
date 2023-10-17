#include "GameLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GameLayer::GameLayer(std::string name)
	:Layer(name), m_Camera(GodDecay::CreateRef<GodDecay::PerspectiveCameraController>(1280.0f / 720.0f))
{
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	m_2DSquareColor = glm::vec4(1.0f);
	m_3DCubeColor = glm::vec4(1.0f);
	m_3DCirleColor = glm::vec4(1.0f);

	m_SqueraTexure = GodDecay::Texture2D::Create("assets/texture/ChernoLogo.png");
	m_CubeTexture = GodDecay::Texture2D::Create("assets/texture/bricks2.jpg");
	m_CirleTexture = GodDecay::Texture2D::Create("assets/texture/matrix.jpg");
	m_ModelTexture = GodDecay::Texture2D::Create("assets/texture/wall.jpg");
}

void GameLayer::OnDetach()
{
	
}

void GameLayer::OnUpDate(float deltaTime)
{
	m_Camera->OnUpdate(deltaTime);

	GodDecay::RenderCommand::SetClearColor(glm::vec4(0.1, 0.1, 0.1, 1.0));
	GodDecay::RenderCommand::Clear();

	//===========================================================================================================
	GodDecay::Renderer2D::ResetStats();
	//-----------------------------------------------
	GodDecay::Renderer2D::BeginScene(*m_Camera->GetCamera().get());

	GodDecay::Renderer2D::DrawQuad(glm::vec2(2.0f, 2.0f), glm::vec2(1.0f, 1.0f), m_2DSquareColor);

	GodDecay::Renderer2D::DrawQuad(glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, 1.0f), m_2DSquareColor);

	GodDecay::Renderer2D::DrawQuad(glm::vec2(2.0f, -2.0f), glm::vec2(20.0f, 20.0f), m_CubeTexture, m_2DSquareColor, 30);

	GodDecay::Renderer2D::DrawQuad(glm::vec2(0.0f, 2.0f), glm::vec2(1.0f, 1.0f), m_SqueraTexure);

	GodDecay::Renderer2D::DrawRotatedQuad(glm::vec2(4.0f, 4.0f), glm::vec2(1.0f, 1.0f), 45.0f, m_SqueraTexure, m_2DSquareColor);

	GodDecay::Renderer2D::EndScene();

	GodDecay::Renderer3D::BeginScene(*m_Camera->GetCamera().get());

	GodDecay::Renderer3D::DrawCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), m_2DSquareColor);
	GodDecay::Renderer3D::DrawCube(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), m_CubeTexture, m_2DSquareColor);
	GodDecay::Renderer3D::DrawRotationCube(glm::vec3(1.0f, 1.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), 45.0f,glm::vec3(0.5,0.8,0.3), m_CubeTexture, m_2DSquareColor);

	GodDecay::Renderer3D::DrawCirle(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), m_2DSquareColor);
	GodDecay::Renderer3D::DrawCirle(glm::vec3(2.0f, 2.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f), m_CirleTexture, m_2DSquareColor);

	GodDecay::Renderer3D::EndScene();
	//===========================================================================================================

}

void GameLayer::OnEvents(GodDecay::Event& e)
{
	m_Camera->OnEvent(e);
}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("sda");
	ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_2DSquareColor));
	ImGui::ColorEdit4("CubeColor", glm::value_ptr(m_3DCubeColor));
	ImGui::ColorEdit4("CirleColor", glm::value_ptr(m_3DCirleColor));

	auto stats = GodDecay::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}
