#include "../Inc/SocketListener.h"
#include <fstream>

extern ofstream debugOut;
SocketListener::SocketListener(void)
{
}


bool SocketListener::Open(const SocketAddr& localAddr)
{
    SetHandle(socket(AF_INET, SOCK_STREAM, 0));
    if(!IsOk())
    {
        debugOut << "Open SocketListener FAILED Creating socket FAILED. ERROR " << WSAGetLastError() << endl;
        return false;
    }
    
    if(bind(GetHandle(), localAddr.GetAddr(), sizeof(*localAddr.GetAddr()))
        == SOCKET_ERROR)
    {
        debugOut << "Binding socket to local port FAILED. ERROR " << WSAGetLastError() << endl;
        return false;
    }

    debugOut << "Opened listeneing socket. Local [" << GetLocalAddr().ToString() << "]. Peer ["
             << GetPeerAddr().ToString() << "]." << endl;

    return true;
}

bool SocketListener::Listen()
{
    if(listen(GetHandle(), 5) == SOCKET_ERROR)
    {
        debugOut << "Listening FAILED. ERROR " << WSAGetLastError() << endl;
        return false;
    }

    debugOut << "Listening... Port " << ntohs(GetLocalAddr().GetPort()) << endl;
    return true;
}
    
bool SocketListener::Accept(SocketStream& streamOut)
{
    if(!IsOk())
    {
        debugOut << "Accept FAILED. Socket not valid." << endl;
        return false;
    }
    streamOut.SetHandle(accept(GetHandle(), NULL, NULL));
    
    if(streamOut.IsOk())
    {
        debugOut << "Opening Socket Stream. Local [" << streamOut.GetLocalAddr().ToString() <<
            "]. Peer [" << streamOut.GetPeerAddr().ToString() << "]." << endl;
    }
    else
    {
        debugOut << "Opening Socket Stream FAILED. Local [" << streamOut.GetLocalAddr().ToString() <<
            "]. Peer [" << streamOut.GetPeerAddr().ToString() << "]." << endl;
    }

    return true;
}