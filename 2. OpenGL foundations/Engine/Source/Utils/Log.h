#pragma once

#include <spdlog/spdlog.h>


class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return spd_Logger; }

private:
	static std::shared_ptr<spdlog::logger> spd_Logger;
};


// log macros
#define LOG_ERROR(...)		SPDLOG_LOGGER_ERROR(Log::GetLogger(),__VA_ARGS__)
#define LOG_WARN(...)		SPDLOG_LOGGER_WARN (Log::GetLogger(),__VA_ARGS__)
#define LOG_INFO(...)		SPDLOG_LOGGER_INFO (Log::GetLogger(),__VA_ARGS__)
#define LOG_CRITICAL(...)	SPDLOG_LOGGER_CRITICAL(Log::GetLogger(),__VA_ARGS__)
#define LOG_DEBUG(...)		SPDLOG_LOGGER_DEBUG(Log::GetLogger(),__VA_ARGS__)
#define LOG_TRACE(...)		SPDLOG_LOGGER_TRACE(Log::GetLogger(),__VA_ARGS__)
