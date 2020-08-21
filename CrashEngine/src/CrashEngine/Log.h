#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace CrashEngine {
	class CRASH_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//core log macros
#define CE_CORE_ERROR(...)	::CrashEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CE_CORE_WARN(...)	::CrashEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CE_CORE_INFO(...)	::CrashEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CE_CORE_TRACE(...)	::CrashEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CE_CORE_FATAL(...)	::CrashEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

//clent log macros
#define CE_ERROR(...)	::CrashEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define CE_WARN(...)	::CrashEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CE_INFO(...)	::CrashEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define CE_TRACE(...)	::CrashEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CE_FATAL(...)	::CrashEngine::Log::GetClientLogger()->critical(__VA_ARGS__)