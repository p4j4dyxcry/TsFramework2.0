#include "Pointer.h"
namespace TS
{
    void ReferenceCounter::AddObserver()
    {
		TS_LOCK(Mutex());
        ++(m_observerCount);
    }

    void ReferenceCounter::SubObserver()
    {
		TS_LOCK(Mutex());
        --(m_observerCount);
    }

    void ReferenceCounter::AddRef()
    {
		TS_LOCK(Mutex());
        ++(m_referenceCount);
        ++(m_observerCount);
    }

    void ReferenceCounter::SubRef()
    {
		TS_LOCK(Mutex());
        --(m_referenceCount);
        --(m_observerCount);
    }

    bool ReferenceCounter::IsRefZero() const
    {
        return m_referenceCount <= 0;
    }

    bool ReferenceCounter::Invalid() const
    {
        return m_observerCount <= 0;
    }
}
