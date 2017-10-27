#include "Allocator.h"
namespace TS
{
    StaticMemoryPool * TSAllocator::s_MinPool     = new StaticMemoryPool(8  , 4096 * 2048);
    StaticMemoryPool * TSAllocator::s_DefaultPool = new StaticMemoryPool(128, 4096 * 1024);
    StaticMemoryPool * TSAllocator::s_LargePool   = new StaticMemoryPool(256, 4096 );

    void* TSAllocator::Alloc(size_t size)
    {
        void * ptr = nullptr;
        if (size <= s_MinPool->GetChunkSize())
        {
            if ( (ptr = s_MinPool->Alloc(size) ) != nullptr )
                return ptr;            
        }

        if (size <= s_DefaultPool->GetChunkSize())
        {
            if ((ptr = s_DefaultPool->Alloc(size)) != nullptr)
                return ptr;
        }
        
        if (ptr = s_LargePool->Alloc(size))
            return ptr;

        return nullptr;
    }

    bool TSAllocator::Free(void* ptr)
    {
        if (s_MinPool->From(ptr))     return s_MinPool->Free(ptr);
        if (s_DefaultPool->From(ptr)) return s_DefaultPool->Free(ptr);
        if (s_LargePool->From(ptr))   return s_LargePool->Free(ptr);

        return false;
    }
}
