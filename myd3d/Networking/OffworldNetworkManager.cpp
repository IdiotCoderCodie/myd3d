#include "OffworldNetworkManager.h"
#include "NET_CONSTS.h"
#include "UDPBroadcast.h"

#include <sstream>

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
    
    // TODO: REFACTOR THIS SHIT!
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

                // TODO: Connected to player. Now read initialization!

                char recvBuffer[2000];
                memset(recvBuffer, 0, 2000);
                if(m_peer.Recv(recvBuffer, 2000, 0) > 0)
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
                                 
                                // What is it? Circle, Square?
                                std::string entityType;
                                ssBuffer >> entityType;
                                if(!entityType.compare("CIRC"))
                                {
                                    // LoadCircle(istream& stream)

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


void LoadCircle(istream& in)
{
    float x, y;

    char attr;
    in >> attr;
    if(attr == 'X')
    {
        in.ignore(1); // ignore the ':' character.
        in >> x >> y;
    }

    // TODO: Call m_scene->AddCircle(position, radius)
}


void LoadSquare(istream& in)
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


int OffworldNetworkManager::run()
{
    // TODO: 1. Search for connection via broadcaster.
    EstablishPeerConnection(); // DONE ^^

    // TODO: 2. Receive data from peer. COPY FROM OTHER PROJECT.
    while(true)
    {
        int timeoMs = 1000;
        setsockopt(m_peer.GetHandle(), SOL_SOCKET, SO_RCVTIMEO, (char*)&timeoMs, sizeof(timeoMs));
        char buffer[1000];
        memset(buffer, 0, 1000);

        if(m_peer.Recv(buffer, 1000, 0) < 1)
        {
            // Nothing received... keep calm and carry on.
            continue;
        }

        std::string strBuffer(buffer);
        std::stringstream bufferStream(strBuffer);

        std::string type;
        bufferStream >> type;
        int circleNum = 0;
        while(!bufferStream.eof())
        {
            if(!type.compare("CIRC"))
            {
                // Tis a circle.
                // Ignore next char...
                bufferStream.ignore(1);

                std::string attr;
                char attrCh[20];
                bufferStream.get(attrCh, 4, ':');
                attr = std::string(attrCh);
                if(!attr.compare("X"))
                {
                    // position...
                    // Ignore next character ':'
                    bufferStream.ignore(1);
                    float x, y;
                    bufferStream >> x >> y;

                    // TODO: Add this data to vector or some shit.
                    if(m_circles.size() > circleNum)
                    {
                        m_circles[circleNum].position = glm::vec2(x, y);
                    }
                    else
                    {
                        m_circles.push_back(NetCircle());
                        m_circles[circleNum].position = glm::vec2(x, y);
                    }
                    circleNum++;
                }
            }
            else if(!type.compare("SQRE"))
            {
                // Tis a square.
            }

            bufferStream >> type;
        }
    }
    // TODO: 3. If receive fails, send check... maybe attempt to reconnect? 

    while(true)
    { // Stop thread from exiting for now....
        Sleep(10000);
    }
    return 1;
}
