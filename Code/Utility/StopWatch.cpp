#include <Windows.h>
#include "StopWatch.h"

using namespace std::chrono;

namespace TS
{
    StopWatch::StopWatch()
    {
        QueryPerformanceFrequency(&m_frequency);
    }

    double StopWatch::Elpased()const
    {        
        LARGE_INTEGER t;
        QueryPerformanceCounter(&t);

        return static_cast<double>(t.QuadPart - m_startTime.QuadPart) / m_frequency.QuadPart;
    }

    void StopWatch::Start()
    {
        QueryPerformanceCounter(&m_startTime);        
        Recode();
    }
    double StopWatch::Recode()
    {
        LARGE_INTEGER t;
        QueryPerformanceCounter(&t);
        m_recode.push_front(t);
        if (m_recode.size() >= m_maxRecode)
            m_recode.pop_back();

        return Elpased();
    }
    void StopWatch::ClearRecode()
    {
        m_recode.clear();
    }
    void StopWatch::SetMaxRecode(unsigned m)
    {
        m_maxRecode = m;
    }


    double StopWatch::ElpasedByLastRecode()const
    {
        auto it = m_recode.begin();

        if (it == m_recode.end())
            return 0;


        LARGE_INTEGER e;
        QueryPerformanceCounter(&e);

        const auto s = (*it);

        return static_cast<double>(e.QuadPart - s.QuadPart) / m_frequency.QuadPart;
    }

    double StopWatch::GetPrevDelta() const
    {
        auto it = m_recode.begin();

        if (it == m_recode.end())
            return 0;
        if (std::next(it) == m_recode.end())
            return 0;

        const auto e = (*it);
        const auto s = (*std::next(it));

        return static_cast<double>(e.QuadPart - s.QuadPart) / m_frequency.QuadPart;
    }

    double StopWatch::GetAvgRecodeIntarval()const
    {
        int count = 0;
        double interval = 0;
        for (auto it = m_recode.begin(); it != m_recode.end(); ++it)
        {
            if (std::next(it) == m_recode.end())
                break;

            auto e = (*it);
            auto s = (*std::next(it));
            interval += static_cast<double>(e.QuadPart - s.QuadPart) / m_frequency.QuadPart;
            count++;

        }
        if (count == 0)
            return 0;
        return static_cast<double>((interval) / static_cast<double>(count));
    }
}

