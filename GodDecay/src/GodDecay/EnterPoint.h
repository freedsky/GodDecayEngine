#pragma once
/// <summary>
/// ����������������
/// </summary>
/// <returns>
/// �������ά��
/// </returns>
#ifdef GD_PLATFORM_WINDOWS

extern GodDecay::Application* GodDecay::CreateApplication();

int main() 
{
	GodDecay::Logger::Init();
	GD_ENGINE_INFO("��־�����ɹ�!!");

	GD_Client_ERROR("�ͻ�����־����");

	GodDecay::Application* app = GodDecay::CreateApplication();

	app->Run();

	delete app;

	return 0;
}

#endif // GD_PLATFORM_WINDOWS

