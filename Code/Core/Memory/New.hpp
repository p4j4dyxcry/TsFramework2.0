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
#define TS_NEW(...)                TS::MetaDataIncludeConstructor<__VA_ARGS__>(TS_LINE,TS_FILENAME,TS_FUNCTION)

//! 配列メモリの確保
#define TS_NEWARRAY(type , count)   TS::AllocArray<type>(TS_LINE,TS_FILENAME,TS_FUNCTION,count)

//! メモリ解放 / 配列メモリ解放
#define TS_DELETE(pointer)          TS::DeleteMemory(pointer)

namespace TS
{
    /**
    * \brief メタ情報を埋め込みつつ可変長コンストラクタを呼ぶ仕組み
    */
    template<typename TypeX>
    struct MetaDataIncludeConstructor
    {
		struct FuncData
		{
			int line;
			const char* filename;
			const char* function;
		};

        //メタ情報の埋め込み
		FuncData metaData;
        MetaDataIncludeConstructor(const int line,
            const char* filename,
            const char* functionname)
        {
			metaData.line = line;
			metaData.filename = filename;
			metaData.function = functionname;
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

            const size_t memorySize = sizeof(TypeX) + sizeof(MemoryMetaData);

            auto pMemory = static_cast<char*>(pAllocator->Alloc(memorySize));
			MemoryMetaData* block = new(pMemory)MemoryMetaData;

            block->objectSize = sizeof(TypeX);
            block->arrayCount = 1;
			block->fileName = metaData.filename;
			block->line = metaData.line;
			block->functionName = metaData.function;
            pMemory += sizeof(MemoryMetaData);
            TypeX* pObject = new (pMemory)TypeX(std::forward<Params>(params)...);


            if (memorySystem.IsEnableMemoryLeak())
            {
                block->pAllocator = pAllocator;

                block->pointer = pObject;
                block->typeData = typeid(TypeX).name();
                memorySystem.RegisterMemoryMetaData(block);
            }
            return pObject;
        }
    };

	/**
	* \brief メモリのメタデータを取得します。
	* \param ptr メモリを開放するポインタ
	*/
	inline MemoryMetaData& GetMemoryMetaDeta(void* pointer)
	{
		return *reinterpret_cast<MemoryMetaData*>( static_cast<char*>(pointer) - sizeof(MemoryMetaData));
	}

    /**
    * \brief メモリを開放する
    * \param ptr メモリを開放するポインタ
    */
    template <typename TypeX>
    void DeleteMemory(TypeX*& ptr)
    {

        auto& memorySystem = GetMemorySystem();

        IAllocator* pAllocator = memorySystem.FindAllocator(ptr);

		MemoryMetaData& meta = GetMemoryMetaDeta(ptr);

		void * pMemoryHead = &meta;


        TypeX* pCurrent = ptr;
        for (unsigned i = 0; i<meta.arrayCount; ++i)
        {
            pCurrent[i].~TypeX();
        }

        if (memorySystem.IsEnableMemoryLeak())
            memorySystem.RemoveMemoryMetaData(&meta);

        pAllocator->Free(pMemoryHead);
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

        const size_t memorySize = sizeof(TypeX) * itemCount + sizeof(MemoryMetaData);

        auto pMemory = static_cast<char*>(pAllocator->Alloc(memorySize));

		MemoryMetaData* block = new(pMemory)MemoryMetaData;

        block->objectSize = sizeof(TypeX);
        block->arrayCount = itemCount;

		pMemory += sizeof(MemoryMetaData);

        TypeX* pCurrent = reinterpret_cast<TypeX*>(pMemory);

        for (int i = 0; i<itemCount; ++i)
        {
            new (pCurrent)TypeX;
            ++pCurrent;
        }

        // ! メタ情報を埋め込んでおく
        if (memorySystem.IsEnableMemoryLeak())
        {
            block->line = line;
            block->fileName = filename;
            block->functionName = functionname;
            block->pAllocator = pAllocator;

            block->pointer = pMemory;
            block->typeData = typeid(TypeX).name();
            memorySystem.RegisterMemoryMetaData(block);
        }
        return reinterpret_cast<TypeX*>(pMemory);
    }
}
