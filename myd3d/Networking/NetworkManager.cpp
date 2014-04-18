#include <iostream>

#include "NetworkManager.h"
#include "SocketListener.h"
#include "UDPBroadcast.h"
#include "NET_CONSTS.h"

#include "../Scenes/TerrainDestructionScene.h"

using namespace std;


NetworkManager::NetworkManager(void)
    : m_receiver(),
    m_numPeers(0),
    m_playerNum(0)
{
}


NetworkManager::~NetworkManager(void)
{
}


void NetworkManager::EstablishGameConnection()
{
    // Send some broadcasts... 
    // Wait for reply (few seconds).
    // If reply, connect to peer.
    // Else start listening for a broadcast.

    cout << "NetManager: Sending broadcast for opponent... " << endl;

    UDPBroadcast broadcast;
    broadcast.SetMessage(to_string(CONNECT_MAGIC_NUM) + "-NGAME");
    //broadcast.SetAttempts(40);
    broadcast.start();
    while(true)
    {
        if(!broadcast.isRunning())
        {
            broadcast.waitForFinish();

            cout << "NetManager: NGAME Broadcast finished." << endl;
            SocketAddr peerAddr;
            if(!broadcast.GetPeerAddr(peerAddr))
            {
                // Broadcast did not successfully find a peer.
                cout << "NetManager: NGAME Broadcast failed to find a peer." << endl;
                break;
            }

            // Broadcast has found peer!
            cout << "NetManager: NGAME Broadcast found a peer. Attempting to connect... " << endl;

            
            
            SocketAddr peerAddrStreamPort(peerAddr.GetIpAddr(), STREAM_PORT);

            Sleep(50);// Sleep, give peer time to setup their end.
            if(m_opponent.Open(peerAddrStreamPort, 0))
            {
                cout << "Connected to peer: [" << m_opponent.GetPeerAddr().ToString() << "]" 
                     << endl;

                m_opponent.Send("Connection Established", 22, 0); 

                char buffer[25];
                memset(buffer, 0, 25);
                m_opponent.Recv(buffer, 20, 0);

                cout << "Received: " << buffer << endl;

                m_playerNum = 2;
                cout << "WE ARE PLAYER TWO!" << endl;

                return; // Connection established, now return.
            }
        }
    }

    cout << "NetManager: Checking for incoming broadcasts... " << endl;
    // Got here because broadcast didn't yield any results...try listening for broadcasts!
    BroadcastReceiver receiver;
    receiver.SetKey(to_string(CONNECT_MAGIC_NUM) + "-NGAME");
    receiver.SetListenPort(BROADCAST_PORT);
    receiver.start();
    while(true)
    {
        if(!receiver.isRunning())
        {
            receiver.waitForFinish();

            cout << "NetManager: Listening for NGAME Broadcasts finished..." << endl;

            // If successful... would have replied and so peer will be expecting us to be 
            // listening for connection.
            SocketListener listener(SocketAddr(INADDR_ANY, STREAM_PORT));
            if(!listener.Listen())
            {
                cout << "NetManager: Listening for connection on port " << STREAM_PORT
                     << " failed." << endl;
                break;
            }
            listener.Accept(m_opponent);
            if(m_opponent.IsOk())
            {
                cout << "NetManager: Connection made, peer: [" << m_opponent.GetPeerAddr().ToString()
                    << endl;

                char buffer[25];
                memset(buffer, 0, 25);
                m_opponent.Recv(buffer, 20, 0);

                cout << "Received: " << buffer << endl;

                m_opponent.Send("Connection Established", 22, 0);

                // Player who picks up a broadcast is set to player 1.
                m_playerNum = 1;
                cout << "WE ARE PLAYER ONE!" << endl;
                return;
            }
        }
    }

    // If got here... something went wrong...

}


