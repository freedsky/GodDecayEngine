#pragma once

#include "GodDecay/Tools/TimeTool.h"

namespace GodDecay 
{
	class OpenGLTimeTool : public Time
	{
	public:
		OpenGLTimeTool(TimeType type);
		~OpenGLTimeTool();

		virtual float GetTime() override;

	private:
		float DeltaTime;
		float LastFrameTime;

		float GetDeltaTime();
	};
}



