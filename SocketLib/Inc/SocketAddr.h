#pragma once
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include <string>

class SocketAddr
{
public:
    SocketAddr();
    SocketAddr(u_long addr, u_short port);
    SocketAddr(char* addr, u_short port);
    ~SocketAddr(void);

    u_short GetPort()   { return ntohs(m_addr.sin_port); }
    u_long GetIpAddr()  { return ntohl(m_addr.sin_addr.s_addr); }
    sockaddr* GetAddr() const { return (sockaddr*)&m_addr; }
    void SetAddr(sockaddr_in& newAddr) { m_addr = newAddr; }

    std::string ToString();

private:
    sockaddr_in m_addr;
};

