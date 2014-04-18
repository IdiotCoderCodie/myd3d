#include "../Inc/SocketAddr.h"
#include <sstream>

SocketAddr::SocketAddr(void)
{
    memset(&m_addr, 0, sizeof(m_addr));
}

SocketAddr::SocketAddr(u_long ipAddr, u_short port)
{
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    m_addr.sin_addr.S_un.S_addr = htonl(ipAddr);
}

SocketAddr::SocketAddr(char* ipAddr, u_short port)
{
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    m_addr.sin_addr.S_un.S_addr = inet_addr(ipAddr);
}


SocketAddr::~SocketAddr(void)
{
}

std::string SocketAddr::ToString()
{
    std::ostringstream oss;
    // Output: ipAddr:port
    oss << inet_ntoa(m_addr.sin_addr) << ':' << ntohs(m_addr.sin_port);
    return oss.str();
}
