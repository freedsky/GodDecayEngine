#pragma once

#include "GodDecay/Core.h"
#include "GodDecay/Window.h"
#include "GodDecay/Renderer/RendererContext.h"

#include <GLFW/glfw3.h>

/// <summary>
/// Windows窗口类的实现
/// 问：是否要把GLFW的部分抽象出来？
/// </summary>
namespace GodDecay 
{
	class WindowsWindow : public Window 
	{
	public:
		WindowsWindow(const WindowProperty& property);
		virtual ~WindowsWindow();

		void OnUpDate() override;

		inline uint32_t GetWidth() const { return m_WindowDate.width; }
		inline uint32_t GetHeight() const { return m_WindowDate.height; }

		inline void* GetNativeWindow() const override { return m_Window; }

		inline void SetEventCallback(const EventCallbackFn& callback) { m_WindowDate.EventCallback = callback; };

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void SetHideMouse(bool enabled) override;
	private:
		void Init(const WindowProperty& property);
		void Shutdown();
	private:
		GLFWwindow* m_Window = nullptr;
		Ref<RendererContext> m_Context;

		struct WindowDate
		{
			std::string title;
			uint32_t width, height;
			bool vsync;

			EventCallbackFn EventCallback;
		};
		WindowDate m_WindowDate;
	};
}



