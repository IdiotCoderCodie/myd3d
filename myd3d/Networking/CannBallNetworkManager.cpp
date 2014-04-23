#include "CannBallNetworkManager.h"
#include "NET_CONSTS.h"
#include "UDPBroadcast.h"

#include "../Scenes/TerrainDestructionCannBallScene.h"

#include <sstream>

CannBallNetworkManager::CannBallNetworkManager(void)
{
}


CannBallNetworkManager::~CannBallNetworkManager(void)
{
}


void CannBallNetworkManager::EstablishPeerConnection(int playerNum)
{
    UDPBroadcast broadcast;
    broadcast.SetMessage(to_string(CONNECT_MAGIC_NUM) + (playerNum == 1 ? "-PLAY1" : "-PLAY2"));
    broadcast.SetBroadcastPort(playerNum == 1 ? BROADCAST_PORT_P1 : BROADCAST_PORT_P2);
    
    SocketStream& peer = (playerNum == 1 ? m_player1 : m_player2);
    // TODO: REFACTOR THIS SHIT!
    broadcast.start();
    while(true) // Not needed? Just call broadcast.waitForFinish()???
    {
        if(!broadcast.isRunning())
        {
            broadcast.waitForFinish();

            cout << "OffworldNetManager P" << playerNum << ": Broadcast finished." << endl;
            SocketAddr peerAddr;
            if(!broadcast.GetPeerAddr(peerAddr))
            {
                cout << "OffworldNetManager P" << playerNum << ": Broadcast failed to find peer."
                     << endl;
                break;
            }

            cout << "OffworldNetManager P" << playerNum 
                 << ": Broadcast found peer. Attempting to connect" << endl;

            SocketAddr peerAddrStreamPort(peerAddr.GetIpAddr(), STREAM_PORT);
            Sleep(20); // Sleep gives peer time to setup their end.
            if(peer.Open(peerAddrStreamPort, 0))
            {
                // managed to connect.
                cout << "Connection to peer [" << peer.GetPeerAddr().ToString() << "]" 
                     << endl;

                // TODO: Connected to player. Now read initialization!

                char recvBuffer[2000];
                memset(recvBuffer, 0, 2000);
                if(peer.Recv(recvBuffer, 2000, 0) > 0)
                {
                    std::stringstream ssBuffer(recvBuffer);
                    std::string head;
                    ssBuffer >> head;
                    if(!head.compare("INIT"))
                    {
                        // Now read in entities.
                        while(!ssBuffer.eof())
                        {
                            std::string entityId;
                            ssBuffer >> head;
                            if(!head.compare("ENT"))
                            {                                
                                // Load in entity data.
                                ssBuffer >> entityId;
                                 
                                entityId = "P" + to_string(playerNum) + entityId;
                                // What is it? Circle, Square?
                                std::string entityType;
                                ssBuffer >> entityType;
                                if(!entityType.compare("CIRC"))
                                {
                                    LoadCircle(ssBuffer, entityId);

                                }
                                else if(!entityType.compare("SQR"))
                                {
                                    // LoadSqare(istream& stream)
                                }
                                
                            }
                        }                       
                    }
                }
                break;
            }
        }
    }
}


void CannBallNetworkManager::LoadCircle(istream& in, std::string& id)
{
    float x, y;

    char attr;
    in >> attr;
    if(attr == 'X')
    {
        in.ignore(1); // ignore the ':' character.
        in >> x >> y;
    }

    m_scene->AddCircle(x, y, 50.0f, id);
    
    // TODO: Call m_scene->AddCircle(position, radius)
}


void CannBallNetworkManager::LoadSquare(istream& in, std::string& id)
{
    float x, y;
    char attr;
    in >> attr;
    if(attr == 'X')
    {
        in.ignore(1);
        in >> x >> y;
    }

    // TODO: Call m_scene->AddSquare(position, radius)
}


void CannBallNetworkManager::GetPeerUpdates(int playerNum)
{
    SocketStream& peer = (playerNum == 1 ? m_player1 : m_player2);

    int timeoMs = 500;
    setsockopt(peer.GetHandle(), SOL_SOCKET, SO_RCVTIMEO, (char*)&timeoMs, sizeof(timeoMs));
    char buffer[1000];
    memset(buffer, 0, 1000);
    
    if(peer.Recv(buffer, 1000, 0) < 1)
    {
        // Nothing received... keep calm and carry on.
        return;
    }

    std::string strBuffer(buffer);
    std::stringstream bufferStream(strBuffer);

    std::string head;
    bufferStream >> head;
    // int circleNum = 0;
    while(!bufferStream.eof())
    {
        if(!head.compare("UPD"))
        {
            // ReadUpdateMessage
            std::string entID;
            bufferStream >> entID;
        
            std::string type;
            bufferStream >> type;
            if(!type.compare("CIRC"))
            {
                // Update "entID" circle.
                float x, y;
                char attr;
                bufferStream >> attr;
                if(attr == 'X')
                { 
                    // Update position.
                    bufferStream.ignore(1); // Ignore ":"
                    bufferStream >> x >> y;
                    entID = "P" + to_string(playerNum) + entID;
                    Entity* ent = m_scene->GetEntity(entID);
                    if(ent)
                    {
                        ent->SetPos(glm::vec3(x, y, 0.0f));
                    }
                }
            }
            else if(!type.compare("SQR"))
            {
                // TODO: Square stuff.
                // Same as circle I think?
            }

            bufferStream >> head; // Get next entID, (if there is one...)
        }
    }
}


int CannBallNetworkManager::run()
{
    // TODO: 1. Search for connection via broadcaster.
    EstablishPeerConnection(1); // DONE ^^
    EstablishPeerConnection(2);
    
    while(true)
    {
        if(isFinishing())
            break;
        GetPeerUpdates(1);
        GetPeerUpdates(2);
    }
    // TODO: 3. If receive fails, send check... maybe attempt to reconnect? 
    return 1;
}
