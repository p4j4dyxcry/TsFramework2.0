#pragma once
#include "Code/Core/Object.h"

namespace TS
{
    template<typename TKey, typename TValue>
    class Finder : Object
    {
    private:
        std::unordered_map<TKey, TValue> m_hashset;
    public:        
        Finder()
        {
        }

        bool Register(TKey key, TValue value);
        bool UnRegister(TKey key);
        
        /**
        * \brief キーを指定して条件に合うアイテムリストを取得
        * \parm key キー
        * \return オブジェクト
        */
        TValue Find(TKey key)const;

        /**
        * \brief 関数を指定して条件に合うアイテムリストを取得
        * \parm func 条件式 [ bool Func( TValue ) ]
        * \return リスト
        */
        template<typename TFunc>
        std::list<TValue> Find(TFunc func)const;
        TValue& operator[](TKey key);
        const TValue& operator[](TKey key)const;
        bool Exists(TKey key)const;
        
    };

    template <typename TKey, typename TValue>
    bool Finder<TKey, TValue>::Register(TKey key, TValue value)
    {
        if (Exists(key) == false)
            m_hashset[key] = value;
        else
            return false;
        return true;
    }

    template <typename TKey, typename TValue>
    bool Finder<TKey, TValue>::UnRegister(TKey key)
    {
        if (Exists(key))
            m_hashset.erase(key);
        else
            return false;
        return true;
    }

    template <typename TKey, typename TValue>
    TValue Finder<TKey, TValue>::Find(TKey key)const
    {
        return m_hashset.find(key);
    }

    template <typename TKey, typename TValue>
    template <typename TFunc>
    std::list<TValue> Finder<TKey, TValue>::Find(TFunc func)const
    {
        std::list<TValue> result;
        for (auto val : m_hashset)
        {
            if (func(val.second))
                result.push_back(val.second);
        }
        return result;
    }

    template <typename TKey, typename TValue>
    TValue& Finder<TKey, TValue>::operator[](TKey key)
    {
        return Find(key);
    }

    template <typename TKey, typename TValue>
    const TValue& Finder<TKey, TValue>::operator[](TKey key) const
    {
        return Find(key);
    }

    template <typename TKey, typename TValue>
    bool Finder<TKey, TValue>::Exists(TKey key) const
    {
        return m_hashset.find(key) != m_hashset.end();
    }
}
