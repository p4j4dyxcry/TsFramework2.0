#include "../Code/Core/INamed.h"
#include "../Code/Core/Develop.h"
#include "Code/Core/Memory/MemoryPool.h"
#include "Code/Core/Memory/MemorySystem.h"
#include "Code/Core/Memory/Pointer.h"
#include <list>
#include <Windows.h>

using namespace TS;

class ColorLogger : public Logger
{
private:
	void SetConsoleCollor(LogMetaData& metaData)
	{
		HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD attr = 0;
		attr |= FOREGROUND_INTENSITY;

		switch (metaData.logLevel)
		{
		case TS::LogLevel::Log_Error:	attr |= (FOREGROUND_RED); break;
		case TS::LogLevel::Log_Info:	attr |= (FOREGROUND_GREEN); break;
		case TS::LogLevel::Log_Warning:	attr |= (FOREGROUND_RED | FOREGROUND_GREEN); break;
		case TS::LogLevel::Log_Debug:	attr |= (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN); break;
		}

		SetConsoleTextAttribute(hCons, attr);
	}
public:


	void Log(LogMetaData& metaData, const char * format)override
	{
		SetConsoleCollor(metaData);
		printf(format);
		
		HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD attr = (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN) ;
		SetConsoleTextAttribute(hCons, attr);
		
	}

};

void UserLoggerTest()
{
	TS_LOG("◇ユーザ定義のロガーの動作確認テスト\n\n");
	auto logger = TS_NEW(ColorLogger)();

	TS_LOG("ユーザ定義のロガー未使用--\n");
	SetUserLogger(nullptr);
	TS_LOG("通常ログ\n");
	TS_LOG_ERROR("エラーログ\n");
	TS_LOG_WARNING("警告ログ\n");
	TS_LOG_DEBUG("デバッグログ\n");
	
	SetUserLogger(logger);
	TS_LOG("--ユーザロガーの使用を開始--\n");
	TS_LOG("通常ログ\n");
	TS_LOG_ERROR("エラーログ\n");
	TS_LOG_WARNING("警告ログ\n");
	TS_LOG_DEBUG("デバッグログ\n");
	
	TS_LOG("\n----------------------------------- \n");
}

void StaticMemoryPoolTest()
{
	TS_LOG("◇メモリプールからメモリを確保するテスト \n\n");
    StaticMemoryPool memory_pool(4096, 1024);

    std::vector<void*> mem(1024);

    for(int i=0;i<8 ; ++i)
    {
        unsigned memorySize = 1024 * ((rand()%31) + 1);
        mem.push_back( memory_pool.Alloc(memorySize));
        TS_LOG("メモリ確保 %d\n",memorySize);
    }
    
    TS_LOG(memory_pool.ToString());

    TS_LOG("メモリ解放\n");
    for (auto e : mem)
    {
        memory_pool.Free(e);
    }
    TS_LOG(memory_pool.ToString());
	TS_LOG("\n----------------------------------- \n");
}

void CustomAllocatorTest()
{
	TS_LOG("◇メモリアロケーションテスト \n\n");
	TS_LOG("デフォルトアロケータ -> \n");	
    auto start = clock();
    for (int i = 0; i<50000; ++i)
    {
        int * ptr = new int(i);

        delete ptr;
    }
    double res = (clock() - start) / (double)CLOCKS_PER_SEC;
    TS_LOG("%f(秒)\n", res);
	TS_LOG("カスタムアロケータ 　-> \n");
    start = clock();
    for (int i = 0; i<50000; ++i)
    {
        int * ptr = TS_NEW(int)(i);
        TS_DELETE(ptr);
    }

    res = (clock() - start) / (double)CLOCKS_PER_SEC;
	TS_LOG("%f(秒)\n", res);
	TS_LOG("\n----------------------------------- \n");

}

void SmartPointerTest()
{
	TS_LOG("◇スマートポインタ動作確認テスト \n\n");
	TS_LOG("メモリリークがなければ成功 \n");
	SharedPtr<int> sPtr = SharedPtr<int>(TS_NEW(int)());
	WeakPtr<int>   wPtr = sPtr;
	WeakPtr<int>   wPtr2 = wPtr;
	wPtr = wPtr2;
	UniquePtr<int> uPtr = (TS_NEW(int)());
	auto ptr = sPtr;
	(*ptr) = 5;
	TS_LOG("\n----------------------------------- \n");
}


void main()
{
    GetMemorySystem().EnableMemoryLeakCheck();
    GetMemorySystem().GetSystemDefaultAllocator();
    
 	UserLoggerTest();
	StaticMemoryPoolTest();
    CustomAllocatorTest();
	SmartPointerTest();

    GetMemorySystem().DumpLeak();

    while (true) {};

}
