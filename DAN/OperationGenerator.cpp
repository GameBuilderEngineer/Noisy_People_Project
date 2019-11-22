//-----------------------------------------------------------------------------
// メタAIイベント実行クラス [OperationGenerator.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "OperationGenerator.h"
#include "NavigationMesh.h"
using namespace aiNS;


//=============================================================================
// 初期化
//=============================================================================
void OperationGenerator::initialize(aiNS::AnalyticalData* _data, GameMaster* _gameMaster, 
	Player* _player, EnemyManager* _enemyManager, TreeManager* _treeManager,
	ItemManager* _itemManager, TelopManager* _telopManager)
{
	data			= _data;
	gameMaster		= _gameMaster;
	player			= _player;
	enemyManager	= _enemyManager;
	treeManager		= _treeManager;
	itemManager		= _itemManager;
	telopManager	= _telopManager;
}


//=============================================================================
// エネミー動的作成
//=============================================================================
void OperationGenerator::spawnEnemy(enemyNS::ENEMYSET _enemySet)
{
	enemyNS::EnemyData* p = enemyManager->createEnemyData(_enemySet);
	enemyManager->createEnemy(p);
}


//=============================================================================
// エネミーリスポーン
//=============================================================================
void OperationGenerator::respawnEnemy(int _enemyID)
{
	enemyNS::EnemyData* p = enemyManager->findEnemyData(_enemyID);
	p->setUp();
	enemyManager->createEnemy(p);
}


//=============================================================================
// エネミーデジタルツリー襲撃
//=============================================================================
void OperationGenerator::enemyAttaksTree(enemyNS::ENEMYSET _enemySet, Tree* _attackTarget)
{
	telopManager->play(telopManagerNS::TELOP_TYPE3);
	enemyNS::EnemyData* p = enemyManager->createEnemyData(_enemySet);
	p->targetTree = _attackTarget;
	enemyManager->createEnemy(p);

}