#define TS_ENGINE_METHOD_OVERRIDE
#include "Engine.h"

namespace TS
{
	namespace
	{
		Engine* g_engine = nullptr;
		bool	g_isCreated = false;
	}

    Engine::Engine()
    {
        m_pMemorySystem = new MemorySystem();
    }

    Engine* Engine::Instance()
    {        
        return g_engine;
    }

	Engine * Engine::Create(EngineSetting option)
	{
		if (g_engine == nullptr && g_isCreated == false)
		{
			g_isCreated = true;
			g_engine = new Engine();

			//! メモリ関連の初期化
			g_engine->GetMemorySystem().EnableMemoryLeakCheck();
			g_engine->GetMemorySystem().GetDefaultAllocator();

			//! メインウィンドウの作成
			g_engine->m_MainWindow = TS_NEW(Window)();

			g_engine->m_MainWindow->Create("_tsframework", 
										  option.windowTitle.c_str(),
										  option.windowWidth,
										  option.windowHeight);
			g_engine->m_MainWindow->Run();

			g_engine->OnInitialize();
		}
		return Instance();
	}

    Engine::~Engine()
    {		
		
    }

    void Engine::Destory()
    {
		auto engine = Instance();

		if (engine == nullptr)
			return;

		engine->OnFinalize();

		engine->m_MainWindow.Release();
		engine->m_pUserLogger.Release();
        if (engine->m_pMemorySystem !=nullptr)
        {
			engine->m_pMemorySystem->DumpLeak();
            delete engine->m_pMemorySystem;
			engine->m_pMemorySystem = nullptr;
        }
		delete g_engine;
		g_engine = nullptr;
    }

    void Engine::SetLogger(const SharedPtr<Logger>& logger)
    {
        m_pUserLogger = logger;
    }

	bool Engine::IsRuning()const
	{
		return m_MainWindow->IsRuning();
	}

	void Engine::UpdateEngine()
	{
		OnUpdate();
	}

    SharedPtr<Logger> Engine::GetLogger() const
    {
        return m_pUserLogger;
    }

    MemorySystem& Engine::GetMemorySystem() const
    {
        return *m_pMemorySystem;
    }

    Engine* GetEngine()
	{
		return Engine::Instance();
	}
}
