#include "../Code/Core/INamed.h"
#include "../Code/Core/Develop.h"
#include <list>
#include "Code/Core/Memory/MemoryPool.h"
#include "Code/Core/Memory/MemorySystem.h"

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
    }
    
    TS_LOG_DEBUG(memory_pool.ToString());

    TS_LOG_DEBUG("メモリ解放\n");
    for (auto e : mem)
    {
        memory_pool.Free(e);
    }
    TS_LOG(memory_pool.ToString());
}

void CustomAllocatorTest()
{

    auto start = clock();
    for (int i = 0; i<50000; ++i)
    {
        int * ptr = new int(i);

        delete ptr;
    }
    double res = (clock() - start) / (double)CLOCKS_PER_SEC;
    TS_LOG("デフォルトアロケータ %f\n", res);

    start = clock();
    for (int i = 0; i<50000; ++i)
    {
        int * ptr = TS_NEW(int)(i);
        TS_DELETE(ptr);
    }

    res = (clock() - start) / (double)CLOCKS_PER_SEC;
    TS_LOG("カスタムアロケータ %f\n", res);
}



void main()
{
    GetMemorySystem().EnableMemoryLeakCheck();

    GetMemorySystem().GetSystemDefaultAllocator();
    SetUserLogger(new KingLogger());
    INamed object;
    object.SetIName("who ?");

    CustomAllocatorTest();
    

    GetMemorySystem().DumpLeak();

    while (true) {};

}
