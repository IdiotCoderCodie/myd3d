#pragma once
#include "SocketStream.h"
class SocketListener : public Socket
{
public:
    SocketListener();
    SocketListener(const SocketAddr& localAddr)  { Open(localAddr); }

    bool Open(const SocketAddr& localAddr);
    bool Listen();  
    bool Accept(SocketStream& streamOut);
};

