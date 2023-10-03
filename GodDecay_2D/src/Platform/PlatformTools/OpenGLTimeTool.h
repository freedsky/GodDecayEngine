#pragma once

#include "GodDecay/Tools/TimeTool.h"

/// <summary>
/// 在OpenGL中去时间的获取
/// </summary>
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



