//===================================================================================================================================
//【networkClient.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/12/05
// [更新日]2019/12/05
//===================================================================================================================================
#include "networkClient.h"
#include "ImguiManager.h"

TreeTable NETWORK_CLIENT::treeTable[TREE_TABLE_SIZE] = { 0 };
D3DXVECTOR3 NETWORK_CLIENT::pos1P = D3DXVECTOR3(0, 0, 0);
D3DXVECTOR3 NETWORK_CLIENT::pos2P = D3DXVECTOR3(0, 0, 0);
bool NETWORK_CLIENT::record1P = false;
bool NETWORK_CLIENT::record2P = false;
int NETWORK_CLIENT::treeNum = 0;
bool NETWORK_CLIENT::requestConnection = false;
bool NETWORK_CLIENT::initialConnection = true;
int NETWORK_CLIENT::connectionTarget = -1;
bool NETWORK_CLIENT::success = false;
//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace ServerListNS
{

	enum PC_LIST
	{
		KANNO,
		NAKAGOMI,
		DATE,
		SAI,
		SOMEYA,
		SHINZATO,
		PC_NUM
	};

	const char* SERVER_NAME_LIST[PC_NUM] = {
		{"ths80619"},			//KANNO,
		{"DESKTOP-2C7Q2ME"},	//NAKAGOMI
		{"ths80214"},			//DATE,
		{"INDES"},				//SAI,
		{"MSI"},				//SOMEYA,
		{"LAPTOP-74FA5EHG"},	//SHINZATO
	};
}

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace ServerListNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
NETWORK_CLIENT::NETWORK_CLIENT()
{
	success = false;
	onResetDisplay = false;
	//接続要求に応じる
	if (!requestConnection)return;//接続要求がない
	//---------
	//共通処理
	//---------
	// WSA
	nRtn = WSAStartup(MAKEWORD(1, 1), &wsaData);

	// Socket
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		WSACleanup();
	}

	switch (initialConnection)
	{

	//---------
	//初期接続
	//---------
	case true:
		//接続テスト
		// IPHost
		for (int i = 0; i < PC_NUM; i++)
		{
			//接続先が選択されている場合はテストしない
			if (connectionTarget != -1)continue;
			//SERVER_NAMEをホスト名として接続を試みる
			lpHostEnt = (HOSTENT *)gethostbyname(SERVER_NAME_LIST[i]);
			if (lpHostEnt != NULL)
			{
				//接続成功
				TCHAR confirm[MAX_PATH + 1];
				sprintf(confirm, "このホストと接続を行いますか？\n%s",SERVER_NAME_LIST[i]);
				if (MessageBox(0, confirm, "確認", MB_YESNO| MB_TOPMOST) == IDYES)
				{
					connectionTarget = i;
				}
			}
			else {
				//ホスト名による接続に失敗したので
				//SERVER_NAMEをIPアドレスとして接続を試みる
				addr = inet_addr(SERVER_NAME_LIST[i]);
				if (addr != INADDR_NONE)
				{
					lpHostEnt = (HOSTENT *)gethostbyaddr((char *)&addr, 4, AF_INET);
				}
				if (lpHostEnt != NULL)
				{
					//接続成功
					TCHAR confirm[MAX_PATH + 1];
					sprintf(confirm, "このIPアドレスと接続を行いますか？\n%s", SERVER_NAME_LIST[i]);
					if (MessageBox(0, confirm, "確認", MB_YESNO | MB_TOPMOST) == IDYES)
					{
						connectionTarget = i;
					}
				}
			}
		}

		//接続先が選択されたかどうか
		if (connectionTarget == -1)
		{
			MessageBox(0,"インターネット接続に失敗しました。","確認",MB_OK|MB_TOPMOST);
		}
		else {
			TCHAR confirm[MAX_PATH + 1];
			sprintf(confirm, "インターネット接続に成功しました。\n接続先：%s", SERVER_NAME_LIST[connectionTarget]);
			MessageBox(0, confirm, "確認", MB_OK | MB_TOPMOST);
		}

		//初期接続終了
		initialConnection = false;

		break;

	//---------
	//実際の接続
	//---------
	case false:
		//接続先が選択されていない場合は接続しない
		if (connectionTarget == -1)return;

		// IPHost
		//SERVER_NAMEをホスト名として接続を試みる
		lpHostEnt = (HOSTENT *)gethostbyname(SERVER_NAME_LIST[connectionTarget]);
		if (lpHostEnt != NULL)
		{
			//接続成功
			success = true;
		}
		else {
			//ホスト名による接続に失敗したので
			//SERVER_NAMEをIPアドレスとして接続を試みる
			addr = inet_addr(SERVER_NAME_LIST[connectionTarget]);
			if (addr != INADDR_NONE)
			{
				lpHostEnt = (HOSTENT *)gethostbyaddr((char *)&addr, 4, AF_INET);
			}
			if (lpHostEnt != NULL)
			{
				//接続成功
				success = true;
			}
		}

		if (!success)
		{
			//失敗したので終了
			return;
		}

		//
		memset(&addrin, 0, sizeof(addrin));
		memcpy(&(addrin.sin_addr),
			lpHostEnt->h_addr_list[0],
			lpHostEnt->h_length);
		addrin.sin_port = htons(port);
		addrin.sin_family = AF_INET;
		addrin.sin_addr.s_addr = *((unsigned long *)lpHostEnt->h_addr);
	
		packageID = 0;

		break;
	}
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
void NETWORK_CLIENT::send(float time)
{
	//接続に失敗しているので、送信を行わない
	if (!success)return;
	
	// buf
	packageID++;
	PACKAGE tmpPackage;
	memset(&tmpPackage, 0, sizeof(tmpPackage));
	tmpPackage.num = 10;
	tmpPackage.pos1P = pos1P;
	tmpPackage.record1P = record1P;
	tmpPackage.pos2P = pos2P;
	tmpPackage.record2P = record2P;
	tmpPackage.networkTester = true;
	tmpPackage.treeMax = TREE_TABLE_SIZE;
	tmpPackage.sceneReset = onResetDisplay;
	onResetDisplay = false;
	if (tmpPackage.treeMax > 0)
	{
		memcpy(tmpPackage.treeTable, treeTable, sizeof(treeTable));
	}
	tmpPackage.timer = time;

	//PACKAGEの情報のみ
	char *buf;	//パッケージ用(PACKAGEの情報のみ)
	buf = (char *)malloc(sizeof(tmpPackage));
	memcpy(buf, &tmpPackage, sizeof(tmpPackage));
	nRtn = sendto(s, buf, sizeof(tmpPackage), 0,
		(LPSOCKADDR)&addrin, sizeof(addrin));
	free(buf);

	//TreeTableの実体
	memset(treeTable, 0, sizeof(treeTable));
	treeNum = 0;	//0クリア
	record1P = false;
	record2P = false;
}


