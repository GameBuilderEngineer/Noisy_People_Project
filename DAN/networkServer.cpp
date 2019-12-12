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
	//PACKAGE‚Ìî•ñ‚Ì‚Ý
	const int sizeOfPackage = sizeof(PACKAGE) - (sizeof(TreeTable*));
	char *buf1;
	buf1 = (char *)malloc(sizeOfPackage);
	fromlen = (int)sizeof(from);	
	//TreeTable‚ÌŽÀ‘Ì(ŽóM)
	nRtn = recvfrom(s,
		buf1,
		sizeOfPackage,
		0,
		(SOCKADDR *)&from,
		&fromlen);
	
	PACKAGE tmp;
	memset(&tmp, 0, sizeof(PACKAGE));
	if (nRtn != sizeOfPackage)
	{
		return tmp;
	}
	else
	{
		int x = 0;
	}
	memcpy(&tmp, buf1, sizeOfPackage);
	free(buf1);

	if (tmp.treeMax > 0)
	{
		int sizeOfTreeTable = sizeof(TreeTable)*tmp.treeMax;
		char *buf2;
		buf2 = (char *)malloc(sizeOfTreeTable);
		if (tmp.treeMax > 0)
		{
			int x = 0;
		}
		nRtn = recvfrom(s,
			buf2,
			sizeOfTreeTable,
			0,
			(SOCKADDR *)&from,
			&fromlen);
		if (nRtn != sizeOfTreeTable)
		{
			return tmp;
		}
		tmp.treeTable = (TreeTable*)malloc(sizeOfTreeTable);
		memcpy(tmp.treeTable, buf2, sizeOfTreeTable);
		free(buf2);
	}

	return tmp;
}