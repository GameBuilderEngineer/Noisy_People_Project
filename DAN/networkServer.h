//network.h
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "base.h"
#include <winsock.h>
#include "GameMaster.h"

#pragma comment(lib, "ws2_32.lib")

const u_short port = 12345;

//===================================================================================================================================
//Åyç\ë¢ëÃÅz
//===================================================================================================================================
typedef struct
{
	bool networkTester;
	int num;
	D3DXVECTOR3 pos1P; 
	bool record1P;
	D3DXVECTOR3 pos2P;
	bool record2P;
	float timer;
	int treeMax;
	bool sceneReset;
	TreeTable treeTable[300];
}PACKAGE;

//ÉNÉâÉX
class NETWORK_INTERFACE
{
public:
	NETWORK_INTERFACE();
	~NETWORK_INTERFACE();

	PACKAGE *updata();

private:
	WSADATA wsaData;
	SOCKET s;
	SOCKADDR_IN addrin;
	SOCKADDR_IN from;
	int fromlen;
	int nRtn;

	bool loop;

};
