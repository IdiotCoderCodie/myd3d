#pragma once
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include "SocketAddr.h"

using namespace std;

class Socket
{
private:
    Socket(const Socket&);
    Socket operator=(const Socket&);

public:
    Socket() : m_hSocket(INVALID_SOCKET) {}
    Socket(SOCKET socket) : m_hSocket(socket) {}
    ~Socket() { Close(); }
    
    void Close();
    void SetHandle(SOCKET handle);
    SOCKET GetHandle() const { return m_hSocket; }


    bool IsOk() { return m_hSocket != INVALID_SOCKET; }
   /* bool Connect(sockaddr_in& peer, unsigned short localPort);
    int Send(const char* buffer, int size, int flags);
    int Recv(char* buffer, int size, int flags);*/

    SocketAddr GetLocalAddr() const;
    SocketAddr GetPeerAddr() const;

private:
    SOCKET      m_hSocket;
};

