#pragma once
#include "Allocator.h"
#include <unordered_map>

namespace TS
{
	/**
	* \brief メモリを確保したときのヘッダデータ
	*/
	struct MemoryMetaData : public Object
	{
		size_t      objectSize;     //! 確保したオブジェクトの1つあたりサイズ
		unsigned    arrayCount;     //! 確保したオブジェクトの数

		MemoryMetaData * pPrevBlock;
		MemoryMetaData * pNextBlock;

		int         line;           //! ソースコード内での行番号
		const char* fileName;       //! ソースコードのファイル名
		const char* functionName;   //! 関数名
		const char* typeData;       //! タイプ情報

		void*		pointer;		//! 先頭ポインタ情報
		IAllocator* pAllocator;		//! アロケータ情報

	};

    class MemorySystem : Object
    {
		friend class Engine;
    private:
        TS_DISABLE_COPY(MemorySystem);
        TS_DISABLE_MOVE(MemorySystem);
        MemorySystem();
    public:

        /**
         * \brief デストラクタ
         */
        virtual ~MemorySystem();

        /**
         * \brief メモリを確保したアロケータを取得する
         * \param ptr 
         * \return 
         */
        IAllocator* FindAllocator(void* ptr);

        
        
        /**
         * \brief メモリリーク検知機能を有効にする
         *         TODO 一時しのぎ実装でコンテナを使っているため非常に遅い傾向
         */
        void EnableMemoryLeakCheck();


        /**
         * \brief メモリリークチェックが有効か取得する
         * \return 
         */
        bool IsEnableMemoryLeak() const;


        /**
         * \brief デフォルトアロケータを設定する
         * \param pAllocator 
         * \return 
         */
        IAllocator* SetDefaultAllocator(IAllocator* pAllocator);

        
        /**
         * \brief デフォルトアロケータを取得する
         * \return 
         */
        IAllocator* GetDefaultAllocator();

        /**
        * \brief システムで使用するデフォルトアロケータを取得する
        * \return
        */
        IAllocator* GetSystemDefaultAllocator();


        /**
         * \brief メモリを確保した際の情報を登録する
         * \param metadata 
         */
        void RegisterMemoryMetaData(MemoryMetaData* metadata);



        /**
         * \brief メモリを開放する際に登録したメタデータを削除する
         * \param pointer 
         */
        void RemoveMemoryMetaData(MemoryMetaData * metadata);

        /**
         * \brief メモリリーク情報をロガーに出力する
         */
        void DumpLeak();

        /**
         * \brief　メモリ情報をロガーに出力する
         */
        void DumpInfo();

		int GetAllocMemoryCount()const;

    protected:
        /**
         * \brief アロケータを追加する
         * \param pAllocator 
         */
        void AddAllocator(IAllocator* pAllocator);

    private:
        bool m_isLeakChek;
        std::vector<IAllocator*> m_Allocators;
        MemoryMetaData* m_pHeadMetaData;
		MemoryMetaData* m_pCurrentMetaData;
		IAllocator* m_pDefaultAllocator;
        IAllocator* m_pUserAllocator;
    };

    /**
     * \brief この関数はEngine::Instance().GetMemorySystem()と等しい
     * \return 
     */
    MemorySystem& GetMemorySystem();
}

#include "New.hpp"
