//===================================================================================================================================
//【network.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/12/05
// [更新日]2019/12/05
//===================================================================================================================================
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//===================================================================================================================================
//【ライブラリのロード】
//===================================================================================================================================
#include "Base.h"
#include "networkServer.h"

#include <WinSock.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")


//===================================================================================================================================
//【定数定義】
//===================================================================================================================================
//const char szServer[64] = "ths80619";				//菅野PC
//const char szServer[64] = "DESKTOP-2C7Q2ME";		//中込PC
//const char szServer[64] = "ths80214";				//伊達PC
//const char szServer[64] = "INDES";					//サイ

//===================================================================================================================================
//【クライアント】
//===================================================================================================================================
class NETWORK_CLIENT
{
//Method
public:
	NETWORK_CLIENT();
	~NETWORK_CLIENT();

	void send(float time);
	void outputGUI();
	static void setSendTreeTable(const TreeTable inTreeTable);
	void resetDisplay();

//Data
public:
	static bool requestConnection;//接続要求
	static bool initialConnection;//初期接続
	static int connectionTarget;//接続先
	static bool success;
	bool onResetDisplay;
private:
	int packageID;
	WSADATA wsaData;
	SOCKET s;
	SOCKADDR_IN addrin;
	HOSTENT *lpHostEnt;
	unsigned int addr;
	int nRtn, nNo = 5;

	static TreeTable treeTable[20];
	static int treeNum;
};
