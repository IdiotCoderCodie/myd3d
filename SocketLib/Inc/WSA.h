#pragma once
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>

class WSA
{
public:
	WSA();
	~WSA();

    bool isOk() { return m_isOk; }

private:
	bool m_isOk;
	WSADATA m_wsaData;
};

