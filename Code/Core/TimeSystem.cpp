#include "pch.h"
#include "TimeSystem.h"

TS::TimeSystem::TimeSystem(): m_Framerate(0), m_dumpFpsInterval(5)
{
}

TS::TimeSystem::TimeSystem(int frameRate):m_dumpFpsInterval(5)
{
    SetFramerate(frameRate);
    m_StopWatch.Start();
}

void TS::TimeSystem::SetFramerate(int framerate)
{
    TS_ASSERT(framerate == 0,"フレームレート0は指定できません。");
    m_Framerate = framerate;

    if (framerate > 0)
        m_StopWatch.SetMaxRecode(framerate);
}

int TS::TimeSystem::GetFrameRate() const
{
    return m_Framerate;
}

void TS::TimeSystem::BeginFrame()
{
    m_StopWatch.Recode();
}

void TS::TimeSystem::EndFrame()
{
    
}

void TS::TimeSystem::WaitRemaining()
{
    if (m_Framerate <= 0)
        return;

    double delta = m_StopWatch.ElpasedByLastRecode();

    //! フレームレート調整
    while (delta <= (1.0f / m_Framerate) - FLT_EPSILON)
    {
        delta = m_StopWatch.ElpasedByLastRecode();
    }

    if (m_dumpFpsInterval >= 0)
        DumpFps(m_dumpFpsInterval);
}

double TS::TimeSystem::GetDeltaTime() const
{
    return m_StopWatch.GetPrevDelta();
}

void TS::TimeSystem::EnableDumpFps(bool isEnable, double interval)
{
    if(!isEnable)
    {
        m_dumpFpsInterval = -1;
        return;
    }

    m_dumpFpsInterval = interval;
}

void TS::TimeSystem::DumpFps(double interval)
{
    if (m_StopWatch.Elpased() >= interval)
    {
        TS_LOG_DEBUG("fps = %2.2lf \n", 1.0f / m_StopWatch.GetAvgRecodeIntarval());
        m_StopWatch.Start();
    }
}




