//-----------------------------------------------------------------------------
// メタAIクラス [AIDirector.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "EnemyManager.h"
#include "ImguiManager.h"
using namespace aiNS;


//=============================================================================
// コンストラクタ
//=============================================================================
AIDirector::AIDirector()
{

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
	TelopManager* _telopManager)
{
	gameMaster = _gameMaster;
	fieldMesh = _fieldMesh;
	player = _player;
	enemyManager = _enemyManager;
	treeManager = _treeManager;
	itemManager = _itemManager;
	telopManager = _telopManager;

	sensor.initialize(&data, gameMaster, player, enemyManager, treeManager, itemManager);
	eventMaker.initialize(&data, &opeGenerator, gameMaster, player, enemyManager, treeManager, itemManager, _telopManager);
	opeGenerator.initialize(&data, gameMaster, player, enemyManager, treeManager, itemManager, _telopManager);
	frameCnt = 0;

	// 解析データの初期化
	data.lastSpawnTime[gameMasterNS::PLAYER_1P] =
	data.lastSpawnTime[gameMasterNS::PLAYER_2P] = gameMasterNS::GAME_TIME;
	data.treeDistanceFromPlayer[gameMasterNS::PLAYER_1P] = new float[treeManager->getTreeList().size()];
	data.treeDistanceFromPlayer[gameMasterNS::PLAYER_2P] = new float[treeManager->getTreeList().size()];
	data.lastTimeEnemyAttaksTree = gameMasterNS::GAME_TIME;
	data.lastTimeCheckedWeightEnemyAttacksTree = gameMasterNS::GAME_TIME;
	data.wasBossEntried = false;

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
	}
	else// frameCnt % 2 == 1
	{
		eventMaker.update();
	}

	frameCnt++;
}


//=============================================================================
// ImGuiに出力
//=============================================================================
void AIDirector::outputGUI()
{
#ifdef _DEBUG
#endif
}