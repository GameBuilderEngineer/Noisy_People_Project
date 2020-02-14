//-----------------------------------------------------------------------------
// メタAIイベント生成クラス [EventMaker.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once
#include "OperationGenerator.h"

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
class EventMaker :public Base
{
private:
	aiNS::AnalyticalData* data;			// 解析データ
	OperationGenerator* opeGenerator;	// イベント実行オブジェクト
	Fuzzy fuzzy;						// ファジー論理オブジェクト
	GameMaster* gameMaster;				// ゲーム管理オブジェクト
	Player* player;						// プレイヤー
	EnemyManager* enemyManager;			// エネミー管理オブジェクト
	TreeManager* treeManager;			// ツリー管理オブジェクト
	ItemManager* itemManager;			// アイテム管理オブジェクト
	TelopManager* telopManager;			// テロップ管理オブジェクト

	bool isBossEntried;					// ボス登場済み

public:
	void initialize(aiNS::AnalyticalData* data, OperationGenerator* _opeGenerator,
		GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager,
		TreeManager* _treeManager, ItemManager* _itemManager, TelopManager* _telopManager);
	void uninitialize();
	void update();

	// エネミー動的作成イベントの作成（SPAWN_ENEMY_AROUND_PLAYER)
	void makeEventSpawningEnemyAroundPlayer(int playerType);
	// エネミーリスポーンイベントの作成（RESPAWN_ENEMY)
	void makeEventRespawnEnemy();
	// エネミーデジタルツリー襲撃イベントの作成（ENEMY_ATTACKS_TREE）
	void makeEventEnemyAttaksTree();
	// 巨大環境破壊ロボ登場イベントの作成(BOSS_ENTRY)
	int makeEventBossEntry();
	// パワーアップアイテム登場イベントの作成(POWER_UP_ITEM)
	void makeEventPowerUpItem();
	// 動的作成するエネミーのパラメータを決める
	enemyNS::ENEMY_TYPE decideSpawnEnemyType();
	// 襲撃イベント対象デジタルツリーを選定する
	int decideAttackTargetTree();
	// 巨大環境破壊ロボの場所を決める
	D3DXVECTOR3 decideBossEntryPoint();
	// 基準座標を基に接地座標を作成する
	D3DXVECTOR3 createGroundedPositionFromPivot(D3DXVECTOR3 pivot, float distance, float reduction);
	// XZ平面上のベクトル(長さ1.0)をランダムに作成する
	D3DXVECTOR3 createRandomDirectionXZ();
};