//===================================================================================================================================
//【ディスプレイシーンのリセット信号のセット】
//===================================================================================================================================
void NETWORK_CLIENT::resetDisplay()
{
	onResetDisplay = true;
}

//===================================================================================================================================
//【GUI】
//===================================================================================================================================
void NETWORK_CLIENT::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("NetworkInformation"))
	{
		ImGui::Checkbox("initilaConnection", &initialConnection);
		ImGui::Checkbox("requestConnection", &requestConnection);
		ImGui::InputInt("connectionTarget", &connectionTarget);
	}
#endif
}

//===================================================================================================================================
//【プレイヤー位置情報の記録】
//===================================================================================================================================
void NETWORK_CLIENT::recordPlayerPosition(const D3DXVECTOR3 position, int playerNo)
{
	switch (playerNo)
	{
	case gameMasterNS::PLAYER_1P:
		pos1P = position;
		record1P = true;
		break;
	case gameMasterNS::PLAYER_2P:
		pos2P = position;
		record2P = true;
		break;
	}
}
//===================================================================================================================================
//【ツリーテーブルへの記録】
//===================================================================================================================================
void NETWORK_CLIENT::recordTreeTable(const TreeTable inTreeTable, int tableNo)
{
	//接続に失敗しているので、記録を行わない
	if (!success)return;
	if (tableNo >= TREE_TABLE_SIZE)return;
	treeTable[tableNo] = inTreeTable;
}

//===================================================================================================================================
//【送信】
//===================================================================================================================================
void NETWORK_CLIENT::setSendTreeTable(const TreeTable inTreeTable)
{
	//接続に失敗しているので、送信を行わない
	if (!success)return;

	treeTable[treeNum] = inTreeTable;

	treeNum++;
}