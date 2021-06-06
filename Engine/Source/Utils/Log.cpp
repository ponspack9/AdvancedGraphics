#include "pch.h"

#include <spdlog/sinks/stdout_color_sinks.h>


std::shared_ptr<spdlog::logger> Log::spd_Logger;

void Log::Init()
{
	spdlog::set_pattern("{%T} %n %^[%l]\t %s:%#:%$  %v");

	spd_Logger = spdlog::stdout_color_mt("OPG");
	spd_Logger->set_level(spdlog::level::trace);
}

