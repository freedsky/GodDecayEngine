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
	GodDecay::Application* app = GodDecay::CreateApplication();

	app->Run();

	delete app;

	return 0;
}

