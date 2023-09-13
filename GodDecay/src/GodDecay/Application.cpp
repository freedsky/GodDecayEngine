#include "gdpch.h"

#include "Application.h"

#include "Events/KeyEvent.h"

#include "Logger.h"

namespace GodDecay 
{
	Application::Application()
	{

	}

	void Application::Run()
	{
		//===========test==========

		KeyPressedEvent e(0, 1);
		if (e.GetEventType() == KeyPressedEvent::GetStaticType()) 
		{
			GD_ENGINE_INFO(e);
		}

		//=========================

		while (true)
		{

		}
	}
}
