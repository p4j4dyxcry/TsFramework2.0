#include "Develop.h"
#include "Logger.h"
#include <stdio.h>
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

    void Logger::Log(LogMetaData& metaData, const char* format, ...)
    {
        TS_UNUSED(metaData);

        va_list ap;

        va_start(ap, format);
        vprintf(format, ap);
        va_end(ap);
    }
}

