//-----------------------------------------------------------------------------
// メタAI用センサークラス [Sensor.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "Sensor.h"
using namespace aiNS;


//=============================================================================
// 初期化
//=============================================================================
void Sensor::initialize(aiNS::PLAYERAD* _playerAD, aiNS::ENEMYAD* _enemyAD,
	aiNS::TREEAD* _treeAD, aiNS::ITEMAD* _itemAD, GameMaster* _gameMaster, Player* _player,
	EnemyManager* _enemyManager, TreeManager* _treeManager, ItemManager* _itemManager)
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
}


//=============================================================================
// 更新処理
//=============================================================================
void Sensor::update()
{
	// 各センサーで情報を取得
	playerSensor();
	enemySensor();
	treeSensor();
	itemSensor();

	// エネミーをスポーンすべきか解析
	if (gameMaster->getGameTime() - enemyAD->lastSpawnTime > MANDATORY_SPAWN_INTERVAL)
	{
		float tmp1 = fuzzy.reverseGrade((float)enemyAD->numChase, 0.0f, 4.0f);
		float tmp2 = fuzzy.reverseGrade((float)enemyAD->numKilledRecently, 0.0f, 6.0f);
		enemyAD->shouldSpawnFuzzily = fuzzy.AND(tmp1, tmp2);
	}
}


//=============================================================================
// プレイヤーセンサー
//=============================================================================
void Sensor::playerSensor()
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		player[i].getPosition();
		player[i].getMatrixWorld();
		player[i].getAxisZ()->direction;
		player[i].getState();
		player[i].getHp();
		player[i].getPower();
	}
}


//=============================================================================
// エネミーセンサー
//=============================================================================
void Sensor::enemySensor()
{
	enemyAD->numChase = 0;
	enemyAD->numKilled = 0;
	enemyAD->numKilledRecently = 0;

	for (int i = 0; i < enemyManager->getEnemyDataList()->nodeNum; i++)
	{
		enemyNS::EnemyData* enemyData = enemyManager->getEnemyDataList()->getValue(i);

		if (enemyData->state == stateMachineNS::CHASE)
		{// 追跡ステートに入っているエネミーの数
			enemyAD->numChase++;
		}
		if (enemyData->isAlive == false)
		{
			// 倒されたエネミーの数
			enemyAD->numKilled++;

			if (enemyData->deadTime - gameMaster->getGameTime() < RECENT_SECOND)
			{// 最近倒されたエネミーの数
				enemyAD->numKilledRecently++;
			}
		}
	}
}


//=============================================================================
// ツリーセンサー
//=============================================================================
void Sensor::treeSensor()
{

}


//=============================================================================
// アイテムセンサー
//=============================================================================
void Sensor::itemSensor()
{

}