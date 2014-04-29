#include "BroadcastReceiver.h"
#include "NET_CONSTS.h"

BroadcastReceiver::BroadcastReceiver(void)
    : m_socket(),
    m_port(BROADCAST_PORT),
    m_foundClient(false),
    m_clientAddr(),
    m_key(std::to_string(CONNECT_MAGIC_NUM))
{
}


BroadcastReceiver::~BroadcastReceiver(void)
{
}

int BroadcastReceiver::run()
{
    if(!m_socket.Open(m_port))
    {
        cout << "Error opening broadcast receiver socket." << WSAGetLastError() <<  endl;
        return 0;
    }

    cout << "Opened broadcast receiver" << endl;

    SocketAddr m_clientAddr;
    int addrSize = sizeof(m_clientAddr);
    char buffer[1000];
    m_foundClient = false;
    // Keep receiving broadcasts until one appears with header we're interested in.
    while(m_foundClient == false)
    {
        memset(buffer, 0, 1000);   
    
        int bytesRcvd = m_socket.RecvFrom(buffer, 1000, 0, m_clientAddr);
        if(bytesRcvd > 0)
        {
            cout << "Recieved \'" << buffer << "\'" << endl;
            cout << "From: " << m_clientAddr.ToString() << endl;

            // Check if received matches key listening for..
            if(strcmp(m_key.c_str(), buffer))
            {
                // Don't match... move on.
                cout << "Received doesn't match key... Continuing to check for further broadcasts." << endl;
            }
            else
            {
                cout << "Received message matches key!" << endl;

                // Send reply... will be setting up listener for you to connect to...
                int bytesSent = m_socket.SendTo(m_clientAddr, "Hello", 5, 0);
                if(bytesSent > 0)
                {
                    cout << "Sent " << bytesSent << "bytes." << endl;
                }
                m_foundClient = true;
            }
        }
    }
    return 1;
}


bool BroadcastReceiver::GetPeerAddr(SocketAddr& out)
{
    if(m_foundClient)
    {
        out = m_clientAddr;
        return true;
    }

    return false;
}