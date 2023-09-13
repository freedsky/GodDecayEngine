#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "Core.h"
/// <summary>
/// ��װspdlog�⣬ʹ�ú궨��򻯿������ù���
/// 
/// �����ᶨ���ļ����빦��
/// </summary>
namespace GodDecay 
{
	class GODDECAY_API Logger
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger> GetEngineLogger() { return m_EngineLogger; }
		inline static Ref<spdlog::logger> GetClientLogger() { return m_ClientLogger; }
	private:
		static Ref<spdlog::logger> m_EngineLogger;
		static Ref<spdlog::logger> m_ClientLogger;
	};
}

//���ú궨��򻯵��ù���
#define GD_ENGINE_TRACE(...)	::GodDecay::Logger::GetEngineLogger()->trace(__VA_ARGS__);
#define GD_ENGINE_INFO(...)		::GodDecay::Logger::GetEngineLogger()->info(__VA_ARGS__);
#define GD_ENGINE_WARN(...)		::GodDecay::Logger::GetEngineLogger()->warn(__VA_ARGS__);
#define GD_ENGINE_ERROR(...)	::GodDecay::Logger::GetEngineLogger()->error(__VA_ARGS__);
#define GD_ENGINE_DEBUG(...)	::GodDecay::Logger::GetEngineLogger()->debug(__VA_ARGS__);
#define GD_ENGINE_CRITICAL(...) ::GodDecay::Logger::GetEngineLogger()->critical(__VA_ARGS__);

#define GD_Client_TRACE(...)	::GodDecay::Logger::GetClientLogger()->trace(__VA_ARGS__);
#define GD_Client_INFO(...)		::GodDecay::Logger::GetClientLogger()->info(__VA_ARGS__);
#define GD_Client_WARN(...)		::GodDecay::Logger::GetClientLogger()->warn(__VA_ARGS__);
#define GD_Client_ERROR(...)	::GodDecay::Logger::GetClientLogger()->error(__VA_ARGS__);
#define GD_Client_DEBUG(...)	::GodDecay::Logger::GetClientLogger()->debug(__VA_ARGS__);
#define GD_Client_CRITICAL(...) ::GodDecay::Logger::GetClientLogger()->critical(__VA_ARGS__);