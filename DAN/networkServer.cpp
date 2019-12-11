//network.cpp

#include "networkServer.h"

NETWORK_INTERFACE::NETWORK_INTERFACE()
{
	// WSA
	WSAStartup(MAKEWORD(1, 1), &wsaData);

	// Socket
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0) 
	{
		WSACleanup();
	}

	// Addrin
	memset(&addrin, 0, sizeof(addrin));
	addrin.sin_port = htons(port);
	addrin.sin_family = AF_INET;
	addrin.sin_addr.s_addr = htonl(INADDR_ANY);

	// Bind
	nRtn = bind(s, (LPSOCKADDR)&addrin, (int)sizeof(addrin));
	if (nRtn == SOCKET_ERROR) 
	{
		closesocket(s);
		WSACleanup();
	}

	loop = true;

	// 
	u_long val = 1;
	ioctlsocket(s, FIONBIO, &val);

}

NETWORK_INTERFACE::~NETWORK_INTERFACE()
{
	// Uninit
	loop = false;
	closesocket(s);
	WSACleanup();
}

PACKAGE NETWORK_INTERFACE::updata()
{
	//while (loop)
	//{
		// buf
	char *buf;
	buf = (char *)malloc(sizeof(PACKAGE));

	fromlen = (int)sizeof(from);
	nRtn = recvfrom(s,
		buf,
		(int)sizeof(PACKAGE),
		0,
		(SOCKADDR *)&from,
		&fromlen);

	PACKAGE tmp;
	memset(&tmp, 0, sizeof(PACKAGE));
	if (nRtn != 0)
	{
		return tmp;
	}

	memcpy(&tmp, buf, sizeof(PACKAGE));
	free(buf);
	return tmp;
	//}
}