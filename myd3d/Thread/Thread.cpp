#include "Thread.h"


Thread::Thread() 
    :   m_running(false),
        m_finish(false),
        m_id(0),
        m_hThread(0)
{

}

Thread::~Thread(void)
{
}


void Thread::start()
{
    m_running = true;
    m_finish = false;
    m_hThread = (HANDLE) _beginthreadex(NULL, 0, runThread, (void*)this, 0, &m_id);
}


int Thread::run()
{
    for(int i = 0; i < 10; i++)
    {
        if(m_finish)
            break;

        Sleep(1000);
        std::cout << "Thread " << m_id << " at: " << i << std::endl;
    }

    return 0;
}


void Thread::waitForFinish()
{
    WaitForSingleObject(m_hThread, INFINITE);

    CloseHandle(m_hThread);
    m_finish        = false;
    m_running       = false;
    m_hThread       = 0;
    m_id            = 0;
}
