#include <Windows.h>

#include "Code/Core/Types.h"
#include "../Code/Core/Develop.h"
#include "Code/Core/Memory/MemoryPool.h"
#include "Code/Core/Memory/MemorySystem.h"
#include "Code/Core/Memory/Pointer.h"
#include "Code/Utility\FileUtility.h"
#include "Code/Utility/Serialize.h"
#include "Code/Utility/StopWatch.h"

using namespace TS;

//-----------------------------------------------------------------
//! ロガーテスト
//-----------------------------------------------------------------

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

const char* g_test_name;
void BeginTest(const char* testname)
{
    g_test_name = testname;
    TS_LOG_ERROR("◇:%sテスト\n\n",g_test_name);
}

void EndTest()
{
    TS_LOG_ERROR("---%sテスト終了\n", g_test_name);
}


void UserLoggerTest()
{
    BeginTest("ユーザロガー");
    SharedPtr<Logger> logger = TS_NEW(ColorLogger)();

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

    EndTest();
}

//-----------------------------------------------------------------
//! シリアライズテスト
//-----------------------------------------------------------------
struct Pokemon
{
	TS::TsString name;
	int hp = 0;

	template <class Archive>
	void serialize(Archive & archive)
	{
		archive(CEREAL_NVP(hp),
			CEREAL_NVP(name));
	}
};
void SerializeTest()
{
    BeginTest("シリアライズ");
	Pokemon pokemon;
	pokemon.name = "PIKACHU";
	pokemon.hp = 100;

	Pokemon pokemon2;
	pokemon2.name = "HITOKAGE";
	pokemon2.hp = 10;

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
    EndTest();
}


//-----------------------------------------------------------------
//! メモリプールテスト
//-----------------------------------------------------------------
void StaticMemoryPoolTest()
{
    BeginTest("メモリプール");
	StaticMemoryPool memory_pool(4096, 1024);

	std::vector<void*> mem(1024);

	for (int i = 0; i<8; ++i)
	{
		unsigned memorySize = 1024 * ((rand() % 31) + 1);
		mem.push_back(memory_pool.Alloc(memorySize));
		TS_LOG("メモリ確保 %d\n", memorySize);
	}

	TS_LOG(memory_pool.ToString());

	TS_LOG("メモリ解放\n");
	for (auto e : mem)
	{
		memory_pool.Free(e);
	}
	TS_LOG(memory_pool.ToString());
    EndTest();
}

//-----------------------------------------------------------------
//! アロケーションテスト
//-----------------------------------------------------------------
void AllocatorTest()
{
	StopWatch stop_watch;

    BeginTest("メモリアロケーション");

	TS_LOG("デフォルトアロケータ -> \n");
	stop_watch.Start();
	for (int i = 0; i<50000; ++i)
	{
		int * ptr = new int(i);
		delete ptr;
	}
	TS_LOG("%4.6f(ミリ秒)\n", stop_watch.Elpased() * 1000.0);

	TS_LOG("カスタムアロケータ 　-> \n");
	stop_watch.Start();
	for (int i = 0; i<50000; ++i)
	{
		int * ptr = TS_NEW(int)(i);
		TS_DELETE(ptr);
	}
	TS_LOG("%4.6f(ミリ秒)\n", stop_watch.Elpased() * 1000.0);

    EndTest();
}

//-----------------------------------------------------------------
//! スマートポインタテスト
//-----------------------------------------------------------------
void SmartPointerTest()
{
    BeginTest("スマートポインタ");
	TS_LOG("メモリリークがなければ成功 \n");
	SharedPtr<int> sPtr = SharedPtr<int>(TS_NEW(int)());
	WeakPtr<int>   wPtr = sPtr;
	WeakPtr<int>   wPtr2 = wPtr;
	wPtr = wPtr2;
	UniquePtr<int> uPtr = (TS_NEW(int)());
	auto ptr = sPtr;
	(*ptr) = 5;
	
    EndTest();
}

//-----------------------------------------------------------------
//! ファイル作成テスト
//-----------------------------------------------------------------
void FilePathTest()
{
    BeginTest("ファイル");
	{
		char path[_MAX_PATH];
		FileUtility::GetCD(path);
		char dirName[_MAX_PATH];
		FileUtility::GetName(dirName, path);

		TS_LOG("カレントディレクトリテスト -> \n%s\n", FileUtility::SetCD(path) ? "成功" : "失敗");

		char dir[_MAX_PATH] = "test_dir";
		FileUtility::Combine(dirName, path, dir);
		TS_LOG("ディレクトリ　　 -> \n%s\n%s\n", dirName, FileUtility::Exist(dirName) ? "存在" : "存在しない");
		TS_LOG("ディレクトリ作成 -> \n%s\n%s\n", dirName, FileUtility::CreateDir(dirName) ? "成功" : "失敗");
		TS_LOG("ディレクトリ　　 -> \n%s\n%s\n", dirName, FileUtility::Exist(dirName) ? "存在" : "存在しない");
		TS_LOG("ディレクトリ削除 -> \n%s\n%s\n", dirName, FileUtility::Delete(dirName, true) ? "成功" : "失敗");
		TS_LOG("ディレクトリ　　 -> \n%s\n%s\n", dirName, FileUtility::Exist(dirName) ? "存在" : "存在しない");
	}
    EndTest();
}

template <typename T>
int RandomTestFunc(T _min , T _max )
{

    bool isFloat = typeid(T).name()[0] == 'f' || typeid(T).name()[0] == 'd';

    const int numTest = 20000;
    const int representative[] = { 0,1987,6921,10250,19873 };

    T big = max(_min,_max);
    T samll = min(_min, _max);

    TS_LOG("--%s型\n",typeid(T).name());
    if(isFloat)
        TS_LOG("\t範囲 %.6f ～ %.6f \n", (_min), (_max))
    else
        TS_LOG("\t範囲 %d ～ %d \n", (_min), (_max))
    for (int i = 0; i<numTest; ++i)
    {
        auto val = Random<T>::Global(_min, _max);
        if (val < samll || val > big)
        {
            TS_LOG_ERROR("\t テスト失敗\n");
            if (isFloat)
                TS_LOG_ERROR("\t値 %.6f \n", (val))
            else
                TS_LOG_ERROR("\t値 %d \n", (val))
            return -1;
        }
        for (int j = 0; j < _ARRAYSIZE(representative); ++j)
            if (i == representative[j])
                if(isFloat)
                    TS_LOG("\t代表値[%d] %.6f \n", j,(val))
                else
                    TS_LOG("\t代表値[%d] %d \n", j, (val))
    }
    TS_LOG("\t テスト成功\n");
    return 0;
}

void RandomTest()
{
    BeginTest("乱数");
    {
        RandomTestFunc<char>(-38, 62);
        RandomTestFunc<unsigned char>(12, 126);

        RandomTestFunc<short>(-31500, -3800);
        RandomTestFunc<unsigned short>(65535, 0);

        RandomTestFunc<int>(-90000, 1500000);
        RandomTestFunc<unsigned int>(1, 5);

        RandomTestFunc<float>(-0.1f, 0.0f);
        RandomTestFunc<double>(-124.51f, -9999.52f);        
    }
    EndTest();
}

//-----------------------------------------------------------------
//! テストを走らせる
//-----------------------------------------------------------------
void RunTests()
{
	GetMemorySystem().EnableMemoryLeakCheck();
	GetMemorySystem().GetSystemDefaultAllocator();

	UserLoggerTest();    
    RandomTest();
	StaticMemoryPoolTest();
	AllocatorTest();
	SmartPointerTest();
	FilePathTest();
	SerializeTest();
}