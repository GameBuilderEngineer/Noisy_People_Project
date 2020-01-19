//-----------------------------------------------------------------------------
// メタAIクラス [AIDirector.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "EnemyManager.h"
#include "ImguiManager.h"
using namespace aiNS;

// Staticメンバ変数
AIDirector* AIDirector::pointer = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
AIDirector::AIDirector()
{
	pointer = this;
}


//=============================================================================
// デストラクタ
//=============================================================================
AIDirector::~AIDirector()
{
	uninitialize();
}


//=============================================================================
// 初期化
//=============================================================================
void AIDirector::initialize(GameMaster* _gameMaster, LPD3DXMESH _fieldMesh, Player* _player,
	EnemyManager* _enemyManager, TreeManager* _treeManager, ItemManager* _itemManager,
	TelopManager* _telopManager, MarkerRenderer* marker)
{
	gameMaster = _gameMaster;
	fieldMesh = _fieldMesh;
	player = _player;
	enemyManager = _enemyManager;
	treeManager = _treeManager;
	itemManager = _itemManager;
	telopManager = _telopManager;
	markerRenderer = marker;

	sensor.initialize(&data, gameMaster, player, enemyManager, treeManager, itemManager);
	eventMaker.initialize(&data, &opeGenerator, gameMaster, player, enemyManager, treeManager, itemManager, _telopManager);
	opeGenerator.initialize(&data, gameMaster, player, enemyManager, treeManager, itemManager, _telopManager, markerRenderer);
	frameCnt = 0;

	// 解析データの初期化
	data.lastSpawnTime[gameMasterNS::PLAYER_1P] =
	data.lastSpawnTime[gameMasterNS::PLAYER_2P] = gameMasterNS::GAME_TIME;
	data.treeDistanceFromPlayer[gameMasterNS::PLAYER_1P] = new float[treeManager->getTreeList().size()];
	data.treeDistanceFromPlayer[gameMasterNS::PLAYER_2P] = new float[treeManager->getTreeList().size()];
	data.lastTimeEnemyAttaksTree = gameMasterNS::GAME_TIME;
	data.ajustTimeEnemyAttaksTree = 0.0f;
	data.wasBossEntried = false;
	data.attackedTree = NULL;

	BoundingSphere temp;
	temp.initialize(NULL, fieldMesh);
	data.fieldRadius = temp.getRadius();
}


//=============================================================================
// 終了処理
//=============================================================================
void AIDirector::uninitialize()
{
	SAFE_DELETE_ARRAY(data.treeDistanceFromPlayer[gameMasterNS::PLAYER_1P]);
	SAFE_DELETE_ARRAY(data.treeDistanceFromPlayer[gameMasterNS::PLAYER_2P]);
}


//=============================================================================
// 実行
//=============================================================================
void AIDirector::run()
{
	if (frameCnt % 2 == 0)
	{
		sensor.update();
		eventMaker.update();
	}
	// 初期構想では1フレーム毎に処理を分けるつもりでいたが、
	// ツリー攻撃イベントのフラグ処理の支障になることから
	// 1フレームに2つの処理をまとめることにした
	//else// frameCnt % 2 == 1
	//{
	//}

	frameCnt++;

	//opeGenerator.updateBossEvent();
}


//=============================================================================
// ImGuiに出力
//=============================================================================
void AIDirector::outputGUI()
{
#ifdef _DEBUG
#endif
}