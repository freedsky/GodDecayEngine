#include "gdpch.h"
#include "WindowsWindow.h"

#include "GodDecay/Events/ApplicationEvent.h"
#include "GodDecay/Events/MouseEvent.h"
#include "GodDecay/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

namespace GodDecay 
{
	static bool s_GLFWInitialized = false;

	bool HideMouse = false;

	static void GLFWErrorCallback(int error, const char* description) 
	{
		GD_ENGINE_ERROR("({0}),{1}", errno, description);
	}

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
		m_Context->SwapBuffers();
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

	void WindowsWindow::SetHideMouse(bool enabled)
	{
		HideMouse = enabled;
		if (HideMouse)
		{
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
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

			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(property.Width, property.Height, property.Title.c_str(), nullptr, nullptr);
		GD_ENGINE_ASSERT(m_Window, "this window create failed! ");
		
		m_Context = CreateRef<OpenGLContext>(m_Window);
		m_Context->ContextInit();
		
		glfwSetWindowUserPointer(m_Window, &m_WindowDate);
		SetVSync(true);
		//callback
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
		{
			WindowDate& data = *(WindowDate*)glfwGetWindowUserPointer(window);

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
		{
			WindowDate& data = *(WindowDate*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallback(event);
		});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scanced, int action, int mods) 
		{
			WindowDate& data = *(WindowDate*)glfwGetWindowUserPointer(window);

			switch (action) 
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyRepetiedEvent event(key);
					data.EventCallback(event);
					break;
				}
			}
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode) 
		{
			WindowDate& data = *(WindowDate*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowDate& data = *(WindowDate*)glfwGetWindowUserPointer(window);

			switch (action) 
			{
			case GLFW_PRESS: 
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) 
		{
			WindowDate& data = *(WindowDate*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) 
		{
			WindowDate& data = *(WindowDate*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		if(m_Window != nullptr)
			glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}


