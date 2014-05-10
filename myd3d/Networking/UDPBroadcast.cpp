#include "UDPBroadcast.h"
#include "NET_CONSTS.h"
#include <sstream>

UDPBroadcast::UDPBroadcast(void)
     : m_socket() ,
     m_foundPeer(false),
     m_message(to_string(CONNECT_MAGIC_NUM)),
     m_port(BROADCAST_PORT),
     m_attempts(1)
{
}


UDPBroadcast::~UDPBroadcast(void)
{
}


int UDPBroadcast::run()
{
    // TODO: Edit this broadcast to keep broadcasting until it gets a response (or is terminated).
    cout << "Creating UDP broadcast..." << endl;

    if(!m_socket.Open(0))
    {
        cout << "ERROR opening UDP socket." << endl;
        return 0;
    }

    char optVal = 1;
    setsockopt(m_socket.GetHandle(), SOL_SOCKET, SO_BROADCAST, &optVal, sizeof(optVal));
    
    char rcvBuff[100000];

    for(int i = 0; i < m_attempts; i++)
    {
        SocketAddr peerAnyAddr(INADDR_BROADCAST, m_port);
        cout << "Sending UDP Broadcast..." << endl;
        if(m_socket.SendTo(peerAnyAddr, m_message.c_str(), m_message.size(), 0) == INVALID_SOCKET)
        {
            cout << "Sending UDP Broadcast FAILED." << endl;
            continue;
        }

        cout << "Waiting for response... " << endl;

        SocketDgram dgramReceive;
        dgramReceive.Open(BROADCAST_PORT_AUX);
        int timeoSecs = 50;
        setsockopt(dgramReceive.GetHandle(), SOL_SOCKET, SO_RCVTIMEO, (char*)&timeoSecs, sizeof(timeoSecs));
      //  std::string t = m_socket.GetLocalAddr().ToString();
        //cout << t << endl;    

        bool echoMatch = false;
        //while(!echoMatch)
        //{
        memset(rcvBuff, 0, 100000);
        for(int attempt = 0; attempt < 10; attempt++) // look for our message amongst any background crap.
        {
            int bytesRcvd = dgramReceive.RecvFrom(rcvBuff, 100000, 0, m_peerAddr);
            if(bytesRcvd > 0)
            {
                cout << "Rcvd " << bytesRcvd << " from " << m_peerAddr.ToString();

                if(!strcmp(rcvBuff, m_message.c_str()))
                {
                    // Match
                    cout << "Found peer, received same message sent!" << endl;
                    echoMatch = true;
                    m_foundPeer = true;
                    return 0;
                }
                else
                {
                    cout << "No match... trying again?" << endl;
                } 
            }
        }
        //}

        int bytesRcvd = 0;

        //while(true)
        //{
        //    while(bytesRcvd < 1)
        //    {
        //        bytesRcvd = m_socket.RecvFrom(response, 100000, 0, m_peerAddr);;
        //    }

        //    std::string recvString(response);
        //    if(!recvString.compare(m_message))
        //    {
        //        // Echo successful! Found a peer.
        //        m_foundPeer = true;
        //        cout << "Read response: \'" << response << "\'" << endl;
        //        cout << "From: " << m_peerAddr.ToString() << endl;
        //        break;
        //        return 0;
        //    }           
        //}
        //if(bytesRcd > 0)
        //{
        //    
        //    return 1;
        //}
        //else
        //{
        //    cout << "Failed to get a response from Broadcast." << endl;
        //    continue; // Try again (if we have more attempts remaining)
        //}
    }

    return 0;
    
}


bool UDPBroadcast::GetPeerAddr(SocketAddr& out)
{
    if(m_foundPeer)
    {
        out = m_peerAddr;
        return true;
    }
    
    return false;
}