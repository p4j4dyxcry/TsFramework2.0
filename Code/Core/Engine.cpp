#define TS_ENGINE_METHOD_OVERRIDE
#include "Engine.h"
#include "Code/Gfx/GfxCore.h"
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
                                                   
	{
	    
	}

    EngineSetting::TimeScaleSetting::TimeScaleSetting():Framerate(60)
    {

    }

    EngineSetting::EngineSetting(){}

    Engine::Engine()
    {
        m_pMemorySystem = new MemorySystem();
    }

    bool Engine::Initialize(EngineSetting& option)
    {
        TS_DUMP_CURRENT_TIME("エンジン初期化");
        //! メモリ関連の初期化
        
        GetMemorySystem().EnableMemoryLeakCheck();
        GetMemorySystem().GetDefaultAllocator();

        //! メインウィンドウの作成
        m_pMainWindow = TS_NEW(Window)();
        m_pMainWindow->Create("_tsframework",
            option.Window.Title.c_str(),
            option.Window.Width,
            option.Window.Height);
        m_pMainWindow->Run();

        m_pTimeSystem = TS_NEW(TimeSystem)(option.Time.Framerate);
        m_pGfxCore    = TS_NEW(GfxCore)(m_pMainWindow->GetHandle());
        
        return true;
    }

    void Engine::Destroy()
    {
        m_pMainWindow.Release();
        m_pUserLogger.Release();
        if (m_pMemorySystem != nullptr)
        {
            m_pMemorySystem->DumpLeak();
            delete m_pMemorySystem;
            m_pMemorySystem = nullptr;
        }
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
            g_engine->Initialize(option);			
;		}        
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

		delete g_engine;
		g_engine = nullptr;
    }

    void Engine::SetLogger(const SharedPtr<Logger>& logger)
    {
        m_pUserLogger = logger;
    }

	bool Engine::IsRuning()const
	{
		return m_pMainWindow->IsRuning();
	}

	void Engine::UpdateEngine()
	{
        m_pTimeSystem->BeginFrame();
        Window::ProsessMessage();
	}

    SharedPtr<Logger> Engine::GetLogger() const
    {
        return m_pUserLogger;
    }

    MemorySystem& Engine::GetMemorySystem() const
    {
        return *m_pMemorySystem;
    }

    SharedPtr<TimeSystem>& Engine::GetTimeSystem() 
    {
        return m_pTimeSystem;
    }

    void Engine::EndFrame()
    {
        m_pTimeSystem->WaitRemaining();
        m_pTimeSystem->EndFrame();
    }

    SharedPtr<GfxCore>& Engine::GetGfxSystem()
    {
        return m_pGfxCore;
    }

    const SharedPtr<GfxCore>& Engine::GetGfxSystem() const
    {
        return m_pGfxCore;
    }

    Engine* GetEngine()
	{
		return Engine::Instance();
	}
}
