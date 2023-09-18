#pragma once

#include "Core.h"

/// <summary>
/// 全局的输入轮询
/// </summary>
namespace GodDecay 
{
	class GODDECAY_API Input 
	{
	public:
		inline static bool IsKeyPressed(uint32_t keycode) { return m_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMosueButtonPressed(uint32_t button) { return m_Instance->IsMosueButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMosuePosition() { return m_Instance->GetMosuePositionImpl(); }
		inline static float GetMouseX() { return m_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return m_Instance->GetMouseYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(uint32_t keycode) const = 0;
		virtual bool IsMosueButtonPressedImpl(uint32_t button) const = 0;
		virtual std::pair<float, float> GetMosuePositionImpl() const = 0;
		virtual float GetMouseXImpl() const = 0;
		virtual float GetMouseYImpl() const = 0;
	private:
		static Input* m_Instance;
	};
}
