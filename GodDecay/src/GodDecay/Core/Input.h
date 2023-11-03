#pragma once

#include "Base.h"

/// <summary>
/// ȫ�ֵ�������ѯ
/// </summary>
namespace GodDecay 
{
	class Input 
	{
	public:
		static bool IsKeyPressed(uint32_t keycode);
		static bool IsMouseButtonPressed(uint32_t button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY(); 
	};
}
