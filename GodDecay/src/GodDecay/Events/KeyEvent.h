#pragma once
#include "Event.h"

#include <sstream>

namespace GodDecay
{
	/// <summary>
	/// �����¼�����
	/// </summary>
	class KeyEvent : public Event
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
	/// ���������¼�
	/// </summary>
	class KeyPressedEvent : public KeyEvent
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
	/// �����ͷ��¼�
	/// </summary>
	class KeyReleasedEvent : public KeyEvent
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
	/// �����ظ��¼�
	/// </summary>
	class KeyRepetiedEvent : public KeyEvent 
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

	/// <summary>
	/// �����ַ��������¼�
	/// </summary>
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEevnt: " << m_KeyCode << " Code";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}