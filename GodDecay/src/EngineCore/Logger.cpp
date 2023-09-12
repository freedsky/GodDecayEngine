#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

/// <summary>
/// 包装spdlog库，使用宏定义简化开发调用过程
/// 
/// 后续会定义文件出入功能
/// </summary>
namespace GodDecay 
{
	Ref<spdlog::logger> Logger::m_EngineLogger = nullptr;
	Ref<spdlog::logger> Logger::m_ClientLogger = nullptr;

	void Logger::Init() 
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		m_EngineLogger = spdlog::stdout_color_mt("Engine");
		m_EngineLogger->set_level(spdlog::level::trace);

		m_ClientLogger = spdlog::stdout_color_mt("Application");
		m_ClientLogger->set_level(spdlog::level::trace);
	}
}
