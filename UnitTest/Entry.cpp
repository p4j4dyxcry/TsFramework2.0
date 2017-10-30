
#include "../Code/Core/Develop.h"
#include "Code/Core/Memory/MemoryPool.h"
#include "Code/Core/Memory/MemorySystem.h"
#include "Code/Core/Memory/Pointer.h"
#include "Code\Utility\File\FileUtility.h"
#include <Windows.h>

using namespace TS;

//! ログのエラーレベルによって色を付けてみるテストクラス
class ColorLogger : public Logger
{
private:
	void  PreLog(LogMetaData& metaData) override
	{
		HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD attr = 0;
		attr |= FOREGROUND_INTENSITY;

		switch (metaData.logLevel)
		{
		case Log_Error:	    attr |= (FOREGROUND_RED); break;
		case Log_Info:	    attr |= (FOREGROUND_GREEN); break;
		case Log_Warning:	attr |= (FOREGROUND_RED | FOREGROUND_GREEN); break;
		case Log_Debug: 	attr |= (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN); break;
		}

		SetConsoleTextAttribute(hCons, attr);
	}

    void  EndLog(LogMetaData& metaData)override
    {
        HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD attr = (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
        SetConsoleTextAttribute(hCons, attr);
    }
public:

};

void UserLoggerTest()
{
	TS_LOG("◇ユーザ定義のロガーの動作確認テスト\n\n");
	SharedPtr<Logger> logger = TS_NEW(ColorLogger)();

	TS_LOG("ユーザ定義のロガー未使用--\n");
	SetUserLogger( nullptr );
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

void FilePathTest()
{

	TS_LOG("◇ファイルテスト \n\n");
	{
		char path[_MAX_PATH];
		FileUtility::GetCD(path);
		char dirName[_MAX_PATH];
		FileUtility::GetName(dirName,path);

		TS_LOG("カレントディレクトリテスト -> \n%s\n", FileUtility::SetCD(path) ? "成功" : "失敗" );

		char dir[_MAX_PATH] = "test_dir";
		FileUtility::Combine(dirName, path, dir);
		TS_LOG("ディレクトリ　　 -> \n%s\n%s\n", dirName, FileUtility::Exist(dirName) ? "存在" : "存在しない");
		TS_LOG("ディレクトリ作成 -> \n%s\n%s\n", dirName, FileUtility::CreateDir(dirName) ? "成功" : "失敗");
		TS_LOG("ディレクトリ　　 -> \n%s\n%s\n", dirName, FileUtility::Exist(dirName) ? "存在" : "存在しない");
		TS_LOG("ディレクトリ削除 -> \n%s\n%s\n", dirName, FileUtility::Delete(dirName,true) ? "成功" : "失敗");
		TS_LOG("ディレクトリ　　 -> \n%s\n%s\n", dirName, FileUtility::Exist(dirName) ? "存在" : "存在しない");
	}
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
	FilePathTest();
//    GetMemorySystem().DumpLeak();

    while (true) {};

}
