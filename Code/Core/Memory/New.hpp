#pragma once
/**
* \brief アロケータを使ったメモリ確保/解放処理を簡潔に書くためのファイル
*　　　　メモリ確保/解放時コンストラクタやデストラクタを暗黙的に呼びだします。
* 　　　　
*       例) SharedPtr<int> value = TS_NEW(int)(5);    //スマートポインタにそのまま代入できる
*           int* array = TS_NEWARRAY(int,30);         // int[30]の配列を確保する
*           TS_DELETE(array);                         //配列解放もTS_DELETEでOK        
*/

//! メモリ確保
#define TS_NEW(type)                TS::CallConstructor<type>(TS_LINE,TS_FILENAME,TS_FUNCTION)

//! 配列メモリの確保
#define TS_NEWARRAY(type , count)   TS::AllocArray<type>(TS_LINE,TS_FILENAME,TS_FUNCTION,count)

//! メモリ解放 / 配列メモリ解放
#define TS_DELETE(pointer)          TS::DeleteMemory(pointer)

namespace TS
{
    /**
     * \brief メモリを確保したときのヘッダデータ
     */
    struct AllockHeaderBlock : public Object
    {
        size_t      objectSize;     //! 確保したオブジェクトの1つあたりサイズ
        unsigned    arrayCount;     //! 確保したオブジェクトの数
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
        for (unsigned i = 0; i<meta->arrayCount; ++i)
        {
            pCurrent->~TypeX();
            pCurrent += meta->objectSize;
        }
        pAllocator->Free(ptr);

        if (memorySystem.IsEnableMemoryLeak())
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

        for (int i = 0; i<itemCount; ++i)
        {
            pCurrent = new (pCurrent)TypeX;
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
}
