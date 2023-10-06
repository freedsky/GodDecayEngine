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
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpDate(float deltaTime)
{
	m_Camera->OnUpdate(deltaTime);

	GodDecay::RenderCommand::SetClearColor(glm::vec4(0.1, 0.1, 0.1, 1.0));
	GodDecay::RenderCommand::Clear();

	GodDecay::Renderer2D::BeginScene(*m_Camera->GetCamera().get());

	GodDecay::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f,1.0f), m_TextureColor);

	GodDecay::Renderer2D::EndScene();
}

void GameLayer::OnEvents(GodDecay::Event& e)
{
	m_Camera->OnEvent(e);
}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("sda");
	ImGui::ColorEdit4("TextureColor", glm::value_ptr(m_TextureColor));
	ImGui::End();
}
