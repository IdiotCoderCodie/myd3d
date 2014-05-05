#include <iostream>
#include <sstream>

#include "NetworkManager.h"
#include "SocketListener.h"
#include "UDPBroadcast.h"
#include "NET_CONSTS.h"

#include "../Scenes/TerrainDestructionScene.h"

using namespace std;


NetworkManager::NetworkManager(void)
:   m_foundOpponent(false),
    m_receiver(),
    m_numPeers(0),
    m_playerNum(1),
    m_timer(),
    m_targetUps(20),
    m_actualUps()
{
    for(int i = 0; i < NM_MAX_PEERS; i++)
    {
        m_peerIsInit[i] = false;
    }

    m_tweakBar = TwNewBar("Networking");
    
    TwAddVarRW(m_tweakBar, "targetUPS", TW_TYPE_INT32, &m_targetUps, "");
    TwAddVarRO(m_tweakBar, "actualUPS", TW_TYPE_INT32, &m_actualUps, "");
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
            SetThreadAffinityMask(GetHandle(), 2);

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
                m_foundOpponent = true;
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
                m_foundOpponent = true;
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
        Sleep(250);
        SetThreadAffinityMask(GetHandle(), 2);

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

                    // TODO: INITIALIZATION!!
                    // TODO: Send all current data from the scene to other, so it can construct it all ready for updates!
                    SendInitData(newStream);
                    
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
    if(m_numPeers < 1)
    {
        return;
    }

    // Get data from the scene.
    std::vector<NetCircle> m_netCircles;
    
    auto circleEnts = m_scene->GetCircles();

    char sendBuffer[1000];
    memset(sendBuffer, 0, 1000);

    std::string sendBufferStr;
    std::ostringstream ssSendBuffer;

    // Pack data into string.
    unsigned int byteN = 0;
    for(auto ent : circleEnts)
    {
        const glm::vec3& pos = ent->GetPos();
        ssSendBuffer << "CIRC X:" << pos.x << " " << pos.y << endl;  
    }
    
    ssSendBuffer.seekp(0, ios::end);
    int bufSize = ssSendBuffer.tellp();
    int timeo = 100;
    setsockopt(m_opponent.GetHandle(), SOL_SOCKET, SO_SNDTIMEO, (char*)timeo, sizeof(timeo));
    for(int i = 0; i < m_numPeers; i++)
    {
        if(m_peers[i].IsOk())
        {
            m_peers[i].Send(ssSendBuffer.str().c_str(), bufSize, 0);
        }
    }

    //m_opponent.Send(ssSendBuffer.str().c_str(), bufSize, 0);
}


void NetworkManager::SendUpdateData()
{
    if(m_numPeers < 1)
    {
        return;
    }

    std::ostringstream ssBuffer;

 
    for(auto ent : m_scene->GetCircles())
    {
        ssBuffer << "UPD " << ent->GetID() << " "
                 << "CIRC " << "X: " << ent->GetPos().x << " " << ent->GetPos().y << endl;
        // TODO: get rotation vectors.
    }

    for(auto ent : m_scene->GetAABBs())
    {
        ssBuffer << "UPD " << ent->GetID() << " "
                 << "SQR " << "X: " << ent->GetPos().x << " " << ent->GetPos().y << endl;
    }

    m_scene->GetNewNetworkCircles(ssBuffer); // Add on any Additional circles :)

    ssBuffer.seekp(0, ios::end);
    int bufSize = ssBuffer.tellp();
    int timeo = 1000;
    for(int i = 0; i < m_numPeers; i++)
    {
        if(m_peers[i].IsOk())
        {
            setsockopt(m_peers[i].GetHandle(), SOL_SOCKET, SO_SNDTIMEO, (char*)&timeo, sizeof(timeo));
            m_peers[i].Send(ssBuffer.str().c_str(), bufSize, 0);
        }
    }
    // TODO: Send squares.
}


void NetworkManager::SendInitData(SocketStream& peer)
{
    std::ostringstream ssBuffer;

    ssBuffer << "INIT" << endl;
    auto& circleEnts = m_scene->GetCircles();
   
    for(auto ent : circleEnts)
    {
        ssBuffer << "ENT " << ent->GetID() << " "
                 << "CIRC " << "X: " << ent->GetPos().x << " " << ent->GetPos().y 
                 << "R: " << ent->GetRadius() << endl;
        // TODO: Get rotation vectors...etc...
    }

    auto& squareEnts = m_scene->GetAABBs();
    for (auto ent : squareEnts)
    {
        ssBuffer << "ENT " << ent->GetID() << " "
                 << "SQR " << "X: " << ent->GetPos().x << " " << ent->GetPos().y 
                 << "W: " << ent->GetMax().x - ent->GetMin().x  << " "
                 << "H: " << ent->GetMax().y - ent->GetMin().y                
                 << endl;

        // TODO: Create PhysSquareEnt, like PhysCircleEnt to get all needed data.
    }

    Entity& cannon = m_scene->GetCannon();
    ssBuffer << "ENT " << cannon.GetID() << " "
             << "CAN " << "X: " << cannon.GetPos().x << " " << cannon.GetPos().y
             << endl;

    ssBuffer << "ENDINIT" << endl;

    // Get the size of the string.
    ssBuffer.seekp(0, ios::end);
    int bufSize = ssBuffer.tellp();
    int timeo = 1000;
    setsockopt(peer.GetHandle(), SOL_SOCKET, SO_SNDTIMEO, (char*)&timeo, sizeof(timeo));

    if(peer.IsOk())
    {
        peer.Send(ssBuffer.str().c_str(), bufSize, 0);
    }

    // Wait for response to confirm initialization.

    char response[100];
    memset(response, 0, 100);
    setsockopt(peer.GetHandle(), SOL_SOCKET, SO_RCVTIMEO, (char*)&timeo, sizeof(timeo));

    for (int i = 0; i < 5; i++) // Loop to keep checking for a response, if no response then leave.
    {
        //while(true)
        { // Keep cheking if received anything. Will break out of loop when "ACK" received.
            int bytesRead = peer.Recv(response, 100, 0);
            timeo = 1000;
            if(bytesRead > 0)
            {
                // Read something...
                std::string sBuffer = std::string(response);
                if(!sBuffer.compare("ACK"))
                {
                    // Acknowledgement of initialization confirmed.
                    // Break out of loop pls.
                    break;
                }
            }
        }
    }
}


int NetworkManager::run()
{
    SetThreadAffinityMask(GetHandle(), 2);
    
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

    m_timer.Start();
    while(true)
    {       
        if(isFinishing())
            return 0;

        float elapsedTime = (float)m_timer.GetTimeInSeconds(); // How long last iteration took.    
        if (m_targetUps > 0)
        {
            float targetUpdTime = 1.0f / m_targetUps;
            if (elapsedTime < targetUpdTime)
            {
                float sleepTime = (targetUpdTime - elapsedTime) * 1000.0f;
                Sleep((DWORD)sleepTime);
                SetThreadAffinityMask(GetHandle(), 2);
                elapsedTime += m_timer.GetTimeInSeconds(); // Lengthen iteration length.
            }
        }
        m_actualUps = (int)(1.0f / elapsedTime);

        CheckForNewPeer();
        SendUpdateData();
    }
    return 1;
}