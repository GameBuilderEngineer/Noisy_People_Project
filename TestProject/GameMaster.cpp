//===================================================================================================================================
//【GameMaster.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#include "GameMaster.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace gameMasterNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
GameMaster::GameMaster()
{
	ZeroMemory(this, sizeof(GameMaster));
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
GameMaster::~GameMaster()
{

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
	if (startInterval > 0) {
		startInterval -= frameTime;
		return;
	}
	//ゲーム開始カウントダウン
	gameStartCount(frameTime);
	if (!alreadyStart)return;
	if(displayStart())displayStartTimer -= frameTime;

	//ゲームタイムの更新
	updateGameTime(frameTime);

	//ゲーム終了カウントダウン
	gameFinishCount(frameTime);
	if(displayFinish())displayFinishTimer -= frameTime;
}

//===================================================================================================================================
//【ゲーム時間の更新】
//===================================================================================================================================
void GameMaster::updateGameTime(float frameTime)
{
	if (alreadyStart == false)return;
	gameTimer -= frameTime;
}

//===================================================================================================================================
//【ゲーム開始時のカウントダウンの更新】
//===================================================================================================================================
void GameMaster::gameStartCount(float frameTime)
{
	if (alreadyStart)return;

	countFlag = false;
	countDownTimer += frameTime;
	if (countDownTimer > COUNT_INTERVAL)
	{
		countDownTimer = 0.0f;
		count--;
		countFlag = true;
	}

	if (count == 0)
	{
		startFinishFlag = true;
		alreadyStart = true;	//ゲーム開始
		displayStartTimer = DISPLAY_START_TIME;
		setCountDown();			//ゲーム終了カウントダウンのセット
	}

}

//===================================================================================================================================
//【ゲーム終了時のカウントダウンの更新】
//===================================================================================================================================
void GameMaster::gameFinishCount(float frameTime)
{
	if (!whetherCountFinish())return;

	countFlag = false;
	countDownTimer += frameTime;
	if (countDownTimer > COUNT_INTERVAL)
	{
		countDownTimer = 0.0f;
		count--;
		countFlag = true;
	}

	if (count == 0)
	{
		startFinishFlag = true;
		alreadyFinish = true;
		displayFinishTimer = DISPLAY_FINISH_TIME;
	}
}

//===================================================================================================================================
//【ゲームカウントダウンのセット】
//===================================================================================================================================
void GameMaster::setCountDown()
{
	count = COUNT_DOWN;
	countDownTimer = 0.0f;
}

//===================================================================================================================================
//【ゲーム開始処理】
//===================================================================================================================================
void GameMaster::gameStart()
{
	countFlag			= false;
	alreadyStart		= false;
	alreadyFinish		= false;
	gameTimer			= GAME_TIME;
	displayStartTimer	= 0.0f;
	displayFinishTimer	= 0.0f;
	startInterval		= START_INTERVAL;
	setCountDown();
}

//===================================================================================================================================
//【setter】
//===================================================================================================================================
//void GameMaster::setPlayerCharacter(int playerNo,int playerType, int modelType) {
//	if (playerNo >= playerNS::NUM_PLAYER || playerNo < 0)return;
//	playerInformation[playerNo].playerType= playerType;
//	playerInformation[playerNo].modelType = modelType;
//}

//===================================================================================================================================
//【getter】
//===================================================================================================================================
//プレイヤー情報の取得
//playerTable* GameMaster::getPlayerInfomation(){	return playerInformation;}
//ゲーム制限時間の取得
float GameMaster::getGameTime() {return gameTimer;}
//ゲームが終了したかどうか
bool GameMaster::whetherGameOver(){	return gameTimer < 0;}
int GameMaster::getCount() { return count; }
bool GameMaster::getCountFlag() { return countFlag; }
bool GameMaster::whetherAlreadyStart() { return alreadyStart; }
bool GameMaster::whetherAlreadyFinish() { return alreadyFinish; }
bool GameMaster::whetherCountFinish() { return gameTimer < (float)(COUNT_DOWN+1); }
bool GameMaster::displayStart() { return displayStartTimer > 0.0f; }
bool GameMaster::displayFinish() { return displayFinishTimer > 0.0f; }
