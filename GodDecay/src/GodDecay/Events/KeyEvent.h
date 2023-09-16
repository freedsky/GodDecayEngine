#pragma once
#include "Event.h"

#include <sstream>

namespace GodDecay
{
	/// <summary>
	/// 按键事件基类
	/// </summary>
	class GODDECAY_API KeyEvent : public Event
	{
	public:
		inline int GetKetCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			:m_KeyCode(keycode)
		{

		}
		int m_KeyCode;
	};

	/// <summary>
	/// 按键按下事件
	/// </summary>
	class GODDECAY_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode)
			: KeyEvent(keycode){}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " Down Code";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	/// <summary>
	/// 按键释放事件
	/// </summary>
	class GODDECAY_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	/// <summary>
	/// 按键重复事件
	/// </summary>
	class GODDECAY_API KeyRepetiedEvent : public KeyEvent 
	{
	public:
		KeyRepetiedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyRepetiedEvent: " << m_KeyCode << " Repetied Code";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyRepetied)
	};
}