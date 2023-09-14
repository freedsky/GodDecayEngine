#pragma once
#include "gdpch.h"

#include "Core.h"
#include "Events/Event.h"

namespace GodDecay 
{
	struct WindowProperty
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProperty(std::string title = "GodDecay Game Engine", uint32_t width = 1280, uint32_t height = 720) 
			:Title(title),Width(width),Height(height)
		{}
	};

	class GODDECAY_API Window 
	{
	public:
		virtual ~Window() = default;

		virtual void OnUpDate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		//event
		using EventCallbackFn = std::function<void(Event&)>;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		static Ref<Window> Create(const WindowProperty& property = WindowProperty());
	};
}
