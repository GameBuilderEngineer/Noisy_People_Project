//-----------------------------------------------------------------------------
// メタAIイベント実行クラス [OperationGenerator.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once
#include "GameMaster.h"
#include "Player.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "TelopManager.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace aiNS
{
	// 前方宣言
	struct AnalyticalData;
}


//=============================================================================
//クラス定義
//=============================================================================
class OperationGenerator :public Base
{
private:
	aiNS::AnalyticalData* data;			// 解析データ
	GameMaster* gameMaster;				// ゲーム管理オブジェクト
	Player* player;						// プレイヤー
	EnemyManager* enemyManager;			// エネミー管理オブジェクト
	TreeManager* treeManager;			// ツリー管理オブジェクト
	ItemManager* itemManager;			// アイテム管理オブジェクト
	TelopManager* telopManager;			// テロップ管理オブジェクト
	Ray ray;

	bool wasBossEntried;
	bool wasTelopDisplayed[3];
	float bossEntryTime;
	
public:
	// 初期化
	void initialize(aiNS::AnalyticalData* _data, GameMaster* _gameMaster,
		Player* _player, EnemyManager* _enemyManager, TreeManager* _treeManager,
		ItemManager* _itemManager, TelopManager* _telopManager);
	// エネミー動的作成
	void spawnEnemy(enemyNS::ENEMYSET _enemySet);
	// エネミーリスポーン
	void respawnEnemy(int _enemyID);
	// エネミーデジタルツリー襲撃
	void enemyAttaksTree(enemyNS::ENEMYSET _enemySet, Tree* _attackTarget);
	// 巨大環境破壊ロボが登場
	void bossEntry(enemyNS::ENEMYSET _enemySet);
	void updateBossEvent();
};
