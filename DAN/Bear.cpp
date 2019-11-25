//-----------------------------------------------------------------------------
// ベアークラス [Bear.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/21
//-----------------------------------------------------------------------------
#include "Bear.h"
using namespace enemyNS;
using namespace stateMachineNS;


//=============================================================================
// コンストラクタ
//=============================================================================
Bear::Bear(ConstructionPackage constructionPackage) : Enemy(constructionPackage)
{
	// サイズを設定後にオブジェクト⇒姿勢制御の順で初期化
	setSize(D3DXVECTOR3(1.0f, 2.7f, 1.0f));
	Object::initialize(&position);
	enemyData->defaultDirection = slip(enemyData->defaultDirection, axisY.direction);
	postureControl(axisZ.direction, enemyData->defaultDirection, 1);
}


//=============================================================================
// デストラクタ
//=============================================================================
Bear::~Bear()
{

}


//=============================================================================
// 更新処理
//=============================================================================
void Bear::update(float frameTime)
{
	Enemy::preprocess(frameTime);
	switch (enemyData->state)
	{
	case CHASE:  chase(frameTime);  break;
	case PATROL: patrol(frameTime); break;
	case REST:   rest(frameTime);   break;
	case DIE:    die(frameTime);    break;
	}
	Enemy::postprocess(frameTime);
}


//=============================================================================
// 追跡ステート
//=============================================================================
void::Bear::chase(float frameTime)
{
	Enemy::chase(frameTime);

	float distance = between2VectorLength(position, *movingTarget);

	if (distance < 7.0f && canAttack)
	{
		attack();
	}
}


//=============================================================================
// 警戒ステート
//=============================================================================
void::Bear::patrol(float frameTime)
{
	Enemy::patrol(frameTime);

	if (onGround && isArraved || isDestinationLost)
	{
		setDebugDestination();		// デバッグ用目的地を設定
		isDestinationLost = false;	// 目的地はロストしていない
		searchPath();
	}
}


//=============================================================================
// 休憩ステート
//=============================================================================
void::Bear::rest(float frameTime)
{

}


//=============================================================================
// ツリー攻撃ステート
//=============================================================================
void Bear::attackTree(float frameTime)
{
	Enemy::attackTree(frameTime);
}


//=============================================================================
// 死亡ステート
//=============================================================================
void::Bear::die(float frameTime)
{
	Enemy::die(frameTime);
}


//=============================================================================
// Getter
//=============================================================================


//=============================================================================
// Setter
//=============================================================================
