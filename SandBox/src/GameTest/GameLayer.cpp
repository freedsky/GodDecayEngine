#include "GameLayer.h"


GameLayer::GameLayer(std::string name)
	:Layer(name)
{
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpDate()
{
	
}

void GameLayer::OnEvents(const GodDecay::Event& e)
{
	//GD_CLIENT_DEBUG("{0}", e.GetName());
}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("sda");
	ImGui::Text("sdas");
	ImGui::Button("new", ImVec2(100, 100));
	ImGui::End();
}
