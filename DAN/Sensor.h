//-----------------------------------------------------------------------------
// メタAI用センサークラス [Sensor.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once
#include "GameMaster.h"
#include "Player.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Telop.h"
#include "Fuzzy.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace aiNS
{
	// 前方宣言
	struct AnalyticalData;

	// enemySensor()
	const float RECENT_SECOND = 30.0f;				// 最近と見なす秒数（直近○秒）
	const float MANDATORY_SPAWN_INTERVAL = 20.0f;	// スポーンのための最低経過間隔秒
}


//=============================================================================
//クラス定義
//=============================================================================
class Sensor
{
private:
	aiNS::AnalyticalData* data;			// 解析データ

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
	void initialize(aiNS::AnalyticalData* _data, GameMaster* _gameMaster, Player* _player,
		EnemyManager* _enemyManager, TreeManager* _treeManager, ItemManager* _itemManager);

	// 更新処理
	void update();
};
