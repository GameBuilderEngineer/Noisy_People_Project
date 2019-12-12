//===================================================================================================================================
//Åynetwork.hÅz
// [çÏê¨é“]HALìåãûGP12A332 16 ‰Ô óFçÑ
// [çÏê¨ì˙]2019/12/05
// [çXêVì˙]2019/12/05
//===================================================================================================================================
#include "networkClient.h"
#include "ImguiManager.h"

TreeTable *NETWORK_CLIENT::treeTable = nullptr;
int NETWORK_CLIENT::treeNum = 0;

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

	packageID = 0;
}

//===================================================================================================================================
//ÅyÉfÉXÉgÉâÉNÉ^Åz
//===================================================================================================================================
NETWORK_CLIENT::~NETWORK_CLIENT()
{
	closesocket(s);
	WSACleanup();
	if (treeNum != 0)
	{
		SAFE_DELETE_ARRAY(treeTable);
	}
}

//===================================================================================================================================
//ÅyëóêMÅz
//===================================================================================================================================
void NETWORK_CLIENT::send(float time)
{
	// buf
	packageID++;
	PACKAGE tmpPackage;
	memset(&tmpPackage, 0, sizeof(tmpPackage));
	tmpPackage.num = 10;
	tmpPackage.tmpPos = D3DXVECTOR3(100, 200, 300);
	tmpPackage.networkTester = true;
	tmpPackage.treeMax = treeNum;
	tmpPackage.treeTable = treeTable;
	tmpPackage.timer = time;
	char *buf1;
	char *buf2;
	int sizeOfPackage = sizeof(PACKAGE) - (sizeof(TreeTable*));
	int sizeOfTreeTable = sizeof(TreeTable)*tmpPackage.treeMax;
	buf1 = (char *)malloc(sizeOfPackage);
	memcpy(buf1, &tmpPackage, sizeof(sizeOfPackage));
	memcpy(buf2, tmpPackage.treeTable, sizeof(sizeOfTreeTable));

	nRtn = sendto(s, buf1, (int)sizeof(sizeOfPackage), 0,
		(LPSOCKADDR)&addrin, sizeof(addrin));
	nRtn = sendto(s, buf2, (int)sizeof(sizeOfTreeTable), 0,
		(LPSOCKADDR)&addrin, sizeof(addrin));

	free(buf1);
	free(buf2);
	SAFE_DELETE_ARRAY(treeTable);
	treeNum = 0;
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

//===================================================================================================================================
//ÅyëóêMÅz
//===================================================================================================================================
void NETWORK_CLIENT::setSendTreeTable(const TreeTable inTreeTable)
{
	if (treeNum == NULL)
	{
		treeTable = new TreeTable;
		memcpy(treeTable, &inTreeTable, sizeof(TreeTable));
	}
	else
	{
		TreeTable *tmpTreeTable;
		tmpTreeTable = new TreeTable[treeNum];
		memcpy(tmpTreeTable, treeTable, sizeof(TreeTable)*treeNum);
		SAFE_DELETE_ARRAY(treeTable);
		treeTable = new TreeTable[treeNum + 1];
		memcpy(treeTable, tmpTreeTable, sizeof(TreeTable)*treeNum);
		SAFE_DELETE_ARRAY(tmpTreeTable);
		memcpy(&treeTable[treeNum], &inTreeTable, sizeof(TreeTable));
	}

	treeNum++;
}