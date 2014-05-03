#include "../Inc/SocketDgram.h"
#include <fstream>

extern ofstream debugOut;

bool SocketDgram::Open(u_short localPort)
{
    SetHandle(socket(AF_INET, SOCK_DGRAM, 0));
    if(!IsOk())
    {
        debugOut << "Open FAILED. Socket creation failed." << endl;
    }

    // Bind to local address
    if(localPort)
    {
        SocketAddr localAddr(INADDR_ANY, localPort);
        if(bind(GetHandle(), localAddr.GetAddr(), sizeof(*localAddr.GetAddr()))
            == SOCKET_ERROR)
        {
            debugOut << "Binding socket to local port FAILED: ERROR " << WSAGetLastError() << endl;
            return false;
        }
    }

    return true;
}

int SocketDgram::SendTo(const SocketAddr& peerAddr, const char* buffer,
                         int size, int flags)
{
    int bytesSent = sendto(GetHandle(), buffer, size, flags, 
                           peerAddr.GetAddr(), sizeof(*peerAddr.GetAddr()));

    if(bytesSent == SOCKET_ERROR)
    {
        debugOut << "SendTo FAILED. ERROR " << WSAGetLastError() << endl;
    }
    else
    {
         //debugOut << "Sending " << bytesSent << "bytes \'" << buffer << "\'" << endl; 
    }

    return bytesSent;
}

int SocketDgram::RecvFrom(char* buffer, int size, int flags, SocketAddr& fromOut)
{
    sockaddr_in from;
    int fromSize = sizeof(from);
    int bytesRcvd = recvfrom(GetHandle(), buffer, size, flags, (sockaddr*)&from, &fromSize);

    if(bytesRcvd == SOCKET_ERROR)
    {
        debugOut << "RecvFrom FAILED. ERROR " << WSAGetLastError() << endl;
    }
    else
    {
       // debugOut << "Recieving " << bytesRcvd << "bytes \'" << buffer << "\'" << endl;
        fromOut.SetAddr(from);
    }

    return bytesRcvd;
}
