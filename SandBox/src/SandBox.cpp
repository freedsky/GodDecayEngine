#include "gdpch.h"

#include <GodDecayTop.h>

class SandBox : public GodDecay::Application
{
public:
	SandBox()
	{

	}

	~SandBox()
	{

	}
};

GodDecay::Application* GodDecay::CreateApplication()
{
	return new SandBox();
}
