#pragma once

#include "GodDecay/Input.h"
#include "GodDecay/Application.h"

/// <summary>
/// windows平台下利用GLFW来实现输入轮询
/// </summary>
namespace GodDecay 
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(uint32_t keycode) const override;
		virtual bool IsMosueButtonPressedImpl(uint32_t button) const override;
		virtual std::pair<float, float> GetMosuePositionImpl() const override;
		virtual float GetMouseXImpl() const override;
		virtual float GetMouseYImpl() const override;
	};
}



