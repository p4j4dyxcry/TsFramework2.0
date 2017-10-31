#pragma once

namespace TS
{
    class StopWatch
    {
    public:
        template <typename T>
        double Elpased()const;

        double ElpasedSeccond()const;
        double ElpasedmSecond()const;
        double ElpasednSecond()const;
        double ElpasedmicroSecond()const;

        template <class T = std::chrono::milliseconds>
        double GetAvgRecodeIntarval()const;

        void Start();
        double Recode();
        void ClearRecode();
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
