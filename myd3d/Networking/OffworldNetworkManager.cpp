#include "OffworldNetworkManager.h"
#include "NET_CONSTS.h"
#include "UDPBroadcast.h"

OffworldNetworkManager::OffworldNetworkManager(void)
{
}


OffworldNetworkManager::~OffworldNetworkManager(void)
{
}


void OffworldNetworkManager::EstablishPeerConnection()
{
    UDPBroadcast broadcast;
    broadcast.SetMessage(to_string(CONNECT_MAGIC_NUM) + (m_playerNum == 1 ? "-PLAY1" : "-PLAY2"));
    broadcast.SetBroadcastPort(m_playerNum == 1 ? BROADCAST_PORT_P1 : BROADCAST_PORT_P2);
    
    broadcast.start();
    while(true) // Not needed? Just call broadcast.waitForFinish()???
    {
        if(!broadcast.isRunning())
        {
            broadcast.waitForFinish();

            cout << "OffworldNetManager P" << m_playerNum << ": Broadcast finished." << endl;
            SocketAddr peerAddr;
            if(!broadcast.GetPeerAddr(peerAddr))
            {
                cout << "OffworldNetManager P" << m_playerNum << ": Broadcast failed to find peer."
                     << endl;
                break;
            }

            cout << "OffworldNetManager P" << m_playerNum 
                 << ": Broadcast found peer. Attempting to connect" << endl;

            SocketAddr peerAddrStreamPort(peerAddr.GetIpAddr(), STREAM_PORT);
            Sleep(20); // Sleep gives peer time to setup their end.
            if(m_peer.Open(peerAddrStreamPort, 0))
            {
                // managed to connect.
                cout << "Connection to peer [" << m_peer.GetPeerAddr().ToString() << "]" 
                     << endl;

                // TODO: Something like send OK message?

                break;
            }
        }
    }
}


int OffworldNetworkManager::run()
{
    // TODO: 1. Search for connection via broadcaster.
    EstablishPeerConnection();
    // TODO: 2. Receive data from peer.
    // TODO: 3. If receive fails, send check... maybe attempt to reconnect?

    while(true)
    { // Stop thread from exiting for now....
        Sleep(10000);
    }
    return 1;
}
