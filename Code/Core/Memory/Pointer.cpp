#include "Pointer.h"
namespace TS
{
    void ReferenceCounter::Lock()
    {
        std::lock_guard<decltype(m_mutex)> lock(m_mutex);
    }

    void ReferenceCounter::AddObserver()
    {
        Lock();
        ++(m_observerCount);
    }

    void ReferenceCounter::SubObserver()
    {
        Lock();
        --(m_observerCount);
    }

    void ReferenceCounter::AddRef()
    {
        Lock();
        ++(m_referenceCount);
        ++(m_observerCount);
    }

    void ReferenceCounter::SubRef()
    {
        Lock();
        --(m_referenceCount);
        --(m_observerCount);
    }

    bool ReferenceCounter::IsRemovePointer() const
    {
        return m_referenceCount <= 0;
    }

    bool ReferenceCounter::Invalid() const
    {
        return m_observerCount <= 0;
    }
}
