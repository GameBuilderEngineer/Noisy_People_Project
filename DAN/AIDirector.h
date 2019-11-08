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
	// プレイヤー解析データ
	typedef struct PlayerAnalyticalData
	{
		float lengthBetweenTwoPlayers;		// プレイヤー同士の距離	
		int	numOfDead;						// 何回死んでいるか

	} PLAYERAD;

	// エネミー解析データ
	typedef struct EnemyAnalyticalData
	{
		int numChase;						// 追跡ステートに入っているエネミーの数
		int numKilled;						// 倒されたエネミーの数
		int numKilledRecently;				// 最近倒されたエネミーの数
		float lastSpawnTime;				// 最後にスポーンした時刻
		float shouldSpawnFuzzily;			// スポーンすべきか重みづけ
	} ENEMYAD;

	// ツリー解析データ
	typedef struct TreeAnalyticalData
	{
		int numDigital;						// デジタルツリーの数
		int numGreen;	
		int numBeingAttackedTree;			// 襲撃されている木の数

	} TREEAD;

	// アイテム解析データ
	typedef struct ItemAnalyticalData
	{

	} ITEMAD;

	// イベントリスト
	enum EVENT_LIST
	{
		SPAWN_ENEMY_AROUND_PLAYER,
		RESPAWN_ENEMY,
		ENEMY_ATTACKS_TREE,
		NUM_EVENT_LIST
	};


	// エネミ―イベント関係定数
	const float RECENT_SECOND = 30.0f;				// 最近と見なす秒数（直近○秒）
	const float MANDATORY_SPAWN_INTERVAL = 20.0f;	// スポーンのための最低経過間隔秒

}


//=============================================================================
//クラス定義
//=============================================================================
// AIディレクターは３つのモジュールを操作しゲーム中のイベントを統括する
// センサーはゲームオブジェクトのパラメータを取集する
// イベントメーカーはパラメータから発生させるイベントを決定する
// オペレーションジェネレータが各オブジェクト管理クラスにイベント実行を指示する
class AIDirector
{
private:
	aiNS::PLAYERAD playerAD;			// プレイヤー解析データ
	aiNS::ENEMYAD enemyAD;				// エネミー解析データ
	aiNS::TREEAD treeAD;				// ツリー解析データ
	aiNS::ITEMAD itemAD;				// アイテム解析データ

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
