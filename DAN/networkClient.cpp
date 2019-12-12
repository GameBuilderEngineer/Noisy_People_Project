//===================================================================================================================================
//Åynetwork.hÅz
// [çÏê¨é“]HALìåãûGP12A332 16 ‰Ô óFçÑ
// [çÏê¨ì˙]2019/12/05
// [çXêVì˙]2019/12/05
//===================================================================================================================================
#include "networkClient.h"
#include "ImguiManager.h"

//===================================================================================================================================
//ÅyÉRÉìÉXÉgÉâÉNÉ^Åz
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
//ÅyÉfÉXÉgÉâÉNÉ^Åz
//===================================================================================================================================
NETWORK_CLIENT::~NETWORK_CLIENT()
{
	closesocket(s);
	WSACleanup();
}

//===================================================================================================================================
//ÅyëóêMÅz
//===================================================================================================================================
void NETWORK_CLIENT::send(float time)
{
	// buf
	PACKAGE tmpPackage;
	tmpPackage.mun = 10;
	tmpPackage.tmpPos = D3DXVECTOR3(100, 200, 300);
	tmpPackage.networkTester = true;
	tmpPackage.timer = time;
	char *buf;
	buf = (char *)malloc(sizeof(PACKAGE));
	memcpy(buf, &tmpPackage, sizeof(PACKAGE));

	nRtn = sendto(s, buf, (int)sizeof(PACKAGE), 0,
		(LPSOCKADDR)&addrin, sizeof(addrin));

	free(buf);
}

//===================================================================================================================================
//ÅyGUIÅz
//===================================================================================================================================
void NETWORK_CLIENT::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("NetworkInformation"))
	{
		ImGui::Text("Server Name = %s", szServer);
	}
#endif
}