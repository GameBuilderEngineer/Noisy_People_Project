//-----------------------------------------------------------------------------
// メタAI用センサークラス [Sensor.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#include "GameMaster.h"
#include "Player.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Telop.h"
#include "Fuzzy.h"
#pragma once


//=============================================================================
// 名前空間
//=============================================================================
namespace aiNS
{
	// 前方宣言
	struct	PlayerAnalyticalData;
	typedef PlayerAnalyticalData PLAYERAD;
	struct	EnemyAnalyticalData;
	typedef EnemyAnalyticalData	ENEMYAD;
	struct	TreeAnalyticalData;
	typedef TreeAnalyticalData TREEAD;
	struct	ItemAnalyticalData;
	typedef ItemAnalyticalData ITEMAD;
}


//=============================================================================
//クラス定義
//=============================================================================
class Sensor
{
private:
	aiNS::PLAYERAD* playerAD;			// プレイヤー解析データ
	aiNS::ENEMYAD* enemyAD;				// エネミー解析データ
	aiNS::TREEAD* treeAD;				// ツリー解析データ
	aiNS::ITEMAD* itemAD;				// アイテム解析データ

	GameMaster* gameMaster;				// ゲーム管理オブジェクト
	Player* player;						// プレイヤー
	EnemyManager* enemyManager;			// エネミー管理オブジェクト
	TreeManager* treeManager;			// ツリー管理オブジェクト
	ItemManager* itemManager;			// アイテム管理オブジェクト

	Fuzzy fuzzy;				
	void playerSensor();				// プレイヤーセンサー
	void enemySensor();					// エネミーセンサー
	void treeSensor();					// ツリーセンサー
	void itemSensor();					// アイテムセンサー

public:
	// 初期化
	void initialize(aiNS::PLAYERAD* _playerAD, aiNS::ENEMYAD* _enemyAD,
		aiNS::TREEAD* _treeAD, aiNS::ITEMAD* _itemAD, GameMaster* _gameMaster, Player* _player,
		EnemyManager* _enemyManager, TreeManager* _treeManager, ItemManager* _itemManager);
	// 更新処理
	void update();
};
