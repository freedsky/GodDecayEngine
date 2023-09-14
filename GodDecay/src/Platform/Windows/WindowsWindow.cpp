#include "gdpch.h"
#include "WindowsWindow.h"

namespace GodDecay 
{
	static bool s_GLFWInitialized = false;

	Ref<Window> Window::Create(const WindowProperty& property) 
	{
		return CreateRef<WindowsWindow>(property);
	}

	WindowsWindow::WindowsWindow(const WindowProperty& property)
	{
		Init(property);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpDate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_WindowDate.vsync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_WindowDate.vsync;
	}

	void WindowsWindow::Init(const WindowProperty& property)
	{
		m_WindowDate.title = property.Title;
		m_WindowDate.width = property.Width;
		m_WindowDate.height = property.Height;

		if(!s_GLFWInitialized)
		{
			int status = glfwInit();
			GD_ENGINE_ASSERT(status, "this glfw init failed! ");

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(property.Width, property.Height, property.Title.c_str(), nullptr, nullptr);
		GD_ENGINE_ASSERT(m_Window, "this window create failed! ");
		
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_WindowDate);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}


