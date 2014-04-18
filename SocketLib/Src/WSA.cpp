#include "../Inc/WSA.h"


WSA::WSA()
:   m_isOk(false),
    m_wsaData()
{
    // Create version identifier.
    WORD wVersionRequested = MAKEWORD(2, 0);
    if (WSAStartup(wVersionRequested, &m_wsaData))
    {
        m_isOk = false;
    }
    else
    {
        m_isOk = true;
    }
}


WSA::~WSA()
{
    WSACleanup();
}


