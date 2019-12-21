//===================================================================================================================================
//【GameMaster.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/12/21
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
	openingTimer		= 0.0f;								//オープニング時間
	endingTimer			= 0.0f;								//エンディング時間
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
	openingTimer	= OPENING_TIME;
	endingTimer		= ENDING_TIME;
	countDownTimer	= COUNT_DOWN_TIME;

#ifdef _DEBUG
#if 1
	gameTimer		= DEBUG_GAME_TIME;
#else
	gameTimer		= GAME_TIME;
#endif
#else
	//Release
	gameTimer		= GAME_TIME;
#endif
	gameTimerStop	= false;
	pause = false;
	progress = 0x00000000;
	discardConversionOrder();
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		killEnemyNum[i] = 0;
	}

	ZeroMemory(wasStartVoicePlayed, 2 * sizeof(bool));
	ZeroMemory(wasFinishVoicePlayed, 2 * sizeof(bool));
}

//===================================================================================================================================
//【ゲームオープニング時間の更新】
//===================================================================================================================================
void GameMaster::updateOpeningTime(float frameTime)
{
	if (gameTimerStop)return;

	openingTimer -= frameTime;
	if (openingTimer <= 0.0f)
	{
		setProgress(PASSING_GAME_OPENING);
	}
}

//===================================================================================================================================
//【ゲーム開始カウントダウンの更新】
//===================================================================================================================================
void GameMaster::updateStartCountDown(float frameTime)
{
	if (gameTimerStop)return;

	//ゲームが開始されたら更新しない
	if (whetherAchieved(PASSING_GAME_START))return;
	
	//ゲームオープニングが終了していた場合
	if(whetherAchieved(PASSING_GAME_OPENING))
		countDownTimer -= frameTime;
}

//===================================================================================================================================
//【ゲーム終了カウントダウンの更新】
//===================================================================================================================================
void GameMaster::updateFinishCountDown(float frameTime)
{
	if (gameTimerStop)return;

	//残り10秒の場合
	if(gameTimer <= 11)
		countDownTimer -= frameTime;
}

//===================================================================================================================================
//【ゲーム時間の更新】
//===================================================================================================================================
void GameMaster::updateGameTime(float frameTime)
{
	if (gameTimerStop)return;

	//ゲームが終了している場合
	if (whetherAchieved(PASSING_GAME_FINISH))
	{
		gameTimer = 0.0f;
		return;
	}

	//ゲームが開始されている場合
	if (whetherAchieved(PASSING_GAME_START))
	{
		gameTimer -= frameTime;
	}
}

//===================================================================================================================================
//【ゲームエンディング時間の更新】
//===================================================================================================================================
void GameMaster::updateEndingTime(float frameTime)
{
	if (gameTimerStop)return;

	//ゲームが終了している場合
	if(whetherAchieved(PASSING_GAME_FINISH))
		endingTimer -= frameTime;
	
	if (endingTimer <= 0.0f)
	{
		setProgress(PASSING_GAME_ENDING);
	}
}

//===================================================================================================================================
//【開始カウントダウン時のアクション】
//===================================================================================================================================
bool GameMaster::playActionStartCount(int countNum)
{
	//ゲームオープニングが終了した場合のみ
	if (!whetherAchieved(PASSING_GAME_OPENING))return false;

	if (countDownTimer > (float)countNum)return false;
	switch (countNum)
	{
	case 3:
		if (whetherAchieved(PASSING_COUNT_DOWN_THREE))return false;
		setProgress(PASSING_COUNT_DOWN_THREE);
		return true;break;
	case 2:
		if (whetherAchieved(PASSING_COUNT_DOWN_TWO))return false;
		setProgress(PASSING_COUNT_DOWN_TWO);
		return true;break;
	case 1:
		if (whetherAchieved(PASSING_COUNT_DOWN_ONE))return false;
		setProgress(PASSING_COUNT_DOWN_ONE);
		return true;break;
	case 0:
		if (whetherAchieved(PASSING_GAME_START))return false;
		setProgress(PASSING_GAME_START);
		countDownTimer = 10.0f;
		return true; break;
	}
	return false;
}

//===================================================================================================================================
//【残り時間1分のアクション】
//===================================================================================================================================
bool GameMaster::playActionRamaining1Min()
{
	if (gameTimer > 60)return false;
	if (whetherAchieved(PASSING_REMAINING_ONE_MINUTE))return false;

	setProgress(PASSING_REMAINING_ONE_MINUTE);

	return true;
}

//===================================================================================================================================
//【終了カウントダウン時のアクション】
//===================================================================================================================================
bool GameMaster::playActionFinishCount(int countNum)
{
	//ゲームタイマーが10秒以上ある
	if (gameTimer > 10)return false;

	if (countDownTimer > (float)countNum)return false;
	switch (countNum)
	{
	case 10:
		if (whetherAchieved(PASSING_REMAINING_10))return false;
		setProgress(PASSING_REMAINING_10);
		return true; break;
	case 9:
		if (whetherAchieved(PASSING_REMAINING_9))return false;
		setProgress(PASSING_REMAINING_9);
		return true; break;
	case 8:
		if (whetherAchieved(PASSING_REMAINING_8))return false;
		setProgress(PASSING_REMAINING_8);
		return true; break;
	case 7:
		if (whetherAchieved(PASSING_REMAINING_7))return false;
		setProgress(PASSING_REMAINING_7);
		return true; break;
	case 6:
		if (whetherAchieved(PASSING_REMAINING_6))return false;
		setProgress(PASSING_REMAINING_6);
		return true; break;
	case 5:
		if (whetherAchieved(PASSING_REMAINING_5))return false;
		setProgress(PASSING_REMAINING_5);
		return true; break;
	case 4:
		if (whetherAchieved(PASSING_REMAINING_4))return false;
		setProgress(PASSING_REMAINING_4);
		return true; break;
	case 3:
		if (whetherAchieved(PASSING_REMAINING_3))return false;
		setProgress(PASSING_REMAINING_3);
		return true; break;
	case 2:
		if (whetherAchieved(PASSING_REMAINING_2))return false;
		setProgress(PASSING_REMAINING_2);
		return true; break;
	case 1:
		if (whetherAchieved(PASSING_REMAINING_1))return false;
		setProgress(PASSING_REMAINING_1);
		return true; break;
	case 0:
		if (whetherAchieved(PASSING_GAME_FINISH))return false;
		setProgress(PASSING_GAME_FINISH);
		return true; break;
	}
	return false;
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
void GameMaster::addKillEnemyNum(int playerNo) { killEnemyNum[playerNo]++; }
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
	float rate = 1.0f;
	if(treeNum)
		rate = (float)numGreening / (float)treeNum;
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
int GameMaster::getEventList(TreeTable** out,float time)
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
	if (out != NULL && eventNum)
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
			TreeTable* table = treeTableList.getValue(i);

			//既に再生済みならスルー
			if (table->playBacked)continue;

			//今再生すべきイベントなので、リストへ登録する
			if (time >= table->eventTime)		
			{
				//リストへ登録
				list[current] = *table;
				//リスト番号を進める
				current++;
				//イベントテーブルを再生完了状態にする
				table->playBacked = true;
			}
		}

		*out = list;
	}

	return eventNum;
}

//===================================================================================================================================
//【GUI】
//===================================================================================================================================
#ifdef _DEBUG
void GameMaster::createGUI()
{
	ImGui::Text("openingTimer = %f",	openingTimer);
	ImGui::Text("countDownTimer = %f",	countDownTimer);
	ImGui::Text("gameTime = %f",		gameTimer);
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