#include "../Code/Core/INamed.h"
#include "../Code/Core/Develop.h"
#include "Code/Core/MemoryPool.h"
#include "Code/Core/Allocator.h"
#include <list>

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
    std::list<int> data{ 5,5,5 };

    auto start = clock();
    for (int i = 0; i<5000000; ++i)
    {
        data.push_back(i);
    }
    double res = (clock() - start) / (double)CLOCKS_PER_SEC;
    TS_LOG("デフォルトアロケータ %f\n", res);

    std::list<int, STLAllocator<int>> data2{ 5,5,5 };
    

    start = clock();
    for (int i = 0; i<5000000; ++i)
    {
        data2.push_back(i);
    }


    res = (clock() - start) / (double)CLOCKS_PER_SEC;
    TS_LOG("カスタムアロケータ %f\n", res);
}

void main()
{
    SetUserLogger(new KingLogger());
    INamed object;
    object.SetIName("who ?");

    //StaticMemoryPoolTest();
    CustomAllocatorTest();
    while(true)
    {
        
    }
}
