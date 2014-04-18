#include "../Inc/SocketStream.h"
#include <fstream>

extern ofstream debugOut;
bool SocketStream::Open(const SocketAddr& peerAddr, u_short localPort)
{
    SetHandle(socket(AF_INET, SOCK_STREAM, 0));
    if(!IsOk())
    {
        debugOut << "Open FAILED. Socket creation failed." << endl;
    }

    // Bind to local address.
    if(localPort)
    {
        SocketAddr localAddr(u_long(0), localPort);
        if(bind(GetHandle(), localAddr.GetAddr(), sizeof(*localAddr.GetAddr()))
            == SOCKET_ERROR)
        {
            debugOut << "Binding socket to local port FAILED: ERROR " << WSAGetLastError() << "." << endl;
            return false;
        }
    }

    // Connect to foreign port.
    if(connect(GetHandle(), peerAddr.GetAddr(), sizeof(peerAddr))
        == SOCKET_ERROR)    
    {
        debugOut << "Connection to peer FAILED. ERROR " << WSAGetLastError() << endl;
        return false;
    }

    debugOut << "Opening socket. Local [" << GetLocalAddr().ToString() << "], Peer [" <<
        GetPeerAddr().ToString() << "]" << endl;

    return true;
}

int SocketStream::Send(const char* buffer, int size, int flags)
{
    int bytesSent = send(GetHandle(), buffer, size, flags);
    if(bytesSent == SOCKET_ERROR)
    {
        // TODO: Handle error.
        debugOut << "Send FAILED. ERROR " << WSAGetLastError() << endl;
    }
    else
    {
        debugOut << "Sending " << bytesSent << "bytes \'" << buffer << "\'" << endl; 
    }

    return bytesSent;
}

int SocketStream::Recv(char* buffer, int size, int flags)
{
    int bytesRecvd = recv(GetHandle(), buffer, size, flags);
    if(bytesRecvd == SOCKET_ERROR)
    {
        debugOut << "Recv FAILED. ERROR " << WSAGetLastError() << endl;
    }
    else
    {
        debugOut << "Reading " << bytesRecvd << "bytes \'" << buffer << "\'." << endl;
    }

    return bytesRecvd;
}