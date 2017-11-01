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
		if (g_engine == nullptr && g_isCreated == false)
		{
			g_isCreated = true;
			g_engine = new Engine();
		}
        return g_engine;
    }

    Engine::~Engine()
    {		
		
    }

    void Engine::Destory()
    {
		auto engine = Instance();

		if (engine == nullptr)
			return;
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
