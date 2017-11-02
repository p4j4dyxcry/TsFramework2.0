#pragma once

#include "MemorySystem.h"

namespace TS
{
    /**
    * \brief STL用アロケータ(試験実装)
    * \tparam T
    */
    template <class T>
    class STLAllocator : public std::allocator<T>
    {

    public:
        STLAllocator() { }

        STLAllocator(const STLAllocator& x) { }

        template<class U>
        STLAllocator(const STLAllocator<U>& x) { }


        T* allocate(size_t n)
        {
            auto& memorySystem = GetMemorySystem();

            IAllocator* pAllocator = memorySystem.GetSystemDefaultAllocator();

            const size_t memorySize = sizeof(T) + sizeof(AllockHeaderBlock);

            auto pMemory = static_cast<char*>(pAllocator->Alloc(memorySize));

            AllockHeaderBlock* block = new(pMemory)AllockHeaderBlock;

            block->objectSize = sizeof(T);
            block->arrayCount = 1;

            pMemory += sizeof(AllockHeaderBlock);
            T* pObject = reinterpret_cast<T*>(pMemory);


            if (memorySystem.IsEnableMemoryLeak())
            {
                MemoryMetaData memory_meta;
                memory_meta.line = 0;
                memory_meta.fileName = "該当なし";
                memory_meta.functionName = "STLのコンテナから確保されました。";

                memory_meta.pAllocator = pAllocator;
                memory_meta.memorySize = memorySize;
                memory_meta.pointer = pObject;
                memory_meta.typeData = typeid(T).name();
                memorySystem.RegisterMemoryMetaData(memory_meta);
            }
            return pObject;
        }

        void deallocate(T* p, std::size_t n)
        {
            auto& memorySystem = GetMemorySystem();

            IAllocator* pAllocator = memorySystem.FindAllocator(p);
            AllockHeaderBlock* meta = reinterpret_cast<AllockHeaderBlock*>(p);
            meta--;
            void * pMemoryHead = meta;

            if (memorySystem.IsEnableMemoryLeak())
                memorySystem.RemoveMemoryMetaData(p);

            pAllocator->Free(pMemoryHead);
        }

        template<class U>
        struct rebind { typedef STLAllocator<U> other; };
    };
}

