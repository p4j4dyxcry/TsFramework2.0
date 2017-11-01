#pragma once

#include "Code/Core/Develop.h"
#include "Code/Core/Thread.h"
namespace TS
{
    /**
     * \brief メモリプールを定義するためのインタフェース
     */
    class IMemoryPool : public IMutex
    {
    public:
        virtual void* Alloc(const size_t memorySize) = 0;
        virtual bool  Free(void* pointer) = 0;
    };
    
    /**
     * \brief 固定サイズのブロックから構成されるメモリープール
     */
    class StaticMemoryPool : public IMemoryPool
    {
    private:
        struct Chunk
        {
            bool    isUsing;
            Chunk * pNext;
            Chunk * pPrev;
            Chunk * pUnion;
            void  * pPointer;
        };
    public:

        /**
         * \brief コンストラクタ
         * \param chunkSize 1つのチャンクの大きさ
         * \param chunkCount チャンクの数
         */
        StaticMemoryPool(const size_t chunkSize , 
                         const unsigned chunkCount);
        /**
         * \brief デストラクタ
         */
        virtual ~StaticMemoryPool();

        /**
         * \brief メモリを確保する
         * \param memorySize 確保するメモリサイズ
         * \return 失敗した場合はnullptr
         */
        void* Alloc(const size_t memorySize) override;

        /**
         * \brief メモリを開放する
         * \param pointer このメモリプールから確保したメモリのポインタ
         * \return 失敗した場合はfalse
         */
        bool  Free(void * pointer)override;

        /**
         * \brief チャンクの大きさを取得する
         * \return 
         */
        size_t GetChunkSize() const;

        
        /**
         * \brief チャンク数を取得する
         * \return 
         */
        unsigned GetChunkCount() const;

        /**
        * \brief メモリプール全体のメモリ量を取得する
        * \return
        */
        size_t GetMemorySize() const;

        /**
         * \brief 使用されているメモリ量を取得する
         * \return 
         */
        size_t GetUsingMemorySize()const;

        /**
         * \brief 指定されたポインタがこのプールから確保された物か調べる
         * \param pointer 
         * \return 
         */
        bool From(void* pointer) const;

        const char* ToString() const override;

        //! コピーコンストラクタとムーブコンストラクタを無効化する
        TS_DISABLE_COPY(StaticMemoryPool);
        TS_DISABLE_MOVE(StaticMemoryPool);

    private:

        /**
         * \brief 空きチャンクまでカレントチャンクを移動させる
         * \param continuous 必要な連続した空きチャンク数
         * \return 空きチャンクが無い場合はfalse
         */
        bool SeekEmptyChunk(const unsigned continuous);

        /**
         * \brief チャンク内のメモリを0クリアする。この関数はデバッグ実行時のみ有効化される
         * \param pChunk 
         */
        void ClearMemory(Chunk* pChunk);
    private:
        size_t         m_ChunkSize;     //! 1つのチャンクの容量。2を基底とする乗数を設定する
        unsigned int   m_ChunkCount;    //! プールに含まれるチャンクの数
        size_t         m_MemorySize;    //! チャンクサイズ * チャンク数
        unsigned char* m_pMemory;       //! メモリプール
        Chunk*         m_pCurrentChunk; //! 現在のチャンク
        Chunk *        m_pChunkList;    //! チャンクの一覧
    };
}
