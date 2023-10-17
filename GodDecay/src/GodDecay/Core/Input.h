#pragma once

#include "Base.h"

/// <summary>
/// 全局的输入轮询
/// </summary>
namespace GodDecay 
{
	class Input 
	{
	public:
		static bool IsKeyPressed(uint32_t keycode);
		static bool IsMosueButtonPressed(uint32_t button);
		static std::pair<float, float> GetMosuePosition();
		static float GetMouseX();
		static float GetMouseY(); 
	};
}
