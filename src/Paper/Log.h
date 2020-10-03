#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "Core.h"

namespace Paper
{
	class PAPER_API Log
	{
	public:
		static void Init();
		
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {return s_CoreLogger;};
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {return s_ClientLogger;};
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	}; 
}

// Core Log Macros
#define PAPER_CORE_ERROR(...)   Paper::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PAPER_CORE_WARN(...)    Paper::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PAPER_CORE_INFO(...)    Paper::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PAPER_CORE_TRACE(...)   Paper::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PAPER_CORE_FATAL(...)   Paper::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Log Macros
#define PAPER_ERROR(...)   Paper::Log::GetClientLogger()->error(__VA_ARGS__)
#define PAPER_WARN(...)    Paper::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PAPER_INFO(...)    Paper::Log::GetClientLogger()->info(__VA_ARGS__)
#define PAPER_TRACE(...)   Paper::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PAPER_FATAL(...)   Paper::Log::GetClientLogger()->fatal(__VA_ARGS__)