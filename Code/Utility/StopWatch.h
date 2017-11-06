#pragma once

namespace TS
{
    /**
     * \brief 時間を計測するためのクラス
     */
    class StopWatch
    {
    public:
        /**
         * \brief コンストラクタ
         */
        StopWatch();

        /**
         * \brief 時間経過を取得する
         * \return 秒
         */
        double Elpased()const;

        /**
        * \brief 経過の平均レコードを取得する
        * \return 秒
        */
        double GetAvgRecodeIntarval()const;

        /**
         * \brief 最終レコードからの経過を求める
         * \return 秒
         */
        double ElpasedByLastRecode()const;

        /**
         * \brief 最終レコードのひとつ前から最終レコードまでの経過を求める
         * \return 秒
         */
        double GetPrevDelta()const;

        /**
         * \brief 計測開始
         */
        void Start();

        /**
         * \brief 結果を記録する
         * \return Startからの経過秒
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
