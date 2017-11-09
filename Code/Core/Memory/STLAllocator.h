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

            const size_t memorySize = sizeof(T) + sizeof(MemoryMetaData);

            auto pMemory = static_cast<char*>(pAllocator->Alloc(memorySize));

			MemoryMetaData* block = new(pMemory)MemoryMetaData;

            block->objectSize = sizeof(T);
            block->arrayCount = 1;
			block->pPrevBlock = nullptr;
			block->pNextBlock = nullptr;

            pMemory += sizeof(MemoryMetaData);
            T* pObject = reinterpret_cast<T*>(pMemory);


            if (memorySystem.IsEnableMemoryLeak())
            {
                block->line = 0;
                block->fileName = "該当なし";
                block->functionName = "STLのコンテナから確保されました。";
                block->pAllocator = pAllocator;

				block->pointer = pObject;
                block->typeData = typeid(T).name();
                memorySystem.RegisterMemoryMetaData(block);
            }
            return pObject;
        }

        void deallocate(T* p, std::size_t n)
        {
            auto& memorySystem = GetMemorySystem();

            IAllocator* pAllocator = memorySystem.FindAllocator(p);
			MemoryMetaData* meta = reinterpret_cast<MemoryMetaData*>(p);
            meta--;
            void * pMemoryHead = meta;

            if (memorySystem.IsEnableMemoryLeak())
                memorySystem.RemoveMemoryMetaData(meta);

            pAllocator->Free(pMemoryHead);
        }

        template<class U>
        struct rebind { typedef STLAllocator<U> other; };
    };
}

