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
	auto [x, y] = GodDecay::Input::GetMosuePosition();
	GD_CLIENT_DEBUG("x = {0}, y = {1}", x, y);
}

void GameLayer::OnEvents(const GodDecay::Event& e)
{
	//GD_CLIENT_DEBUG("{0}", e.GetName());
}
