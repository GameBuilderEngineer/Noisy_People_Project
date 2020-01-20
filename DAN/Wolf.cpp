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
		parts[i] = new EnemyParts;
		parts[i]->position = PARTS_OFFSET_POS[i];
		parts[i]->setEnemy(this);
	}
	parts[BODY]->setRenderer(EnemyManager::wolfBodyRenderer);
	parts[ARM]->setRenderer(EnemyManager::wolfArmRenderer);

	// アニメーションマネージャを初期化
	animationManager = new WolfAnimationManager(PARTS_MAX, this, (Object**)&parts[0]);
	
	for (int i = 0; i < 2; i++)
	{
		playParmeters[i] =
			getPlayParameters(FOOT_STEPS_SE, enemyData->type);
		playParmeters[i].loop = true;
		playParmeters[i].playerID = i;
		SoundInterface::S3D->playSound(&playParmeters[i]);
		SoundInterface::S3D->SetVolume(playParmeters[i], 0.0f);
	}
}


//=============================================================================
// デストラクタ
//=============================================================================
Wolf::~Wolf()
{
	// パーツの破棄
	for (int i = 0; i < PARTS_MAX; i++)
	{
		SAFE_DELETE(parts[i]);
	}
	// アニメーションマネージャの破棄
	SAFE_DELETE(animationManager);

	for (int i = 0; i < 2; i++)
	{
		SoundInterface::S3D->stopSound(playParmeters[i]);
	}
}


//=============================================================================
// 更新処理
//=============================================================================
void Wolf::update(float frameTime)
{
	Enemy::preprocess(frameTime);
	switch (enemyData->state)
	{
	case CHASE:			chase(frameTime);		break;
	case PATROL:		patrol(frameTime);		break;
	case REST:			rest(frameTime);		break;
	case ATTACK_TREE:	attackTree(frameTime);	break;
	case DIE:			die(frameTime);			break;
	}
	Enemy::postprocess(frameTime);

	// パーツアニメーションの更新
	animationManager->update(frameTime);

	// 3Dサウンド（移動音）の再生
	if (animationManager->canPlayMoveSound)
	{
		animationManager->canPlayMoveSound = false;

		// ここで再生
		for (int i = 0; i < 2; i++)
		{
			// ボリューム
			float distance = D3DXVec3Length(&(position - player[i].position));
			float volume = 0.0f;
			if (distance < DISTANCE_MAX)
			{
				volume = (DISTANCE_MAX - distance) / DISTANCE_MAX;
			}

			if (enemyData->state != REST || enemyData->state != DIE)
			{
				// ボリューム調整
				SoundInterface::S3D->SetVolume(playParmeters[i], volume);
			}
		}
	}
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

	// 移動
	move(frameTime);
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
	// ボリューム調整
	SoundInterface::S3D->SetVolume(playParmeters[0], 0.0f);
	SoundInterface::S3D->SetVolume(playParmeters[1], 0.0f);
 
	Enemy::rest(frameTime);
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
	
	// ボリューム調整
	SoundInterface::S3D->SetVolume(playParmeters[0], 0.0f);
	SoundInterface::S3D->SetVolume(playParmeters[1], 0.0f);

	Enemy::die(frameTime);
}


//=============================================================================
// Getter
//=============================================================================
EnemyParts* Wolf::getParts(int type)
{
	return parts[type];
}


//=============================================================================
// Setter
//=============================================================================
