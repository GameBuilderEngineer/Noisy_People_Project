//-----------------------------------------------------------------------------
// ウルフクラス [Wolf.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/21
//-----------------------------------------------------------------------------
#include "Wolf.h"
#include "EnemyManager.h"
#include "WolfAnimation.h"
using namespace enemyNS;
using namespace stateMachineNS;
using namespace wolfNS;

//=============================================================================
// コンストラクタ
//=============================================================================
Wolf::Wolf(ConstructionPackage constructionPackage): Enemy(constructionPackage)
{
	// サイズを設定後にオブジェクト⇒姿勢制御の順で初期化
	//scale *= 2.0f;
	setSize(D3DXVECTOR3(1.0f, 0.87f, 1.0f) * 2.0f);
	Object::initialize(&position);
	enemyData->defaultDirection = slip(enemyData->defaultDirection, axisY.direction);
	postureControl(axisZ.direction, enemyData->defaultDirection, 1);

	// パーツの作成
	for (int i = 0; i < PARTS_MAX; i++)
	{
		parts[i] = new Object;
		parts[i]->position = PARTS_OFFSET_POS[i];
	}

	// アニメーションマネージャを初期化
	animationManager = new WolfAnimationManager(PARTS_MAX, this, &parts[0]);
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
	case CHASE:  chase(frameTime);  break;
	case PATROL: patrol(frameTime); break;
	case REST:   rest(frameTime);   break;
	case DIE:    die(frameTime);    break;
	}
	Enemy::postprocess(frameTime);

	// パーツアニメーションの更新
	animationManager->update(frameTime);
}


//=============================================================================
// 追跡ステート
//=============================================================================
void::Wolf::chase(float frameTime)
{
	float distance = between2VectorLength(position, *movingTarget);

	if (distance < 7.0f && canAttack)
	{
		attack();
	}

	Enemy::chase(frameTime);
}


//=============================================================================
// 警戒ステート
//=============================================================================
void::Wolf::patrol(float frameTime)
{
	if (onGround && isArraved || isDestinationLost)
	{
		setDebugDestination();		// デバッグ用目的地を設定
		isDestinationLost = false;	// 目的地はロストしていない
		searchPath();
	}

	Enemy::patrol(frameTime);
}


//=============================================================================
// 休憩ステート
//=============================================================================
void::Wolf::rest(float frameTime)
{

}


//=============================================================================
// ツリー攻撃ステート
//=============================================================================
void Wolf::attackTree(float frameTime)
{
	Enemy::attackTree(frameTime);
}


//=============================================================================
// 死亡ステート
//=============================================================================
void::Wolf::die(float frameTime)
{
	if (cntTimeDie > DIE_STATE_RENDERING_TIME)
	{
		EnemyManager::wolfBodyRenderer->unRegisterObjectByID(parts[BODY]->id);
		EnemyManager::wolfBodyRenderer->updateAccessList();
		EnemyManager::wolfArmRenderer->unRegisterObjectByID(parts[ARM]->id);
		EnemyManager::wolfArmRenderer->updateAccessList();
	}
	Enemy::die(frameTime);
}


//=============================================================================
// Getter
//=============================================================================
Object* Wolf::getParts(int type)
{
	return parts[type];
}


//=============================================================================
// Setter
//=============================================================================
