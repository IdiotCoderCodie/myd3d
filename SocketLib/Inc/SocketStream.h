#pragma once
#include "Socket.h"
#include "SocketAddr.h"

class SocketStream : public Socket
{
public:
    SocketStream(void) {};
    SocketStream(SOCKET handle) : Socket(handle) {}
    //SocketStream(const SocketAddr& peerAddr, u_short localPort);

    bool Open(const SocketAddr& peerAddr, u_short localPort = 0);
    int Send(const char* buffer, int size, int flags);
    int Recv(char* buffer, int size, int flags);
};

