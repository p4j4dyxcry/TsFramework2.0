#include "../Code/Core/INamed.h"
#include "../Code/Core/Develop.h"
#include <list>
#include "Code/Core/Memory/MemoryPool.h"
#include "Code/Core/Memory/MemorySystem.h"
#include "Code/Core/Memory/Pointer.h"
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

SharedPtr<float> SmartPointerTest()
{
	//IPointer<int> iPtr = (TS_NEW(int)());
	SharedPtr<int> sPtr = (TS_NEW(int)());
	SharedPtr<int>wPtr = sPtr;
	//UniquePtr<int> uPtr = (TS_NEW(int)());
	//auto ptr = sPtr;
	//(*ptr) = 5;
	return SharedPtr<float>(TS_NEW(float)(1.0f));
}


void main()
{
    GetMemorySystem().EnableMemoryLeakCheck();

    GetMemorySystem().GetSystemDefaultAllocator();
    SetUserLogger(new KingLogger());
    INamed object;
    object.SetIName("who ?");

//    CustomAllocatorTest();
	auto ptr = SmartPointerTest();
	printf("%d", *ptr);

    GetMemorySystem().DumpLeak();

    while (true) {};

}
