#pragma once
#include <bemapiset.h>

namespace TS
{
    /**
     * \brief 時間を計測するためのクラス
     */
    class StopWatch
    {
    public:

        StopWatch();

        /**
         * \brief ミリ秒で時間経過を取得する
         * \return 
         */
        double Elpased()const;

        /**
        * \brief 経過の平均レコードを取得する
        * \tparam T 秒、ミリ秒、マイクロ秒ナノ秒
        * \return
        */
        double GetAvgRecodeIntarval()const;

        double GetLastRecodeDelta()const;

        double GetPrevDelta()const;
        /**
         * \brief 計測開始
         */
        void Start();


        /**
         * \brief 結果を記録しておく
         * \return 
         */
        double Recode();


        /**
         * \brief 結果一覧を削除する
         */
        void ClearRecode();

        /**
         * \brief 結果の最大保存容量
         * \param m 
         */
        void SetMaxRecode(unsigned m);
    protected:
        LARGE_INTEGER m_startTime;
        LARGE_INTEGER m_frequency;
        unsigned int m_maxRecode = 60;
        std::list<LARGE_INTEGER> m_recode;
    };


}
