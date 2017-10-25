#include "../Code/Core/INamed.h"
#include "../Code/Core/Develop.h"

using namespace TS;
#include <Windows.h>

class KingLogger : public Logger
{
public:
    void Log(LogMetaData& metaData, const char* format, ...) override
    {
        if( metaData.logLevel == Log_Error )
            MessageBox(nullptr,"test",nullptr,MB_OK);
        Logger::Log(metaData, format);
    }
};

void main()
{
    SetUserLogger(new KingLogger());
    INamed object;
    object.SetIName("who ?");

    TS_LOG_ERROR(object.ToString());
    while(true)
    {
        
    }
}
