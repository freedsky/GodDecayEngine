#include "gdpch.h"
#include "OpenGLTimeTool.h"

#include <GLFW/glfw3.h>

namespace GodDecay 
{
	OpenGLTimeTool::OpenGLTimeTool(TimeType type)
		: Time(type)
	{
		DeltaTime = 0.0f;
		LastFrameTime = 0.0f;
	}

	OpenGLTimeTool::~OpenGLTimeTool()
	{
	}

	float OpenGLTimeTool::GetTime()
	{
		switch (GetTimeType()) 
		{
		case TimeType::None: GD_ENGINE_ASSERT(false, "this Time Type is Null"); return -1.0f;
		case TimeType::DeltaTime: return GetDeltaTime(); break;
		}

		GD_ENGINE_ASSERT(false, "this Time Type is UnKnow");
		return -1.0f;
	}

	float OpenGLTimeTool::GetDeltaTime()
	{
		float CurrentTime = (float)glfwGetTime();
		DeltaTime = CurrentTime - LastFrameTime;
		LastFrameTime = CurrentTime;

		return DeltaTime;
	}
}


