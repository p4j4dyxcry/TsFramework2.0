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
         * \brief 経過を取得する
         * \tparam T どの型で経過を取得するか
         * \return 
         */
        template <typename T>
        double Elpased()const;

        /**
         * \brief 秒で経過を取得する
         * \return 
         */
        double ElpasedSeccond()const;

        /**
        * \brief ミリ秒で経過を取得する
        * \return
        */
        double ElpasedmSecond()const;

        /**
        * \brief マイクロ秒で経過を取得する
        * \return
        */
        double ElpasedmicroSecond()const;

        /**
        * \brief ナノ秒で経過を取得する
        * \return
        */
        double ElpasednSecond()const;

        /**
        * \brief 経過の平均レコードを取得する
        * \tparam T 秒、ミリ秒、マイクロ秒ナノ秒
        * \return
        */
        template <class T = std::chrono::milliseconds>
        double GetAvgRecodeIntarval()const;

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
        std::chrono::system_clock::time_point m_startTime;
        unsigned int m_maxRecode = 15;
        std::list<std::chrono::system_clock::time_point> m_recode;
    };

    template < class T>
    double StopWatch::GetAvgRecodeIntarval()const
    {
        int count = 0;
        long long interval = 0;
        for (auto it = m_recode.begin(); it != m_recode.end(); ++it)
        {
            if (std::next(it) != m_recode.end())
            {
                auto s = (*it);
                auto e = (*std::next(it));
                interval += std::chrono::duration_cast<T>(e - s).count();
                count++;
            }
            else
            {
                break;
            }
        }
        if (count == 0)
            return 0;
        return static_cast<double>((interval) / static_cast<double>(count));
    }
}
