#include "gdpch.h"

#include <GodDecayTop.h>

#include "GameTest/GameLayer.h"

class SandBox : public GodDecay::Application
{
public:
	SandBox()
	{
		PushLayer(new GameLayer("Game?"));
		PushLayer(new GodDecay::ImGuiLayer());
	}

	~SandBox()
	{

	}
};

GodDecay::Application* GodDecay::CreateApplication()
{
	return new SandBox();
}
