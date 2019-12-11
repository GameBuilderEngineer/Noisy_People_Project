//network.h
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "base.h"
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

const u_short port = 12345;

typedef struct
{
	D3DXVECTOR3 tmpPos;
	int mun;
}PACKAGE;

class NETWORK_INTERFACE
{
public:
	NETWORK_INTERFACE();
	~NETWORK_INTERFACE();

	bool updata();

private:
	WSADATA wsaData;
	SOCKET s;
	SOCKADDR_IN addrin;
	SOCKADDR_IN from;
	int fromlen;
	int nRtn;

	bool loop;

};
