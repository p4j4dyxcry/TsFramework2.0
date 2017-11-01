#pragma once

#include "Code/Core/Object.h"

//! 関数を別スレッドからアクセスできない用にロックします。
#define TS_LOCK(lockObject) std::lock_guard<decltype(lockObject)> __lock__(lockObject)

namespace TS
{
    /**
	 * \brief ミューテクスを持つインタフェース
	 */
	class IMutex : public Object
	{
	protected:
		std::mutex m_mutex;
		
		std::mutex& Mutex() 
		{ 
			return m_mutex; 
		}
	};
}
