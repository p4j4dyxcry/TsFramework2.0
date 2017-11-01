#pragma once

namespace TS
{

    /**
    * \brief 共有を許さないスマートポインタ
    * \tparam T
    */
    template <typename T>
    class UniquePtr;

    /**
    * \brief 強参照スマートポインタ
    *        参照カウンタの操作権限　有
    *        監視カウンタの操作権限　有
    *        メモリの破棄権限　　　　有
    *        参照カウンタの破棄権限　有
    * \tparam T
    */
    template <typename T>
    class SharedPtr;

    /**
    * \brief 弱参照スマートポインタ
    *        参照カウンタの操作権限　無
    *        監視カウンタの操作権限　有
    *        メモリの破棄権限　　　　無
    *        参照カウンタの破棄権限　有
    * \tparam T
    */
    template <typename T>
    class WeakPtr;
};