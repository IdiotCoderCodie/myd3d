#pragma once
#include "Socket.h"

class SocketDgram : public Socket
{
public:
    SocketDgram() {}
    SocketDgram(SOCKET handle) : Socket(handle) {}

    bool Open(u_short localPort);
    int SendTo(const SocketAddr& peerAddr, const char* buffer,
                         int size, int flags);
    int RecvFrom(char* buffer, int size, int flags, SocketAddr& fromOut);

};

