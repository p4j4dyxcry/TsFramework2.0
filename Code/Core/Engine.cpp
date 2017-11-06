#define TS_ENGINE_METHOD_OVERRIDE
#include "Engine.h"

namespace TS
{
	namespace
	{
		Engine* g_engine = nullptr;
		bool	g_isCreated = false;
	}

    EngineSetting::WindowSetting::WindowSetting()  : Title("TsFramework")
                                                   , Width(1280)
                                                   , Height(768)
    {}

    EngineSetting::EngineSetting(){}

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
										  option.Window.Title.c_str(),
										  option.Window.Width,
										  option.Window.Height);
			g_engine->m_MainWindow->Run();

            g_engine->m_StopWatch.Start();
            //日付のDump
			{
                time_t t = time(nullptr);

                struct tm lt;
                localtime_s(&lt, &t);

                std::stringstream s;
                s << "20" << lt.tm_year - 100 << "/" << lt.tm_mon + 1 << "/" << lt.tm_mday << " ";
                s << lt.tm_hour << "時" << lt.tm_min << "分" << lt.tm_sec << "秒";
                TS_LOG("//!---------------------------------------------\n");
                TS_LOG("//! エンジン初期化 ::%s\n", s.str().c_str());
                TS_LOG("//!---------------------------------------------\n");
			}

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
        m_StopWatch.Recode();
        Window::ProsessMessage();

        double delta = m_StopWatch.GetLastRecodeDelta();

        //! 60フレームに合わせる
	    while(delta <= OneFrameBy60Fps - FLT_EPSILON )
	    {
            delta = m_StopWatch.GetLastRecodeDelta();
	    }

	    if(m_StopWatch.Elpased() >= 1.0)
        {
            TS_LOG("fps = %2.2lf \n", 1.0f / m_StopWatch.GetAvgRecodeIntarval() );
            m_StopWatch.Start();
        }
	}

    SharedPtr<Logger> Engine::GetLogger() const
    {
        return m_pUserLogger;
    }

    MemorySystem& Engine::GetMemorySystem() const
    {
        return *m_pMemorySystem;
    }

    double Engine::GetDeltaTime() const
    {
        return m_StopWatch.GetPrevDelta();
    }

    Engine* GetEngine()
	{
		return Engine::Instance();
	}
}
