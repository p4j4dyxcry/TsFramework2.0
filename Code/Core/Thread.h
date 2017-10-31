#pragma once

namespace TS
{
	class IMutex : public Object
	{
	protected:
		std::mutex m_mutex;
		
		std::mutex& Mutex() 
		{ 
			return m_mutex; 
		}
	};
#define TS_LOCK(lockObject) std::lock_guard<decltype(lockObject)> __lock__(lockObject)

}
