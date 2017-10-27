#pragma once
#include "MemoryPool.h"

namespace TS
{

    class IAllocator : public Object
    {
    public:
        virtual void* Alloc(size_t size) = 0;
        virtual bool Free(void* ptr) = 0;
    };
}

