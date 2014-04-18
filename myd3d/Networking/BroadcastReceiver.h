#pragma once
#include <SocketDgram.h>
#include "../Thread/Thread.h"

class BroadcastReceiver : public Thread
{
public:
    BroadcastReceiver(void);
    ~BroadcastReceiver(void);

    int run();

    bool GetPeerAddr(SocketAddr& out);

    void SetListenPort(int port) { m_port = port; }
    void SetKey(const std::string& newKey) { m_key = newKey; }

private:
    SocketDgram m_socket;
    int         m_port;
    bool        m_foundClient;
    SocketAddr  m_clientAddr;
    std::string m_key; // the key broadcast to be listening for.
};
