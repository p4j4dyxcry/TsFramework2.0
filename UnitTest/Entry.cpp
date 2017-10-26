#include "../Code/Core/INamed.h"
#include "../Code/Core/Develop.h"
#include "Code/Core/MemoryPool.h"

using namespace TS;
#include <Windows.h>

class KingLogger : public Logger
{
public:

};

void StaticMemoryPoolTest()
{
    TS_LOG_DEBUG("StaticMemoryPool のテスト\n");
    StaticMemoryPool memory_pool(4096, 1024);

    std::vector<void*> mem(1024);

    for(int i=0;i<32 ; ++i)
    {
        unsigned memorySize = 1024 * ((rand()%31) + 1);
        mem.push_back( memory_pool.Alloc(memorySize));
        TS_LOG_DEBUG("メモリ確保 %d\n",memorySize);
        Sleep(100);
    }
    
    TS_LOG_DEBUG(memory_pool.ToString());

    TS_LOG_DEBUG("メモリ解放\n");
    for (auto e : mem)
    {
        memory_pool.Free(e);
    }
    TS_LOG_DEBUG(memory_pool.ToString());
}

void main()
{
    SetUserLogger(new KingLogger());
    INamed object;
    object.SetIName("who ?");
    
    StaticMemoryPoolTest();
    
    while(true)
    {
        
    }
}
