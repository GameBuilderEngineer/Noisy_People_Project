//===================================================================================================================================
//【network.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/12/05
// [更新日]2019/12/05
//===================================================================================================================================
#include "networkClient.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
NETWORK_CLIENT::NETWORK_CLIENT()
{
	// WSA
	nRtn = WSAStartup(MAKEWORD(1, 1), &wsaData);

	// Socket
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		WSACleanup();
	}

	// IPHost
	lpHostEnt = (HOSTENT *)gethostbyname(szServer);
	if (lpHostEnt == NULL) {
		addr = inet_addr(szServer);
		lpHostEnt = (HOSTENT *)gethostbyaddr((char *)&addr, 4, AF_INET);
	}

	memset(&addrin, 0, sizeof(addrin));
	memcpy(&(addrin.sin_addr),
		lpHostEnt->h_addr_list[0],
		lpHostEnt->h_length);
	addrin.sin_port = htons(port);
	addrin.sin_family = AF_INET;
	addrin.sin_addr.s_addr = *((unsigned long *)lpHostEnt->h_addr);
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
NETWORK_CLIENT::~NETWORK_CLIENT()
{
	closesocket(s);
	WSACleanup();
}

//===================================================================================================================================
//【送信】
//===================================================================================================================================
void NETWORK_CLIENT::send()
{
	// buf
	PACKAGE tmpPackage;
	tmpPackage.mun = 10;
	tmpPackage.tmpPos = D3DXVECTOR3(100, 200, 300);

	char *buf;
	buf = (char *)malloc(sizeof(PACKAGE));
	memcpy(buf, &tmpPackage, sizeof(PACKAGE));

	nRtn = sendto(s, buf, (int)sizeof(PACKAGE), 0,
		(LPSOCKADDR)&addrin, sizeof(addrin));

	free(buf);
}