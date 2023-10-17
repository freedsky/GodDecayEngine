#pragma once
/// <summary>
/// ����������������
/// </summary>
/// <returns>
/// �������ά��
/// </returns>

extern GodDecay::Application* GodDecay::CreateApplication();

int main() 
{
	GodDecay::Logger::Init();
	GD_ENGINE_INFO("��־�����ɹ�!!");

	GD_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
	GodDecay::Application* app = GodDecay::CreateApplication();
	GD_PROFILE_END_SESSION();

	GD_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
	app->Run();
	GD_PROFILE_END_SESSION();

	GD_PROFILE_BEGIN_SESSION("EndShut", "HazelProfile-Shutdown.json");
	delete app;
	GD_PROFILE_END_SESSION();

	return 0;
}


