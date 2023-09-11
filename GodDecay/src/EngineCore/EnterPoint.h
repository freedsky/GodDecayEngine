#pragma once

#ifdef GD_PLATFORM_WINDOWS

extern GodDecay::Application* GodDecay::CreateApplication();

int main() 
{
	GodDecay::Application* app = GodDecay::CreateApplication();

	app->Run();

	delete app;

	return 0;
}

#endif // GD_PLATFORM_WINDOWS

