//-----------------------------------------------------------------------------
// タイガークラス [Tiger.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/21
//-----------------------------------------------------------------------------
#include "Tiger.h"
#include "EnemyManager.h"
#include "TigerAnimation.h"
using namespace enemyNS;
using namespace stateMachineNS;
using namespace tigerNS;

//=============================================================================
// コンストラクタ
//=============================================================================
Tiger::Tiger(ConstructionPackage constructionPackage) : Enemy(constructionPackage)
{
	// サイズを設定後にオブジェクト⇒姿勢制御の順で初期化
	setSize(D3DXVECTOR3(2.0f, 2.8, 2.0f));
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
	animationManager = new TigerAnimationManager(PARTS_MAX, this, &parts[0]);

	// バレットマネージャにレンダラーを設定
	bulletManager = new TigerBulletManager(EnemyManager::tigerBulletRenderer);
}


//=============================================================================
// デストラクタ
//=============================================================================
Tiger::~Tiger()
{
	// パーツの破棄
	for (int i = 0; i < PARTS_MAX; i++)
	{
		SAFE_DELETE(parts[i]);
	}
	// アニメーションマネージャの破棄
	SAFE_DELETE(animationManager);
	// バレットマネージャの破棄
	SAFE_DELETE(bulletManager);
}


//=============================================================================
// 更新処理
//=============================================================================
void Tiger::update(float frameTime)
{
	Enemy::preprocess(frameTime);
	switch (enemyData->state)
	{
	case CHASE:  chase(frameTime);  break;
	case PATROL: patrol(frameTime); break;
	case REST:   rest(frameTime);   break;
	case DIE:    die(frameTime);    break;
	}
	//if (enemyData->enemyID == debugEnemyID)
	//{
	//	if (input->wasKeyPressed('N'))
	//	{
	//		shot(&player[0]);
	//	}
	//}
	Enemy::postprocess(frameTime);

	// パーツアニメーションの更新
	animationManager->update(frameTime);

	// バレットの更新
	muzzlePosition = center;// ●仮
	bulletManager->update(frameTime);
}


//=============================================================================
// 追跡ステート
//=============================================================================
void::Tiger::chase(float frameTime)
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
void::Tiger::patrol(float frameTime)
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
void::Tiger::rest(float frameTime)
{

}


//=============================================================================
// ツリー攻撃ステート
//=============================================================================
void Tiger::attackTree(float frameTime)
{
	Enemy::attackTree(frameTime);
}


//=============================================================================
// ショット
//=============================================================================
void Tiger::shot(Player* target)
{
	Ray temp;
	temp.start = muzzlePosition;
	temp.direction = target->center - muzzlePosition;
	D3DXVec3Normalize(&temp.direction, &temp.direction);
	bulletManager->shoot(temp);
}


//=============================================================================
// 死亡ステート
//=============================================================================
void::Tiger::die(float frameTime)
{
	if (cntTimeDie > DIE_STATE_RENDERING_TIME)
	{
		EnemyManager::tigerBodyRenderer->unRegisterObjectByID(parts[BODY]->id);
		EnemyManager::tigerBodyRenderer->updateAccessList();
		EnemyManager::tigerGunRenderer->unRegisterObjectByID(parts[GUN]->id);
		EnemyManager::tigerGunRenderer->updateAccessList();
		EnemyManager::tigerLegLRenderer->unRegisterObjectByID(parts[LEG_L]->id);
		EnemyManager::tigerLegLRenderer->updateAccessList();
		EnemyManager::tigerLegRRenderer->unRegisterObjectByID(parts[LEG_R]->id);
		EnemyManager::tigerLegRRenderer->updateAccessList();
	}
	Enemy::die(frameTime);
}


//=============================================================================
// Getter
//=============================================================================
Object* Tiger::getParts(int type)
{
	return parts[type];
}


//=============================================================================
// Setter
//=============================================================================
