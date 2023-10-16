#include "gdpch.h"
#include "Window.h"

#include "GodDecay/Core/Base.h"

#ifdef GD_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace GodDecay
{
	Scope<Window> Window::Create(const WindowProperty& property)
	{
		#ifdef GD_PLATFORM_WINDOWS
				return CreateScope<WindowsWindow>(property);
		#else
				GD_ENGINE_ASSERT(false, "Unknown platform!");
				return nullptr;
		#endif
	}
}

