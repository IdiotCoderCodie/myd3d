#include "../Inc/Socket.h"
#include <fstream>

extern ofstream debugOut;

void Socket::Close()
{
    debugOut << "Closing Socket. Local [" << GetLocalAddr().ToString() << "], Peer [" <<
        GetPeerAddr().ToString() << "]." << endl;

    shutdown(GetHandle(), SD_SEND);
    closesocket(GetHandle());
    m_hSocket = INVALID_SOCKET;
}
//bool Socket::Connect(sockaddr_in& peer, unsigned short localPort)
//{
//    if (!m_isOk)
//    {
//        debugOut << "Connection FAILED. Socket creation failed. " << endl;
//        return false;
//    }
//
//    if (connect(m_socket, (sockaddr*)&peer, sizeof(peer)) == SOCKET_ERROR)
//    {
//        debugOut << " Connection FAILED with " << WSAGetLastError() << endl;
//        return false;
//    }
//
//    debugOut << "Opening socket. Local: [" << inet_ntoa(GetLocalAddr().sin_addr) << ':'
//             << GetLocalAddr().sin_port << "]. Peer: [" << inet_ntoa(GetPeerAddr().sin_addr)
//             << ':' << GetPeerAddr().sin_port << "]." << endl;
//
//    return true;
//}


//int Socket::Send(const char* buffer, int size, int flags)
//{
//    int bytesSent = send(m_socket, buffer, size, flags);
//    if (bytesSent == SOCKET_ERROR)
//    {
//        // HANDLE ERROR. 
//        debugOut << "Send FAILED with " << WSAGetLastError() << endl;
//    }
//    else
//    {
//        debugOut << "Sending " << bytesSent << " bytes: " << buffer << endl;
//    }
//
//    return bytesSent;
//}


//int Socket::Recv(char* buffer, int size, int flags)
//{
//    int bytesRead = recv(m_socket, buffer, size, flags);
//    if (bytesRead == SOCKET_ERROR)
//    {
//        debugOut << "Recv FAILED with " << WSAGetLastError() << endl;
//    }
//    else
//    {
//        debugOut << "Received " << bytesRead << " bytes: " << buffer << endl;
//    }
//
//    return bytesRead;
//}

void Socket::SetHandle(SOCKET handle)
{
    if(IsOk())
    {
        Close();
    }

    m_hSocket = handle;
}


SocketAddr Socket::GetLocalAddr() const
{
    SocketAddr out;
    int size = sizeof(*out.GetAddr());
    getsockname(m_hSocket, out.GetAddr(), &size);
    return out;
}

SocketAddr Socket::GetPeerAddr() const
{
    SocketAddr out;
    int size = sizeof(*out.GetAddr());
    getpeername(m_hSocket, out.GetAddr(), &size);
    return out;
}