#include "Develop.h"
#include "Logger.h"
#include <iostream>
#include <cstdarg>


namespace TS
{

    namespace
    {
        Logger  g_DefaultLogger;
        Logger* g_pUserLogger = nullptr;
    }

    void SetUserLogger(Logger* logger)
    {
        g_pUserLogger = logger;
    }

    Logger& GetLogger()
    {
        return g_pUserLogger ? *g_pUserLogger : g_DefaultLogger;
    }

	const char* Logger::Format(const char * format, ...)
	{
		static char logBuffer[4096] = "\0";

		char* ap;
		va_start(ap, format);
		vsprintf_s(logBuffer, format, ap);
		return logBuffer;
	}

    void Logger::Log(LogMetaData& metaData, const char* format)
    {
        TS_UNUSED(metaData);		

		std::cout << format;
    }
}

