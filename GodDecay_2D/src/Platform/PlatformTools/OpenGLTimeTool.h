#pragma once

#include "GodDecay/Tools/TimeTool.h"

/// <summary>
/// ��OpenGL��ȥʱ��Ļ�ȡ
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



