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
		parts[i] = new EnemyParts;
		parts[i]->position = PARTS_OFFSET_POS[i];
		parts[i]->setEnemy(this);
	}
	parts[BODY]->setRenderer(EnemyManager::tigerBodyRenderer);
	parts[GUN]->setRenderer(EnemyManager::tigerGunRenderer);
	parts[LEG_L]->setRenderer(EnemyManager::tigerLegLRenderer);
	parts[LEG_R]->setRenderer(EnemyManager::tigerLegRRenderer);

	// アニメーションマネージャを初期化
	animationManager = new TigerAnimationManager(PARTS_MAX, this, (Object**)&parts[0]);

	// バレットマネージャを作成
	bulletManager = new TigerBulletManager(player);

	// 戦術選択に個性を付ける
	tacticsPersonality = rand() % 4;
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


#define TIGER_SOUND_MAX  (40)
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
#ifdef _DEBUG
	if (enemyData->enemyID == debugEnemyID)
	{
		
		if (input->wasKeyPressed('N'))
		{
		}
	}
#endif
	Enemy::postprocess(frameTime);

	// パーツアニメーションの更新	
	animationManager->update(frameTime);

	// 3Dサウンド（移動音）の再生
	if (animationManager->canPlayMoveSound)
	{
		animationManager->canPlayMoveSound = false;

		PLAY_PARAMETERS tmpPlayParmeters =
			getPlayParameters(FOOT_STEPS_SE, enemyData->type);

		// ここで再生
		for (int i = 0; i < 2; i++)
		{
			tmpPlayParmeters.playerID = i;
			SoundInterface::S3D->playSound(&tmpPlayParmeters);
			// ボリューム
			float distance = D3DXVec3Length(&(position - player[i].position));
			float volume = 0.0f;
			if (distance < TIGER_SOUND_MAX)
			{
				volume = (TIGER_SOUND_MAX - distance) / TIGER_SOUND_MAX;
			}
			// ボリューム調整
			SoundInterface::S3D->SetVolume(tmpPlayParmeters, volume);
		}
	}

	// バレットの更新
	bulletManager->update(frameTime);
}


//=============================================================================
// 追跡ステート
//=============================================================================
void::Tiger::chase(float frameTime)
{
	Enemy::chase(frameTime);

	float distance = between2VectorLength(position, *movingTarget);
	if (distance < 7.0f && canAttack)
	{
		attack();
	}
	else
	{
		// 時間をカウント
		tacticsTime += frameTime;

		if (tacticsTime < 3.0f + tacticsPersonality)
		{
			// 移動
			move(frameTime);
		}
		else if (tacticsTime >= 3.0f + tacticsPersonality && tacticsTime < 6.0f + tacticsPersonality)
		{
			animationManager->inactivateAll();
			animationManager->activate(animationManager->getAnimation(tigerAnimNS::SHOT));
			shot();
			moveDirection = player[chasingPlayer].position - position;
		}
		else if (tacticsTime >= 6.0f + tacticsPersonality)
		{
			tacticsTime = 0.0f;
			animationManager->inactivateAll();
		}
	}
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
	Enemy::rest(frameTime);
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
void Tiger::shot()
{
	D3DXVECTOR3 muzzlePosition;			// 銃口ポジション
	D3DXVECTOR3 connectionPosition;		// 接続部ポジション

	D3DXVec3TransformCoord(&muzzlePosition, &MUZZLE_POSITION, &parts[GUN]->matrixWorld);
	D3DXVec3TransformCoord(&connectionPosition, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &parts[GUN]->matrixWorld);

	Ray temp;
	//temp.start = connectionPosition;
	//temp.direction = muzzlePosition - connectionPosition;
	temp.start = muzzlePosition;
	temp.direction = player[chasingPlayer].position - muzzlePosition;

	temp.distance = 0.0f;
	temp.rayIntersect(attractorMesh, *attractorMatrix);
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
EnemyParts* Tiger::getParts(int type)
{
	return parts[type];
}

TigerBulletManager* Tiger::getBulletMangaer()
{
	return bulletManager;
}

//=============================================================================
// Setter
//=============================================================================
