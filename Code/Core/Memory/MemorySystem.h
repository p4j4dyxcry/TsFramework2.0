#pragma once
#include "Allocator.h"
#include <unordered_map>

namespace TS
{

    /**
    * \brief メモリ確保情報のメタデータを定義します
    */
    struct MemoryMetaData : Object
    {
        int         line;           //! ソースコード内での行番号
        const char* fileName;       //! ソースコードのファイル名
        const char* functionName;   //! 関数名
        const char* typeData;       //! タイプ情報
        size_t      memorySize;     //! メモリ量
        void *      pointer;        //! ポインタ
        IAllocator* pAllocator;     //! アロケータ
    };

    class MemorySystem : Object
    {
		friend class Engine;
    private:
        TS_DISABLE_COPY(MemorySystem);
        MemorySystem();
    public:
        static MemorySystem& Instance();

        ~MemorySystem();

        IAllocator* FindAllocator(void* ptr);
        void EnableMemoryLeakCheck();
        bool IsEnableMemoryLeak() const;
        IAllocator* SetDefaultAllocator(IAllocator* pAllocator);
        IAllocator* GetDefaultAllocator();
        IAllocator* GetSystemDefaultAllocator();
        void RegisterMemoryMetaData(MemoryMetaData metadata);
        void RemoveMemoryMetaData(void * pointer);

        void DumpLeak();
        void DumpInfo();

    private:
        bool m_isLeakChek;
        std::vector<IAllocator*> m_Allocators;
        std::unordered_map<void*,MemoryMetaData> m_MetaDatas;
        IAllocator* m_pDefaultAllocator;
        IAllocator* m_pUserAllocator;

        void AddAllocator(IAllocator* pAllocator);
    };

    MemorySystem& GetMemorySystem();

    template <class T>
    class STLAllocator : Object
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
            return reinterpret_cast<T*>(MemorySystem::Instance().GetDefaultAllocator()->Alloc(sizeof(T) * n));
        }

        void deallocate(T* p, std::size_t n)
        {
            MemorySystem::Instance().GetDefaultAllocator()->Free(p);
        }
    };

    struct AllockHeaderBlock : Object
    {
        size_t      objectSize;
        unsigned    arrayCount;
    };

    /**
     * \brief メタ情報を埋め込みつつ可変長コンストラクタを呼ぶ仕組み
     */
    template<typename TypeX>
    struct CallConstructor
    {
        //メタ情報の埋め込み
        MemoryMetaData memory_meta;
        CallConstructor(const int line,
            const char* filename,
            const char* functionname)
        {
            memory_meta.line = line;
            memory_meta.fileName = filename;
            memory_meta.functionName = functionname;
        } 

        //! 対象のメモリを確保しコンストラクタを呼び出す
        /**
        * \brief対象のメモリを確保しコンストラクタを呼び出す
        * \param params 作成するクラスのコンストラクタ引数
        * \return 作成されたオブジェクト
        */
        template <typename... Params>
        TypeX* operator()(Params... params)
        {
            auto& memorySystem = GetMemorySystem();

            IAllocator* pAllocator = memorySystem.GetSystemDefaultAllocator();

            const size_t memorySize = sizeof(TypeX) + sizeof(AllockHeaderBlock);

            auto pMemory = static_cast<char*>(pAllocator->Alloc(memorySize));
            AllockHeaderBlock* block = new(pMemory)AllockHeaderBlock;

            block->objectSize = sizeof(TypeX);
            block->arrayCount = 1;

            pMemory += sizeof(AllockHeaderBlock);
            TypeX* pObject = new (pMemory)TypeX(std::forward<Params>(params)...);


            if (memorySystem.IsEnableMemoryLeak())
            {
                memory_meta.pAllocator = pAllocator;
                memory_meta.memorySize = memorySize;
                memory_meta.pointer = pMemory;
                memory_meta.typeData = typeid(TypeX).name();
                memorySystem.RegisterMemoryMetaData(memory_meta);

            }
            return pObject;
        }
    };


    /**
     * \brief メモリを開放する
     * \param ptr メモリを開放するポインタ
     */
    template <typename TypeX> 
    void DeleteMemory(TypeX*& ptr)
    {

        auto& memorySystem = GetMemorySystem();

        IAllocator* pAllocator = memorySystem.FindAllocator(ptr);

        AllockHeaderBlock* meta = reinterpret_cast<AllockHeaderBlock*>(ptr);
        meta--;

        TypeX* pCurrent = ptr;
        for(unsigned i =0 ; i<meta->arrayCount ; ++i)
        {
            pCurrent->~TypeX();
            pCurrent += meta->objectSize;
        }
        pAllocator->Free(ptr);

        if( memorySystem.IsEnableMemoryLeak())
            memorySystem.RemoveMemoryMetaData(ptr);
		ptr = nullptr;
    }

    /**
     * \brief 配列のメモリを確保する
     * \param itemCount 配列の数
     */
    template <typename TypeX>
    TypeX* AllocArray(const int line,
                    const char* filename,
                    const char* functionname,
                    const int itemCount)
    {
        auto& memorySystem = GetMemorySystem();

        IAllocator* pAllocator = GetMemorySystem().GetSystemDefaultAllocator();

        const size_t memorySize = sizeof(TypeX) * itemCount + sizeof(TypeX);

        auto pMemory = static_cast<char*>(pAllocator->Alloc(memorySize));

        AllockHeaderBlock* block = new(pMemory)AllockHeaderBlock;

        block->objectSize = sizeof(TypeX);
        block->arrayCount = itemCount;

        pMemory += sizeof(AllockHeaderBlock);

        TypeX* pCurrent = reinterpret_cast<TypeX*>(pMemory);

        for(int i=0; i<itemCount ; ++i)
        {
            pCurrent = new (pMemory)TypeX;
            ++pCurrent;
        }

        // ! メタ情報を埋め込んでおく
        if (memorySystem.IsEnableMemoryLeak())
        {
            MemoryMetaData memory_meta;

            memory_meta.line = line;
            memory_meta.fileName = filename;
            memory_meta.functionName = functionname;

            memory_meta.pAllocator = pAllocator;
            memory_meta.memorySize = memorySize;
            memory_meta.pointer = pMemory;
            memory_meta.typeData = typeid(TypeX).name();
            memorySystem.RegisterMemoryMetaData(memory_meta);
        }
        return reinterpret_cast<TypeX*>(pMemory);
    }


#define TS_NEW(type)                CallConstructor<type>(TS_LINE,TS_FILENAME,TS_FUNCTION)
#define TS_NEWARRAY(type , count)   AllocArray<type>(TS_LINE,TS_FILENAME,TS_FUNCTION,count)
#define TS_DELETE(pointer)          DeleteMemory(pointer)

}
