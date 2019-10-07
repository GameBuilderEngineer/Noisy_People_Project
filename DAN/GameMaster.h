//===================================================================================================================================
//【GameMaster.h】
// [作成者] HAL東京GP12A332 11 菅野 樹
// [作成日] 2019/09/20
// [更新日] 2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
//#include "StaticMeshLoader.h"
//#include "Player.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace gameMasterNS {
	const float GAME_TIME = 60.0f * 4.0f;	//4分
	const float COUNT_DOWN_TIME = 3.0f;		//3秒

	enum PLAYER_TYPE
	{
		PLAYER_CHARACTER,
		NON_PLAYER_CHARACTER,
		TYPE_NUM
	};

	enum RESULT_LIST
	{
		WIN_1P,
		WIN_2P,
		DRAW,
	};

	enum MODEL_TYPE
	{
		ADAM,
		EVE,
		MODEL_TYPE_MAX
	};

	const int MODEL_ADAM = MODEL_TYPE::ADAM;
	const int MODEL_EVE = MODEL_TYPE::EVE;

	const int COUNT_DOWN = 3;
	const float COUNT_INTERVAL = 1.0f;
	const float START_INTERVAL = 0.5f;
	const float DISPLAY_START_TIME = 1.0f;
	const float DISPLAY_FINISH_TIME = 1.0f;
}
//===================================================================================================================================
//【構造体定義】
//===================================================================================================================================
//プレイヤー情報
struct playerTable
{
	int playerType;		//PC|NPC
	int modelType;		//プレイヤーのモデルタイプ
	int wage;			//チンギン
};

//===================================================================================================================================
//【ゲーム管理クラス】
// [用途]	このクラスは、シーンをまたいで情報の橋渡しを行う。
//			更新や設定は、各シーンで行う。
//===================================================================================================================================
class GameMaster :public Base
{
private:
	//Data
	float gameTimer;										//ゲーム時間
	float countDownTimer;									//カウントダウン時間
	float startInterval;									//開始カウントダウンインターバル
	float displayStartTimer;								//スタートアナウンス表示時間
	float displayFinishTimer;								//フィニッシュアナウンス表示時間
	bool alreadyStart;										//スタート済
	bool alreadyFinish;										//フィニッシュ済
	int count;												//ゲームカウント
	//playerTable playerInformation[playerNS::NUM_PLAYER];	//プレイヤー情報
	//int chingin[playerNS::NUM_PLAYER];						//	チンギン
	bool countFlag;
	bool startFinishFlag;
public:
	//Method
	GameMaster();
	~GameMaster();
	void initialize();
	void update(float frameTime);
	void updateGameTime(float frameTime);					//ゲーム時間の更新
	void gameStartCount(float frameTime);					//ゲーム開始時のカウントダウン
	void gameFinishCount(float frameTime);					//ゲーム終了時のカウントダウン
	void setCountDown();									//カウントダウンのセット
	void gameStart();										//ゲーム開始処理

	//setter
	//void setPlayerCharacter(int playerNo, int playerType, int modelType);	//キャラクター情報をセット

	//getter
	//playerTable* getPlayerInfomation();
	float getGameTime();
	int getCount();
	bool getCountFlag();
	bool whetherGameOver();
	bool whetherAlreadyStart();
	bool whetherAlreadyFinish();
	bool whetherCountFinish();
	bool displayStart();		//スタート表記表示時間
	bool displayFinish();		//フィニッシュ表記表示時間

};