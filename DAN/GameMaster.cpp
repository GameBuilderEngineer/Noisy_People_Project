//===================================================================================================================================
//【GameMaster.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/10/25
//===================================================================================================================================
#include "GameMaster.h"
#include "ImguiManager.h"
#include "Input.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace gameMasterNS
{
	Input* input = NULL;
}

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace gameMasterNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
GameMaster::GameMaster()
{
	gameTimer			= 0.0f;								//ゲーム時間
	countDownTimer		= 0.0f;								//カウントダウン時間
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		ZeroMemory(&playerInformation[i], sizeof(PlayerTable));
	}
	treeNum				= 0;
	//conversionOrder		= NULL;
	input				= getInput();
	gameTimerStop		= false;
#ifdef _DEBUG
	showGUI = true;
#endif // _DEBUG
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
GameMaster::~GameMaster()
{
	//SAFE_DELETE_ARRAY(conversionOrder);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void GameMaster::initialize()
{

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void GameMaster::update(float frameTime)
{
	//ゲーム中ならば、ポーズ切り替え受付
	if (gameTimer > 0)
	{
		//スタートボタン/Ｐキーを入力された場合ポーズにする。
		if (input->wasKeyPressed('P') || 
			input->getController()[0]->wasButton(virtualControllerNS::SPECIAL_MAIN)||
			input->getController()[1]->wasButton(virtualControllerNS::SPECIAL_MAIN))
		{
			pause = !pause;
		}
#ifdef _DEBUG
		//タッチパッド(SUB)ボタン/Qキーを入力された場合ゲームタイマーを停止。
		if (input->wasKeyPressed('Q') ||
			input->getController()[0]->wasButton(virtualControllerNS::SPECIAL_SUB) ||
			input->getController()[1]->wasButton(virtualControllerNS::SPECIAL_SUB))
		{
			gameTimerStop = !gameTimerStop;
		}

#endif // _DEBUG


	}
}

//===================================================================================================================================
//【ポーズ処理】
//===================================================================================================================================
bool GameMaster::paused()
{
	return pause;
}

//===================================================================================================================================
//【ゲーム開始時処理】
//===================================================================================================================================
void GameMaster::startGame()
{
	gameTimer		= GAME_TIME;
	gameTimerStop	= false;
	pause = false;
	progress = 0x00000000;
	discardConversionOrder();
}

//===================================================================================================================================
//【ゲーム時間の更新】
//===================================================================================================================================
void GameMaster::updateGameTime(float frameTime)
{
	if (gameTimerStop)return;

	gameTimer -= frameTime;
}

//===================================================================================================================================
//【残り時間1分の更新】
//===================================================================================================================================
bool GameMaster::playActionRamaining1Min()
{
	if (gameTimer > 60)return false;
	if (whetherAchieved(PASSING_REMAINING_ONE_MINUTE))return false;

	setProgress(PASSING_REMAINING_ONE_MINUTE);

	return true;
}


#pragma region conversionOrderFunction
//===================================================================================================================================
//【緑化した木の本数を記録】
//===================================================================================================================================
void GameMaster::recordTreeTable(TreeTable treeTable)
{
	TreeTable data = treeTable;
	data.eventTime = GAME_TIME - gameTimer;
	treeTableList.insertFront(data);
	treeTableList.listUpdate();
}

//===================================================================================================================================
//【変換順番変数を準備する】
//===================================================================================================================================
void GameMaster::setTreeNum(int num) 
{
	treeNum = num;
}

//===================================================================================================================================
//【変換順番変数を破棄する】
//===================================================================================================================================
void GameMaster::discardConversionOrder() {
	this->treeNum = 0;
	treeTableList.allClear();
}
#pragma endregion

//===================================================================================================================================
//【setter】
//===================================================================================================================================
void GameMaster::setConversionOrder(int* newValue) {} // conversionOrder = newValue; };
void GameMaster::setProgress(int achievement) { progress |= achievement; }

//===================================================================================================================================
//【getter】
//===================================================================================================================================
PlayerTable* GameMaster::getPlayerInfomation(){	return playerInformation;}				//プレイヤー情報の取得
float GameMaster::getGameTime() {return gameTimer;}										//ゲーム制限時間の取得
int GameMaster::getProgress() { return progress; }
bool GameMaster::whetherAchieved(int ahievement) { return progress & ahievement; }
//緑化率の取得
int	 GameMaster::getGreeningRate() {
	int numGreening = 0;
	for (int i = 0; i < treeTableList.nodeNum; i++)
	{
		TreeTable data = *treeTableList.getValue(i);
		switch (data.eventType)
		{
		case TO_DEAD: numGreening--;				break;
		case TO_GREEN_WITH_ANALOG:numGreening++;	break;
		case TO_GREEN_WITH_DIGITAL:numGreening++;	break;
		}
	}
	float rate = numGreening / treeNum;
	return (int)(rate*100.0f);
}													
//緑化本数
int  GameMaster::getGreeningTreeNum(int playerNo)
{
	int numGreening = 0;
	for (int i = 0; i < treeTableList.nodeNum; i++)
	{
		TreeTable data = *treeTableList.getValue(i);
		if (data.player != playerNo)continue;
		switch (data.eventType)
		{
		case TO_GREEN_WITH_ANALOG:numGreening++;	break;
		case TO_GREEN_WITH_DIGITAL:numGreening++;	break;
		}
	}
	return numGreening;
}
//撃退エネミー数
int  GameMaster::getKillEnemyNum(int playerNo)
{
	return killEnemyNum[playerNo];
}
//イベントリストの取得
int GameMaster::getEventList(TreeTable* out,float time)
{
	int eventNum = 0;
	
	//取得した時間からイベントの数を取得する。
	for (int i = 0;i< treeTableList.nodeNum;i++)
	{
		TreeTable table = *treeTableList.getValue(i);	//イベントを取得
		if (table.playBacked)continue;					//既に再生済みなので、スルー
		if (time > table.eventTime)	eventNum++;			//今再生すべきイベントなので、イベント数を加算
	}

	//リストの作成
	//・イベントリストの取得要求有(outにポインタ有)
	//・イベント数が0でない
	if (out && eventNum)
	{
		//アウトプットするイベントリストを作成する。
		TreeTable* list = NULL;
		list = new TreeTable[eventNum];
		int current = 0;
		for (int i = 0; i < treeTableList.nodeNum; i++)
		{
			//現在値がカウントしたイベント数を超えたらスルー[メモリガード]
			if (current >= eventNum)continue;
			
			//イベントを取得
			TreeTable table = *treeTableList.getValue(i);

			//既に再生済みならスルー
			if (table.playBacked)continue;

			//今再生すべきイベントなので、リストへ登録する
			if (time >= table.eventTime)		
			{
				//リストへ登録
				list[current] = table;
				current++;
			}
		}

		out = list;
	}

	return eventNum;
}

//===================================================================================================================================
//【GUI】
//===================================================================================================================================
#ifdef _DEBUG
void GameMaster::createGUI()
{
	ImGui::Text("gameTime = %f",		gameTimer);
	ImGui::Text("countDownTimer = %f",	countDownTimer);
	ImGui::Text("TreeNum = %d",	treeNum);
	if (ImGui::CollapsingHeader("TreeTableList"))
	{
		ImGui::Text("listNum: %d ", treeTableList.nodeNum);
		
		for (int i = 0; i < treeTableList.nodeNum; i++)
		{
			if (ImGui::CollapsingHeader("TreeTable"))
			{
				TreeTable table = *treeTableList.getValue(i);
				ImGui::Text("No:[%d]",i);				
				ImGui::Text("TreeId = %d",table.id);
				ImGui::Text("PlayerNo:%d",table.player+1);
				ImGui::Text("EventTime = %.02f",table.eventTime);
				ImGui::Text("model = %d",table.modelType);
				ImGui::Text("position(%.02f,%.02f,%.02f)",table.position.x,table.position.y,table.position.z);
				ImGui::Text("rotation(%.02f,%.02f,%.02f,%.02f)",table.rotation.x,table.rotation.y,table.rotation.z,table.rotation.w);
				ImGui::Text("scale(%.02f,%.02f,%.02f)",table.scale.x,table.scale.y,table.scale.z);
				
				if (table.playBacked)	ImGui::Text("playBacked:true");
				else					ImGui::Text("playBacked:false");

				switch (table.eventType)
				{
				case EVENT_TYPE::TO_DEAD:				ImGui::Text("EVENT:TO_DEAD");					break;
				case EVENT_TYPE::TO_GREEN_WITH_ANALOG:	ImGui::Text("EVENT:TO_GREEN_WITH_ANALOG");		break;
				case EVENT_TYPE::TO_GREEN_WITH_DIGITAL:	ImGui::Text("EVENT:TO_GREEN_WITH_DIGITAL");		break;
				}
			}
		}
	}
}
#endif