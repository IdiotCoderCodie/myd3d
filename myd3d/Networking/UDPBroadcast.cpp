#include "UDPBroadcast.h"
#include "NET_CONSTS.h"

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
    
    for(int i = 0; i < m_attempts; i++)
    {
        SocketAddr peerAnyAddr(INADDR_BROADCAST, m_port);
        cout << "Sending UDP Broadcast..." << endl;
        if(m_socket.SendTo(peerAnyAddr, m_message.c_str(), m_message.size(), 0) == INVALID_SOCKET)
        {
            cout << "Sending UDP Broadcast FAILED." << endl;
            continue;
        }

        char response[100];
        memset(response, 0, 100);

        cout << "Waiting for response... " << endl;

        // Set timeout of .25second on the recieve.
        int timeoSecs = 250;
        setsockopt(m_socket.GetHandle(), SOL_SOCKET, SO_RCVTIMEO, (char*)&timeoSecs, sizeof(timeoSecs));
        if(m_socket.RecvFrom(response, 100, 0, m_peerAddr) > 0)
        {
            m_foundPeer = true;
            cout << "Read response: \'" << response << "\'" << endl;
            cout << "From: " << m_peerAddr.ToString() << endl;
            return 1;
        }
        else
        {
            cout << "Failed to get a response from Broadcast." << endl;
            continue; // Try again (if we have more attempts remaining)
        }
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