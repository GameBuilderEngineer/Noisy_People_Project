//-----------------------------------------------------------------------------
// ウルフクラス [Wolf.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/21
//-----------------------------------------------------------------------------
#include "Wolf.h"
using namespace enemyNS;
using namespace stateMachineNS;

//=============================================================================
// コンストラクタ
//=============================================================================
Wolf::Wolf(StaticMesh* _staticMesh, EnemyData* _enemyData): Enemy(_staticMesh, _enemyData)
{

}


//=============================================================================
// デストラクタ
//=============================================================================
Wolf::~Wolf()
{

}


//=============================================================================
// 更新処理
//=============================================================================
void Wolf::update(float frameTime)
{
	Enemy::preprocess(frameTime);
	switch (enemyData->state)
	{
	case CHASE:  chase();  break;
	case PATROL: patrol(); break;
	case REST:   rest();   break;
	case DIE:    die();    break;
	}
	Enemy::update(frameTime);
}


//=============================================================================
// 追跡ステート
//=============================================================================
void::Wolf::chase()
{
	if (onGround && isArraved || isDestinationLost)
	{
		// 適切なプレイヤーを移動ターゲットに設定する
		if (isNoticedPlayer[gameMasterNS::PLAYER_1P] && isNoticedPlayer[gameMasterNS::PLAYER_2P] == false)
		{
			setMovingTarget(&player[gameMasterNS::PLAYER_1P].position);
		}
		else if (isNoticedPlayer[gameMasterNS::PLAYER_1P] == false && isNoticedPlayer[gameMasterNS::PLAYER_2P])
		{
			setMovingTarget(&player[gameMasterNS::PLAYER_2P].position);
		}
		else if (isNoticedPlayer[gameMasterNS::PLAYER_1P] && isNoticedPlayer[gameMasterNS::PLAYER_2P])
		{
			float distance1 = between2VectorLength(position, player[gameMasterNS::PLAYER_1P].position);
			float distance2 = between2VectorLength(position, player[gameMasterNS::PLAYER_2P].position);
			if (distance1 < distance2) { setMovingTarget(&player[gameMasterNS::PLAYER_1P].position); }
			else { setMovingTarget(&player[gameMasterNS::PLAYER_1P].position); }
		}

		isDestinationLost = false;	// 目的地はロストしていない
		shouldSearch = true;		// パスサーチ実行
	}

	setMove(true);
}


//=============================================================================
// 警戒ステート
//=============================================================================
void::Wolf::patrol()
{
	if (onGround && isArraved || isDestinationLost)
	{
		setDebugDestination();		// デバッグ用目的地を設定
		isDestinationLost = false;	// 目的地はロストしていない
		shouldSearch = true;		// パスサーチ実行
	}

	setMove(true);
}


//=============================================================================
// 休憩ステート
//=============================================================================
void::Wolf::rest()
{

}


//=============================================================================
// 死亡ステート
//=============================================================================
void::Wolf::die()
{

}


//=============================================================================
// Getter
//=============================================================================


//=============================================================================
// Setter
//=============================================================================
