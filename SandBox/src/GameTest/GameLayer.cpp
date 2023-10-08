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

	model = GodDecay::CreateRef<GodDecay::Model>("assets/model/suzanne.obj");
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

	GodDecay::Renderer2D::DrawQuad(glm::vec2(0.0f, 2.0f), glm::vec2(1.0f,1.0f), m_2DSquareColor);

	GodDecay::Renderer2D::EndScene();

	//====================================================

	GodDecay::Renderer3D::BeginScene(*m_Camera->GetCamera().get());

	GodDecay::Renderer3D::DrawCirle(glm::vec3(0.0f, 0.0f,0.0f), glm::vec2(1.0f, 1.0f), m_3DCirleColor);

	GodDecay::Renderer3D::DrawCube(glm::vec3(0.0f, 0.0f,0.0f), glm::vec2(1.0f, 1.0f), m_3DCubeColor);

	GodDecay::Renderer3D::EndScene();

	//=====================================================

	GodDecay::Renderer3D::BeginSceneModel(*m_Camera->GetCamera().get(), model->GetModelData()->ModelShader);
	
	GodDecay::Renderer3D::DrawModel(model->GetModelData(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), m_3DCubeColor);

	GodDecay::Renderer3D::EndScene();
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
	ImGui::End();
}
