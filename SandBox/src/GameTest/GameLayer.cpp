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
