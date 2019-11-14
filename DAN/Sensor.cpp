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
void Sensor::initialize(aiNS::AnalyticalData* _data, GameMaster* _gameMaster, Player* _player,
	EnemyManager* _enemyManager, TreeManager* _treeManager, ItemManager* _itemManager)
{
	data = _data;
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

	//// エネミーをプレイヤー周りにスポーンすべきか解析
	//if (gameMaster->getGameTime() - enemyAD->lastSpawnTime > MANDATORY_SPAWN_INTERVAL)
	//{
	//	float tmp1 = fuzzy.reverseGrade((float)enemyAD->numChase, 0.0f, 4.0f);
	//	float tmp2 = fuzzy.reverseGrade((float)enemyAD->numKilledRecently, 0.0f, 6.0f);
	//	weightSpawn = fuzzy.AND(tmp1, tmp2);
	//}

	// 倒したエネミーをリスポーンすべきか解析

	// エネミーのツリー襲撃イベントを発生させるか解析
}


//=============================================================================
// プレイヤーセンサー
//=============================================================================
void Sensor::playerSensor()
{
	D3DXVECTOR3 vecBetweenPlayers =
		*player[gameMasterNS::PLAYER_1P].getPosition() - *player[gameMasterNS::PLAYER_2P].getPosition();

	// プレイヤー同士の距離	
	data->lengthBetweenTwoPlayers = D3DXVec3Length(&vecBetweenPlayers);

	// プレイヤーのコンディション
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		// HPの残量を0.0～1.0に変換
		float fuzzyHp = fuzzy.grade(player[i].getHp(), 0, playerNS::MAX_HP);

		if (fuzzyHp > 0.7f)
		{
			data->playerCondition[i] = PLAYER_CONDITION::GOOD;
		}
		else if(fuzzyHp > 0.4f)
		{
			data->playerCondition[i] = PLAYER_CONDITION::NORMAL;
		}
		else if(fuzzyHp > 0.2f)
		{
			// 電力の残量を0.0～1.0に変換
			float fuzzyPower = fuzzy.grade(player[i].getPower(), 0, playerNS::MAX_POWER);

			if (fuzzyPower > 0.33f)
			{
				data->playerCondition[i] = PLAYER_CONDITION::NORMAL;
			}
			else
			{
				data->playerCondition[i] = PLAYER_CONDITION::BAD;
			}
		}
		else
		{
			data->playerCondition[i] = PLAYER_CONDITION::BAD;
		}
	}
}


//=============================================================================
// エネミーセンサー
//=============================================================================
void Sensor::enemySensor()
{
	data->numChase = 0;
	data->numKilled = 0;
	data->numKilledRecently = 0;

	for (int i = 0; i < enemyManager->getEnemyDataList()->nodeNum; i++)
	{
		enemyNS::EnemyData* enemyData = enemyManager->getEnemyDataList()->getValue(i);

		if (enemyData->state == stateMachineNS::CHASE)
		{// 追跡ステートに入っているエネミーの数
			data->numChase++;
		}
		if (enemyData->isAlive == false)
		{
			// 倒されたエネミーの数
			data->numKilled++;

			if (enemyData->deadTime - gameMaster->getGameTime() < RECENT_SECOND)
			{// 最近倒されたエネミーの数
				data->numKilledRecently++;
			}
		}
	}
}


//=============================================================================
// ツリーセンサー
//=============================================================================
void Sensor::treeSensor()
{
	data->numDigital = 0;
	data->numGreen = 0;
	data->numBeingAttackedTree = 0;

	treeNS::TreeData* treeData;
	for (size_t i = 0; i < treeManager->getTreeList().size(); i++)
	{
		treeData = treeManager->getTreeList()[i]->getTreeData();
		if (treeData->type == treeNS::DIGITAL_TREE)
		{
			data->numDigital++;
		}
		if (treeData->geenState == treeNS::GREEN)
		{
			data->numGreen++;
		}
		if (treeData->isAttaked)
		{
			data->numBeingAttackedTree++;
		}
	}
}


//=============================================================================
// アイテムセンサー
//=============================================================================
void Sensor::itemSensor()
{

}