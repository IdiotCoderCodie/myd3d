#pragma once

#include "SocketStream.h"
#include "../Thread/Thread.h"

class BallViewerNetworkManager : public Thread
{
public:
    BallViewerNetworkManager(void);
    ~BallViewerNetworkManager(void);

    void EstablishP1Connection();
    void EstablishP2Connection();

    int run();
    
    void SetPlayerNumToFollow(int num) { m_playerNum = num; }

private:
    int m_playerNum; // Player to follow latest canon ball.
    SocketStream m_player1;
    SocketStream m_player2;
};

