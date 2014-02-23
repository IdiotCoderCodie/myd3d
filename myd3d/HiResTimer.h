#pragma once
#include <windows.h>

class HiResTimer
{
public:
    HiResTimer(void);
    ~HiResTimer(void);

    void Start();

    /**
     * Returns time since Start() or GetTime*() was called in seconds.
     */
    double GetTimeInSeconds();
    /**
     * Returns time since Start() or GetTime*() was called in milliseconds.
     */
    double GetTimeInMilliseconds() { return (GetTimeInSeconds() * 1000.0); }
    /**
     * Returns time since Start() or GetTime*() was called in microseconds.
     */
    double GetTimeInMicroseconds() { return (GetTimeInSeconds() * 1000000.0); }

private:
    double m_frequency;
    INT64 m_start;
};

