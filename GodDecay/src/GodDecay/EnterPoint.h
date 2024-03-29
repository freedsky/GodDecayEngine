#pragma once
/// <summary>
/// 启动类放在引擎库中
/// </summary>
/// <returns>
/// 方便后期维护
/// </returns>

extern GodDecay::Application* GodDecay::CreateApplication();

int main() 
{
	GodDecay::Logger::Init();
	GD_ENGINE_INFO("日志启动成功!!");
	GodDecay::Application* app = GodDecay::CreateApplication();

	app->Run();

	delete app;

	return 0;
}


