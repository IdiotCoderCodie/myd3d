#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <WinSock2.h>
#include <process.h>
#include <iostream>
#include <Windows.h>

class Thread
{
public:
    Thread(void);
    virtual ~Thread(void);

    bool isRunning() { return m_running; }
  
    void start();
    void finish() { m_finish = true; }
    bool isFinishing() { return m_finish; } 
    virtual int run() = 0;
    
    void waitForFinish();

private:
    static unsigned int __stdcall runThread(void* param)
    {
        Thread* ThreadPtr = (Thread*)param;

        ThreadPtr->run();

        ThreadPtr->m_running = false;

        return 0;
    }

public:
   

private:
    bool m_finish;
    bool m_running;
    unsigned int m_id;
    HANDLE m_hThread;
};
