#include "gdpch.h"

#include <GodDecayTop.h>

#include "GameTest/GameLayer.h"

class SandBox : public GodDecay::Application
{
public:
	SandBox()
	{
		PushLayer(new GameLayer("Game?"));
	}

	~SandBox()
	{

	}
};

GodDecay::Application* GodDecay::CreateApplication()
{
	return new SandBox();
}
