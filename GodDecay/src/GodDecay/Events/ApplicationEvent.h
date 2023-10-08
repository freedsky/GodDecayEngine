#pragma once
#include "Event.h"

#include <sstream>

namespace GodDecay
{
	/// <summary>
	/// 窗口缩放事件
	/// </summary>
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	/// <summary>
	/// 窗口关闭事件
	/// </summary>
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	/// <summary>
	/// 应用程序每次的tick事件
	/// </summary>
	class ApplicationTickEvent : public Event
	{
	public:
		ApplicationTickEvent() {}

		EVENT_CLASS_TYPE(ApplicationTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	/// <summary>
	/// 应用程序更新阶段
	/// </summary>
	class ApplicationUpdateEvent : public Event
	{
	public:
		ApplicationUpdateEvent() {}

		EVENT_CLASS_TYPE(ApplicationUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	/// <summary>
	/// 应用程序渲染更新事件
	/// </summary>
	class ApplicationRenderEvent : public Event
	{
	public:
		ApplicationRenderEvent() {}

		EVENT_CLASS_TYPE(ApplicationRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}