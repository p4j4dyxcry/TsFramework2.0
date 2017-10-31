#pragma once

namespace TS
{
	class Engine : IMutex
	{
	private:
		TS_DISABLE_COPY(Engine);
		TS_DISABLE_MOVE(Engine);
		Engine() 
		{
			m_pMemorySystem = new MemorySystem();
		};
	public:
		static Engine* Instance()
		{
			static Engine engine;
			return &engine;
		}

		void Destory()
		{
			m_pUserLogger.Release();
			m_pMemorySystem->DumpLeak();
			delete m_pMemorySystem;
		}

		void SetLogger(SharedPtr<Logger> logger)
		{
			m_pUserLogger = logger;
		}
		SharedPtr<Logger> GetLogger()const
		{
			return m_pUserLogger;
		}

		MemorySystem& GetMemorySystem()
		{
			return *m_pMemorySystem;
		}

	private:
		MemorySystem*	  m_pMemorySystem;
		SharedPtr<Logger> m_pUserLogger;
	};

	Engine* GetEngine();
}