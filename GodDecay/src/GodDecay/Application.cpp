#include "gdpch.h"

#include "Application.h"

#include "Events/KeyEvent.h"

#include "Logger.h"

#include <GLFW/glfw3.h>

namespace GodDecay 
{
	Application::Application()
	{
		window = Window::Create();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			window->OnUpDate();
		}
	}
}
