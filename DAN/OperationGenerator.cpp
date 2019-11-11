//-----------------------------------------------------------------------------
// メタAIイベント実行クラス [OperationGenerator.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "OperationGenerator.h"

using namespace aiNS;


//=============================================================================
// 初期化
//=============================================================================
void OperationGenerator::initialize(PLAYERAD* _playerAD, ENEMYAD* _enemyAD, TREEAD* _treeAD, ITEMAD* _itemAD,
	GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager, TreeManager* _treeManager,
	ItemManager* _itemManager, TelopManager* _telopManager)
{
	playerAD = _playerAD;
	enemyAD = _enemyAD;
	treeAD = _treeAD;
	itemAD = _itemAD;

	gameMaster = _gameMaster;
	player = _player;
	enemyManager = _enemyManager;
	treeManager = _treeManager;
	itemManager = _itemManager;
	telopManager = _telopManager;
}


//=============================================================================
// エネミー動的作成（SPAWN_ENEMY_AROUND_PLAYER)
//=============================================================================
void OperationGenerator::spawnEnemyAroundPlayer(enemyNS::ENEMYSET _enemySet)
{
	enemyNS::EnemyData* p = enemyManager->createEnemyData(_enemySet);
	enemyManager->createEnemy(p);
}


//=============================================================================
// エネミーリスポーン(RESPAWN_ENEMY)
//=============================================================================
void OperationGenerator::respawnEnemy(int _enemyID)
{
	enemyNS::EnemyData* p = enemyManager->findEnemyData(_enemyID);
	p->setUp();
	enemyManager->createEnemy(p);
}


//=============================================================================
// エネミーデジタルツリー襲撃（ENEMY_ATTACKS_TREE）
//=============================================================================
void OperationGenerator::enemyAttaksTree()
{

}
