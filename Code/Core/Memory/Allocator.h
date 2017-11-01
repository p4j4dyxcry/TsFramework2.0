#pragma once
#include "MemoryPool.h"

namespace TS
{

    /**
     * \brief アロケータのインタフェースを定義
     */
    class IAllocator : public Object
    {
    public:
        /**
         * \brief メモリ確保関数
         * \param size 要求サイズ
         * \return 
         */
        virtual void* Alloc(size_t size) = 0;


        /**
         * \brief メモリ解放関数
         * \param ptr 解放するメモリ
         * \return 
         */
        virtual bool Free(void* ptr) = 0;
    };
}

