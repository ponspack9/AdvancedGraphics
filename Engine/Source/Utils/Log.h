#pragma once

#include <spdlog/spdlog.h>


// log macros
#define LOG_ERROR(...)		SPDLOG_LOGGER_ERROR(Log::GetLogger(),__VA_ARGS__)
#define LOG_WARN(...)		SPDLOG_LOGGER_WARN (Log::GetLogger(),__VA_ARGS__)
#define LOG_INFO(...)		SPDLOG_LOGGER_INFO (Log::GetLogger(),__VA_ARGS__)
#define LOG_CRITICAL(...)	SPDLOG_LOGGER_CRITICAL(Log::GetLogger(),__VA_ARGS__)
#define LOG_DEBUG(...)		SPDLOG_LOGGER_DEBUG(Log::GetLogger(),__VA_ARGS__)
#define LOG_TRACE(...)		SPDLOG_LOGGER_TRACE(Log::GetLogger(),__VA_ARGS__)

class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return spd_Logger; }

	static void OnGlError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

		LOG_ERROR("OpenGL debug message: {0}", message);

		switch (source)
		{
			case GL_DEBUG_SOURCE_API:				LOG_ERROR(" - source: GL_DEBUG_SOURCE_API"); break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		LOG_ERROR(" - source: GL_DEBUG_SOURCE_WINDOW_SYSTEM"); break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:	LOG_ERROR(" - source: GL_DEBUG_SOURCE_SHADER_COMPILER"); break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:		LOG_ERROR(" - source: GL_DEBUG_SOURCE_THIRD_PARTY"); break;
			case GL_DEBUG_SOURCE_APPLICATION:		LOG_ERROR(" - source: GL_DEBUG_SOURCE_APPLICATION"); break;
			case GL_DEBUG_SOURCE_OTHER:				LOG_ERROR(" - source: GL_DEBUG_SOURCE_OTHER"); break;
			default:								LOG_ERROR(" - source: UNKNOWN_ERROR"); break;
		}
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR:				LOG_ERROR(" - type: GL_DEBUG_SOURCE_API"); break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	LOG_ERROR(" - type: GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR"); break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	LOG_ERROR(" - type: GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR"); break;
			case GL_DEBUG_TYPE_PORTABILITY:			LOG_ERROR(" - type: GL_DEBUG_TYPE_PORTABILITY"); break;
			case GL_DEBUG_TYPE_PERFORMANCE:			LOG_ERROR(" - type: GL_DEBUG_TYPE_PERFORMANCE"); break;
			case GL_DEBUG_TYPE_MARKER:				LOG_ERROR(" - type: GL_DEBUG_TYPE_MARKER"); break;
			case GL_DEBUG_TYPE_PUSH_GROUP:			LOG_ERROR(" - type: GL_DEBUG_TYPE_PUSH_GROUP"); break;
			case GL_DEBUG_TYPE_POP_GROUP:			LOG_ERROR(" - type: GL_DEBUG_TYPE_POP_GROUP"); break;
			case GL_DEBUG_TYPE_OTHER:				LOG_ERROR(" - type: GL_DEBUG_TYPE_OTHER"); break;
			default:								LOG_ERROR(" - type: UNKNOWN_ERROR"); break;
		}
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:				LOG_ERROR(" - severity: GL_DEBUG_SEVERITY_HIGH"); break;
			case GL_DEBUG_SEVERITY_MEDIUM:				LOG_ERROR(" - severity: GL_DEBUG_SEVERITY_MEDIUM"); break;
			case GL_DEBUG_SEVERITY_LOW:					LOG_ERROR(" - severity: GL_DEBUG_SEVERITY_LOW"); break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:		LOG_ERROR(" - severity: GL_DEBUG_SEVERITY_NOTIFICATION"); break;
			default:									LOG_ERROR(" - severity: UNKNOWN_ERROR"); break;
		}

	}

private:
	static std::shared_ptr<spdlog::logger> spd_Logger;
};



