#pragma once
/// <summary>
/// 启动类放在引擎库中
/// </summary>
/// <returns>
/// 方便后期维护
/// </returns>
#ifdef GD_PLATFORM_WINDOWS

extern GodDecay::Application* GodDecay::CreateApplication();

int main() 
{
	GodDecay::Logger::Init();
	GD_ENGINE_INFO("日志启动成功!!");

	GD_Client_ERROR("客户端日志启动");

	GodDecay::Application* app = GodDecay::CreateApplication();

	app->Run();

	delete app;

	return 0;
}

#endif // GD_PLATFORM_WINDOWS

