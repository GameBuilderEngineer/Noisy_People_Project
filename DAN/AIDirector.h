//-----------------------------------------------------------------------------
// メタAIクラス [AIDirector.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once
#include "Sensor.h"
#include "EventMaker.h"
#include "OperationGenerator.h"
#include "GameMaster.h"
#include "Player.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "TelopManager.h"
#include "LinkedList.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace aiNS
{
	// イベントリスト
	enum EVENT_LIST
	{
		SPAWN_ENEMY_AROUND_PLAYER,		// プレイヤーの周りに動的にエネミーをスポーンする
		RESPAWN_ENEMY,					// 一度倒したエネミーを初期位置にリスポーンする
		ENEMY_ATTACKS_TREE,				// エネミーがツリーを襲撃するイベントが発生する
		NUM_EVENT_LIST
	};

	// プレイヤーのコンディション
	enum PLAYER_CONDITION
	{
		GOOD,
		NORMAL,
		BAD
	};

	// イベント発生の評価値定数（下記値を上回ればイベントが実行される）
	const float WEIGHT_SPAWN = 0.8f;						// SPAWN_ENEMY_AROUND_PLAYER
	const float WEIGHT_RESPAWN = 0.7f;						// RESPAWN_ENEMY
	const float WEIGHT_ENEMY_ATTACKS_TREE = 0.9f;			// ENEMY_ATTACKS_TREE
	
	struct AnalyticalData
	{
		// プレイヤー
		float lengthBetweenTwoPlayers;						// プレイヤー同士の距離	
		int playerCondition[gameMasterNS::PLAYER_NUM];		// プレイヤーのコンディション

		// エネミー
		int numChase;										// 追跡ステートに入っているエネミーの数
		int numKilled;										// 倒されたエネミーの数
		int numKilledRecently;								// 最近倒されたエネミーの数
		float lastSpawnTime;								// 最後にスポーンした時刻

		// ツリー
		int numDigital;										// デジタルツリーの数
		int numGreen;										// 緑化されている木の数	
		int numBeingAttackedTree;							// 襲撃されている木の数

		// イベント発生の評価値（0.0～1.0）
		float weightSpawn[gameMasterNS::PLAYER_NUM];		// SPAWN_ENEMY_AROUND_PLAYER
		float weightRespawn;								// RESPAWN_ENEMY
		float weightEnemyAttacksTree;						// ENEMY_ATTACKS_TREE
	};
}


//=============================================================================
// AIディレクターは３つのモジュールを操作しゲーム中のイベントを統括する
// *センサーはゲームオブジェクトのパラメータを取集する
// *イベントメーカーはパラメータから発生させるイベントを決定する
// *オペレーションジェネレータが各オブジェクト管理クラスにイベント実行を指示する
//=============================================================================
class AIDirector
{
private:
	aiNS::AnalyticalData data;			// 解析データ

	Sensor sensor;						// センサーモジュール
	EventMaker eventMaker;				// イベント生成モジュール
	OperationGenerator opeGenerator;	// イベント実行モジュール
	int frameCnt;						// フレームカウンタ

	GameMaster* gameMaster;				// ゲーム管理オブジェクト
	Player* player;						// プレイヤー
	EnemyManager* enemyManager;			// エネミー管理オブジェクト
	TreeManager* treeManager;			// ツリー管理オブジェクト
	ItemManager* itemManager;			// アイテム管理オブジェクト
	TelopManager* telopManager;			// テロップ管理オブジェクト

public:
	// 実行
	void run();
	// 初期化
	void initialize(GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager,
		TreeManager* _treeManager, ItemManager* _itemManager, TelopManager* _telopManager);
	// ImGuiに表示
	void outputGUI();
};
