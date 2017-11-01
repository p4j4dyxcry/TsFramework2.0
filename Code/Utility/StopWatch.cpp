#include "StopWatch.h"

using namespace std::chrono;

namespace TS
{
    template < typename T>
    double StopWatch::Elpased()const
    {
        auto d = std::chrono::system_clock::now() - m_startTime;
        return (double)duration_cast<T>(d).count();
    };

    double StopWatch::ElpasedSeccond()const
    {
        return Elpased<seconds>();
    }
    double StopWatch::ElpasedmSecond()const
    {
        return Elpased<milliseconds>();
    }

    double StopWatch::ElpasedmicroSecond()const
    {
        return Elpased<microseconds>();
    }

    double StopWatch::ElpasednSecond()const
    {
        return Elpased<nanoseconds>();
    }


    void StopWatch::Start()
    {
        m_startTime = std::chrono::system_clock::now();
    }
    double StopWatch::Recode()
    {
        m_recode.push_back(std::chrono::system_clock::now());
        if (m_recode.size() >= m_maxRecode)
            m_recode.pop_front();

        return ElpasedmSecond();
    }
    void StopWatch::ClearRecode()
    {
        m_recode.clear();
    }
    void StopWatch::SetMaxRecode(unsigned m)
    {
        m_maxRecode = m;
    }
}

