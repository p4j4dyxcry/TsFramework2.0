#include "Engine.h"

namespace TS
{
    Engine::Engine()
    {
        m_pMemorySystem = new MemorySystem();
    }

    Engine* Engine::Instance()
    {
        static Engine engine;
        return &engine;
    }

    Engine::~Engine()
    {
        Destory();
    }

    void Engine::Destory()
    {
        m_pUserLogger.Release();

        if (m_pMemorySystem !=nullptr)
        {
            m_pMemorySystem->DumpLeak();
            delete m_pMemorySystem;
            m_pMemorySystem = nullptr;
        }
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
