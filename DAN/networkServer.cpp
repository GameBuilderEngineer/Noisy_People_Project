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
	char *buf1;
	char *buf2;
	//buf = (char *)malloc();

	fromlen = (int)sizeof(from);

	int sizeOfPackage = sizeof(PACKAGE) - (sizeof(TreeTable*));
	nRtn = recvfrom(s,
		buf1,
		(int)sizeof(sizeOfPackage),
		0,
		(SOCKADDR *)&from,
		&fromlen);
	
	PACKAGE tmp;
	memset(&tmp, 0, sizeof(PACKAGE));
	if (nRtn != sizeof(sizeOfPackage))
	{
		return tmp;
	}
	memcpy(&tmp, buf1, sizeof(PACKAGE));
	free(buf1);


	int sizeOfTreeTable = sizeof(TreeTable)*tmp.treeMax;
	if (tmp.treeMax > 0)
	{
		int x = 0;
	}
	nRtn = recvfrom(s,
		buf2,
		(int)sizeof(sizeOfTreeTable),
		0,
		(SOCKADDR *)&from,
		&fromlen);
	if (nRtn != sizeof(PACKAGE))
	{
		return tmp;
	}
	tmp.treeTable = (TreeTable*)malloc(sizeOfTreeTable);
	memcpy(tmp.treeTable, buf2, sizeof(sizeOfTreeTable));
	free(buf2);

	return tmp;






	//}
}