#pragma once
#include "MemoryPool.h"

namespace TS
{
    class TSAllocator
    {
    private:
       static StaticMemoryPool* s_MinPool;
       static StaticMemoryPool* s_DefaultPool;
       static StaticMemoryPool* s_LargePool;
    public:
        static void* Alloc(size_t size);
        static bool Free(void* ptr);
    };

    template <class T>
    class STLAllocator
    {

    public:
        // 要素の型
        using value_type = T;
        STLAllocator() {}

        template <class U>
        STLAllocator(const STLAllocator<U>&) {}

        T* allocate(size_t n)
        {
            //return reinterpret_cast<T*>(malloc(sizeof(T) * n));
            return reinterpret_cast<T*>(TSAllocator::Alloc(sizeof(T) * n));
        }

        void deallocate(T* p, std::size_t n)
        {
            //free(p);
            TSAllocator::Free(p);
        }

    };
}

