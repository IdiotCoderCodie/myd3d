#pragma once

#include "BroadcastReceiver.h"
#include "SocketStream.h"
#include "../Thread/Thread.h"

class OffworldNetworkManager : public Thread
{
public:
    OffworldNetworkManager(void);
    ~OffworldNetworkManager(void);

    void EstablishPeerConnection();

    int run();

    void SetPlayerNum(int num) { m_playerNum = num; }

private:
    int m_playerNum;
    SocketStream m_peer; 
};
