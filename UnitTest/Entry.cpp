
#include "Code/Core/Types.h"
#include "../Code/Core/Develop.h"
#include "Code/Core/Memory/MemoryPool.h"
#include "Code/Core/Memory/MemorySystem.h"
#include "Code/Core/Memory/Pointer.h"
#include "Code/Utility\FileUtility.h"
#include "Code/Utility/Serialize.h"
#include "Code/Utility/StopWatch.h"
#include <Windows.h>
#include "Code/Utility/Window.h"
#include "Code\Core\Engine.h"
using namespace TS;

//! ログのエラーレベルによって色を付けてみるテストクラス
class ColorLogger : public Logger
{
private:
	void  PreLog(LogMetaData& metaData) override
	{
	    const HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
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
        const HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
        const WORD attr = (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
        SetConsoleTextAttribute(hCons, attr);
    }
public:

};


struct Pokemon 
{
	std::string name;
	int hp = 0;

	template <class Archive>
	void serialize(Archive & archive)
	{
		archive(CEREAL_NVP(hp), 
				CEREAL_NVP(name) );
	}
};
void SerializeTest()
{
	Pokemon pokemon;
	pokemon.name = "PIKACHU";
	pokemon.hp = 100;

	Pokemon pokemon2;
	pokemon2.name = "HITOKAGE";
	pokemon2.hp = 10;

	if(1)
	{
		std::ofstream ofs("pokemon.xml");
		cereal::XMLOutputArchive o_archive(ofs);
		o_archive(CEREAL_NVP(pokemon));
        Array<Pokemon> pokemons(30);
        o_archive(pokemons);

        Array<int> ints(3);
        o_archive(ints);

        Array<bool> bools(15);
        bools[5] = true;
        o_archive(bools);
	}

	if(1)
	{
		std::ifstream ifs("pokemon.xml");
		cereal::XMLInputArchive i_archive(ifs);

		Pokemon pokemon_i;
		Pokemon pokemon_2;

		i_archive(pokemon_i);
		//i_archive(pokemon_2);

		std::cout << pokemon_i.name << std::endl;
		std::cout << pokemon_i.hp << std::endl;
	}

}

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
    
    StopWatch stop_watch;
    stop_watch.Start();
    for (int i = 0; i<50000; ++i)
    {
        int * ptr = new int(i);

        delete ptr;
    }
    TS_LOG("4.2%f(ミリ秒)\n", stop_watch.ElpasedmSecond());
	TS_LOG("カスタムアロケータ 　-> \n");
    stop_watch.Start();
    for (int i = 0; i<50000; ++i)
    {
        int * ptr = TS_NEW(int)(i);
        TS_DELETE(ptr);
    }
    
	TS_LOG("4.2%f(ミリ秒)\n", stop_watch.ElpasedmSecond());
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
	SerializeTest();

	Window window;
	window.Create("window1", "sample");
	Window window2;
	window2.Create("window2", "2",128,128,window.GetHandle());

	window.Run();
	window2.Run();

	MSG msg;
	while (window.IsRuning())
	{
		if (GetMessage(&msg, nullptr, 0, 0) != -1)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (GetAsyncKeyState('A'))
			window.Close();
	}
	GetEngine()->Destory();
}
