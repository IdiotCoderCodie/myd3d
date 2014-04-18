#pragma once
#include <SocketDgram.h>
#include "../Thread/Thread.h"

class UDPBroadcast : public Thread
{
public:
    UDPBroadcast(void);
    ~UDPBroadcast(void);

    int run();

    // Returns the address of the peer found from the broadcast.
    bool GetPeerAddr(SocketAddr& out);

    void SetMessage(const std::string& message) { m_message = message; }
    void SetBroadcastPort(int port) { m_port = port; }
    void SetAttempts(int attempts) { m_attempts = attempts; }

private:
    SocketDgram     m_socket;
    SocketAddr      m_peerAddr;
    bool            m_foundPeer;
    std::string     m_message;
    int             m_port;
    unsigned int    m_attempts;
};
