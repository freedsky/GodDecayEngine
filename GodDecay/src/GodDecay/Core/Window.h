#pragma once
#include "gdpch.h"

#include "Base.h"
#include "GodDecay/Events/Event.h"
/// <summary>
/// 抽象窗口类，因为不同平台的窗口实现不一
/// </summary>
namespace GodDecay 
{
	struct WindowProperty
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProperty(std::string title = "GodDecay Game Engine", uint32_t width = 1600, uint32_t height = 900) 
			:Title(title),Width(width),Height(height)
		{}
	};

	class Window 
	{
	public:
		virtual ~Window() = default;

		virtual void OnUpDate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;
		//event
		using EventCallbackFn = std::function<void(Event&)>;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetHideMouse(bool enabled) = 0;

		static Scope<Window> Create(const WindowProperty& property = WindowProperty());
	};
}