void NetworkManager::CheckForNewPeer()
{
    // Check if the receiver has picked up a connection yet.
    if(!m_receiver.isRunning())
    {
        // Wait for finish closes thread and everything. So we can restart.
        m_receiver.waitForFinish();

        // Receiver has pixked up a connection.
        cout << "NetManager: Receiver has picked up a connection. " << endl;
       /* SocketAddr peerAddr;
        m_receiver.GetPeerAddr(peerAddr);
*/
        SocketListener listener(SocketAddr(INADDR_ANY, STREAM_PORT));

        if(!listener.Listen())
        {
            cout << "NetManager: Failed to listen on port" << STREAM_PORT << "." << endl;
        }
        else
        {
            if(m_numPeers < 5)
            {
                listener.Accept(m_peers[m_numPeers]);
                SocketStream& newStream = m_peers[m_numPeers];
                if(newStream.IsOk())
                {
                    cout << "NetManager: Acquired new SocketStream connection." << 
                        "Peer: " << newStream.GetPeerAddr().ToString() << endl;

                    // Can now send data to this.
                    // Doesn't need to know whether it is a camera or not, can just send data and
                    // have other side decide if it needs this data.
                    
                    m_numPeers++;                  
                }
            }
            else
            {
                cout << "NetManager: Max peers already connected. Cannot Connect another." << endl;
            }
        }
        
        // Restart the thread, checking for more incoming broadcasts.
        m_receiver.start();
    }
}


void NetworkManager::PackAndSendData()
{
    // Get data from the scene.
    std::vector<NetCircle> m_netCircles;
    
    auto circleEnts = m_scene->GetCircles();

    char sendBuffer[1000];
    memset(sendBuffer, 0, 1000);

    unsigned int byteN = 0;
    for(auto ent : circleEnts)
    {
        memcpy(&sendBuffer[byteN], "CIRC", 4);
        byteN += 4;
        memcpy(&sendBuffer[byteN], " X:", 3);
        byteN += 3;

        const glm::vec3& pos = ent->GetPos();

        memcpy(&sendBuffer[byteN], (char*)&pos.x, 4);
        byteN += 4;
        memcpy(&sendBuffer[byteN], (char*)&pos.y, 4);
        byteN += 4;             
    }

    int timeo = 100;
    setsockopt(m_opponent.GetHandle(), SOL_SOCKET, SO_SNDTIMEO, (char*)timeo, sizeof(timeo));
    m_opponent.Send(sendBuffer, byteN, 0);
}


int NetworkManager::run()
{
    // First things first... check for game connection.
    // TODO: Won't need to do this for camera views.
    EstablishGameConnection();

    // Start the receiver to check for incoming connections.
    // ERROR HERE
    // ESA 10048 here when more than one client tries to listen on the same computer on the same port.
    // WSAEADDRINUSE. Maybe have Player 1 and Player 2 listening on different ports?

    int port;
    switch(m_playerNum)
    {
    case 1:
        port = BROADCAST_PORT_P1;
        break;
    case 2:
        port = BROADCAST_PORT_P2;
        break;
    default:

        break;
    }

    m_receiver.SetListenPort(port);
    m_receiver.SetKey(to_string(CONNECT_MAGIC_NUM) + (m_playerNum == 1 ? "-PLAY1" : "-PLAY2"));
    m_receiver.start();

    float testUpdateData = 1.234567f;

    while(true)
    {       
        if(isFinishing())
            return 0;

        CheckForNewPeer();
       // Sleep(500); // TODO: Remove.

        PackAndSendData();

        //// Send a float for testing.
        //int timeoMs = 100;
        //char sendBuffer[100];
        //memcpy(sendBuffer, (char*)&testUpdateData, 4);

        //setsockopt(m_opponent.GetHandle(), SOL_SOCKET, SO_SNDTIMEO, (char*)&timeoMs, sizeof(timeoMs));
        //m_opponent.Send(sendBuffer, 4, 0);

       
        //setsockopt(m_opponent.GetHandle(), SOL_SOCKET, SO_RCVTIMEO, (char*)&timeoMs, sizeof(timeoMs));
        //char buffer[100];
        //memset(buffer, 0, 100);       

        //if(m_opponent.Recv(buffer, 100, 0) < 1)
        //{
        //    continue;
        //}

        //float val;
        //memcpy((char*)&val, buffer, 4);

        //cout << "Received float: " << val << endl;

        //testUpdateData = val + 1.0f;
    }
    return 1;
}