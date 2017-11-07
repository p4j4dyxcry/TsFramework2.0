#pragma once

namespace TS
{
    /**
     * \brief 全ての基底となるクラス
     */
    class Object
    {
    public:
        /**
         * \brief クラスの名前を取得します
         * \return クラスの名前
         */
        const char*  GetTypeName() const;

        /**
         * \brief このクラスを文字列に変換します
         * \return 文字列化されたクラス
         */
        virtual const char* ToString()const;

        
        /**
        * \brief const char * への暗黙のキャストです。
        *        デフォルトではToString()と同じ挙動になります。
        */
        virtual operator const char*() const;

        
        /**
         * \brief デストラクタ
         */
        virtual ~Object() = default;
    };
}

