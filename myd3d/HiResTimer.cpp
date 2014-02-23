#include "HiResTimer.h"


HiResTimer::HiResTimer(void)
    : m_frequency(-1.0),
      m_start(0)
{
    // Get the frequency of ticks.
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    m_frequency = double(li.QuadPart);
}


HiResTimer::~HiResTimer(void)
{
}


void HiResTimer::Start()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    m_start = li.QuadPart;
}


double HiResTimer::GetTimeInSeconds()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    double retVal = (li.QuadPart - m_start) / m_frequency;
    m_start = li.QuadPart;
    return retVal;
}