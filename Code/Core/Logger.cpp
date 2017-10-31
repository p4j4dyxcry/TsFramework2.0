
#include "Logger.h"
#include <cstdarg>

#include "Code/Utility/DoubleOutStream.h"
#include "Memory/Pointer.h"


namespace TS
{
    namespace
    {
        Logger  g_DefaultLogger;
        SharedPtr<Logger> g_pUserLogger = nullptr;
    }

    void SetUserLogger(const SharedPtr<Logger> logger)
    {
        g_pUserLogger = logger;
    }

    Logger& GetLogger()
    {
        return g_pUserLogger != nullptr ? *g_pUserLogger : g_DefaultLogger;
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
        PreLog(metaData);

        std::ofstream debugLog("debug.log", std::ios::app);
        cout2(debugLog) << format;

        EndLog(metaData);
    }
}

