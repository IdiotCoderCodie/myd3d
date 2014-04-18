#include "BallViewerNetworkManager.h"

#include "NET_CONSTS.h"
#include "UDPBroadcast.h"

BallViewerNetworkManager::BallViewerNetworkManager(void)
{
}


BallViewerNetworkManager::~BallViewerNetworkManager(void)
{
}

void BallViewerNetworkManager::EstablishP1Connection()
{
    UDPBroadcast broadcast;
    broadcast.SetMessage(to_string(CONNECT_MAGIC_NUM) + "-PLAY1");
    broadcast.SetBroadcastPort(BROADCAST_PORT_P1);
    
    broadcast.start();
    while(true) // Not needed? Just call broadcast.waitForFinish()???
    {
        if(!broadcast.isRunning())
        {
            broadcast.waitForFinish();

            cout << "BallViewNetManager P" << m_playerNum << ": Broadcast finished." << endl;
            SocketAddr peerAddr;
            if(!broadcast.GetPeerAddr(peerAddr))
            {
                cout << "BallViewNetManager P" << m_playerNum << ": Broadcast failed to find peer."
                     << endl;
                break;
            }

            cout << "BallViewNetManager P" << m_playerNum 
                 << ": Broadcast found peer. Attempting to connect" << endl;

            SocketAddr peerAddrStreamPort(peerAddr.GetIpAddr(), STREAM_PORT);
            Sleep(20); // Sleep gives peer time to setup their end.
            if(m_player1.Open(peerAddrStreamPort, 0))
            {
                // managed to connect.
                cout << "Connection to  Player ONE peer [" << m_player1.GetPeerAddr().ToString() << "]" 
                     << endl;

                // TODO: Something like send OK message?

                break;
            }
        }
    }
}

void BallViewerNetworkManager::EstablishP2Connection()
{
    UDPBroadcast broadcast;
    broadcast.SetMessage(to_string(CONNECT_MAGIC_NUM) + "-PLAY2");
    broadcast.SetBroadcastPort(BROADCAST_PORT_P2);
    
    broadcast.start();
    while(true) // Not needed? Just call broadcast.waitForFinish()???
    {
        if(!broadcast.isRunning())
        {
            broadcast.waitForFinish();

            cout << "BallViewNetManager P" << m_playerNum << ": Broadcast finished." << endl;
            SocketAddr peerAddr;
            if(!broadcast.GetPeerAddr(peerAddr))
            {
                cout << "BallViewNetManager P" << m_playerNum << ": Broadcast failed to find peer."
                     << endl;
                break;
            }

            cout << "BallViewNetManager P" << m_playerNum 
                 << ": Broadcast found peer. Attempting to connect" << endl;

            SocketAddr peerAddrStreamPort(peerAddr.GetIpAddr(), STREAM_PORT);
            Sleep(20); // Sleep gives peer time to setup their end.
            if(m_player2.Open(peerAddrStreamPort, 0))
            {
                // managed to connect.
                cout << "Connection to Player TWO peer [" << m_player2.GetPeerAddr().ToString() << "]" 
                     << endl;

                // TODO: Something like send OK message?

                break;
            }
        }
    }
}


int BallViewerNetworkManager::run()
{
    EstablishP1Connection();

    EstablishP2Connection();

    while(true)
    {
        Sleep(10000);

    }

    return 1;
}