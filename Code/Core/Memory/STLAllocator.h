#pragma once

#include "MemorySystem.h"

/**
* \brief STL用アロケータ(試験実装)
* \tparam T
*/
template <class T>
class STLAllocator : public Object
{

public:
    // 要素の型
    using value_type = T;
    STLAllocator() {}

    template <class U>
    STLAllocator(const STLAllocator<U>&) {}

    T* allocate(size_t n)
    {
        return reinterpret_cast<T*>(MemorySystem::Instance().GetDefaultAllocator()->Alloc(sizeof(T) * n));
    }

    void deallocate(T* p, std::size_t n)
    {
        MemorySystem::Instance().GetDefaultAllocator()->Free(p);
    }
};