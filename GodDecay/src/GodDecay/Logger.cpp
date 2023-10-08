#include "gdpch.h"

#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

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
