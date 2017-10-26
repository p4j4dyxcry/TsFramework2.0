﻿#include "MemoryPool.h"
namespace TS
{    
    StaticMemoryPool::StaticMemoryPool(const size_t chunkSize, const unsigned chunkCount)
        :m_ChunkSize(chunkSize),
         m_ChunkCount(chunkCount),
         m_pMemory(nullptr),
         m_pCurrentChunk(nullptr),
         m_pChunkList(nullptr)

    {
        m_pMemory = new unsigned char[GetMemorySize()];
        m_pChunkList = new Chunk[GetChunkCount()];

#if _DEBUG
        memset(m_pMemory, 0, GetMemorySize());
#endif    
       
        //! チャンク情報の初期化
        for(int i=0;i<GetChunkCount();++i)
        {
            if (i == 0)
                m_pChunkList[i].pPrev = nullptr;
            else
                m_pChunkList[i].pPrev = &m_pChunkList[i - 1];

            if (i == GetChunkCount() - 1)
                m_pChunkList[i].pNext = nullptr;
            else
                m_pChunkList[i].pNext = &m_pChunkList[i + 1];

            m_pChunkList[i].isUsing = false;
            m_pChunkList[i].pUnion = nullptr;

            m_pChunkList[i].pPointer = &m_pMemory[i * GetChunkSize()];
        }

        m_pCurrentChunk = m_pChunkList;

    }

    StaticMemoryPool::~StaticMemoryPool()
    {
        delete m_pMemory;
        delete m_pChunkList;
    }

    void* StaticMemoryPool::Alloc(const size_t memorySize)
    {
        //! 要求されたメモリに対する必要なチャンクを計算する
        const unsigned requiredChunk = (memorySize / ( GetChunkSize() + 1)) + 1;

        //! 空きチャンクまで進める
        if (!SeekEmptyChunk(requiredChunk))
            return nullptr;
        
        //! 使用情報の更新
        Chunk* pHead = m_pCurrentChunk;
        for(int i=0; i<requiredChunk ; ++i)
        {
            m_pCurrentChunk->isUsing = true;
            m_pCurrentChunk->pUnion = pHead;
            m_pCurrentChunk = m_pCurrentChunk->pNext;
        }
        return pHead->pPointer;
    }

    bool StaticMemoryPool::Free(void* pointer)
    {
        //! アドレスからチャンク番号を求める
        const int chunkID = ( static_cast<unsigned char *>(pointer) - m_pMemory) / GetChunkSize();
        
        //! このメモリプールで確保されたメモリではない。
        if(chunkID < 0 || chunkID >= GetChunkCount())
            return false;


        Chunk* pHead = &m_pChunkList[chunkID];

        //! 使っていないメモリを解放しようとしている。ここで引っかかっている場合は2重解放の疑いが有る
        if (!pHead->isUsing)
            return false;

        pHead->isUsing = false;
        pHead->pUnion = nullptr;

        ClearMemory(pHead);

        Chunk* pChunk = pHead->pNext;

        //! 先頭チャンクと同じメモリの仕様情報を更新していく
        while(pChunk->pUnion == pHead)
        {
            if (!pChunk->isUsing)
                throw;

            pChunk->isUsing = false;
            pChunk->pUnion = nullptr;

            ClearMemory(pChunk);

            pChunk = pChunk->pNext;
        }
        return true;
    }

    size_t StaticMemoryPool::GetChunkSize() const
    {
        return m_ChunkSize;
    }

    unsigned StaticMemoryPool::GetChunkCount() const
    {
        return m_ChunkCount;
    }

    size_t StaticMemoryPool::GetMemorySize() const
    {
        return m_ChunkCount * m_ChunkSize;
    }

    size_t StaticMemoryPool::GetUsingMemorySize()const
    {
        size_t result = 0;
        
        for(int i=0; i<GetChunkCount() ; ++i)
        {
            if (m_pChunkList[i].isUsing)
                result += GetChunkSize();
        }
        return result;
    }

    bool StaticMemoryPool::SeekEmptyChunk(const unsigned continuous)
    {
        Chunk* pTempChunk = m_pCurrentChunk;
        int counter = 0;

        //! 1. 現在のチャンクから末端まで空きの検索を進める
        while(pTempChunk != nullptr)
        {
            if (!pTempChunk->isUsing)
                ++counter;
            if (counter >= continuous)
            {
                m_pCurrentChunk = pTempChunk;
                return true;
            }
            pTempChunk = pTempChunk->pNext;
        }

        counter = 0;
        pTempChunk = m_pChunkList;
        //! 2. (1)で見つからなかった場合は先頭からカレントまで空きの検索を進める
        while (pTempChunk != m_pCurrentChunk)
        {
            if (!pTempChunk->isUsing)
                ++counter;
            if (counter >= continuous)
            {
                m_pCurrentChunk = pTempChunk;
                return true;
            }
            pTempChunk = pTempChunk->pNext;
        }

        return false;
    }

    void StaticMemoryPool::ClearMemory(Chunk* pChunk)
    {
#if _DEBUG
        if(pChunk != nullptr)
        {
            memset(pChunk->pPointer, 0, GetChunkSize());
        }
#endif
    }

    const char* StaticMemoryPool::ToString() const
    {
        static std::string str;

        std::ostringstream ss;
        ss << "StaticMemoryPool-----------------------------------" << std::endl
            << "\t" << "メモリサイズ    " << GetMemorySize() << std::endl
            << "\t" << "チャンクサイズ  " << GetChunkSize() << std::endl
            << "\t" << "チャンク数      " << GetChunkCount() << std::endl
            << "\t" << "メモリ使用量(%%) " << GetUsingMemorySize() * 100 / static_cast<double>(GetMemorySize()) << "%%" << std::endl;
        str = ss.str();
        return str.c_str();
    }
}
