//-----------------------------------------------------------------------------
// エネミー基底クラス [Enemy.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#include "Enemy.h"
#include "EnemyManager.h"
#include "ImguiManager.h"
#include "Sound.h"
#include "ItemManager.h"
#include <cassert>
using namespace enemyNS;
using namespace stateMachineNS;

// 静的メンバ変数
int Enemy::numOfEnemy = 0;			// エネミーの総数
#ifdef _DEBUG
int Enemy::debugEnemyID = -1;		// デバッグするエネミーのID
#endif//_DEBUG

#pragma region [Basic Process]
//=============================================================================
// コンストラクタ
//=============================================================================
Enemy::Enemy(ConstructionPackage constructionPackage)
{
	numOfEnemy++;	// エネミーの数を加算

	enemyData = constructionPackage.enemyData;
	staticMesh = constructionPackage.staticMesh;
	gameMaster = constructionPackage.gameMaster;
	player = constructionPackage.player;
	attractorMesh = constructionPackage.attractorMesh;
	attractorMatrix = constructionPackage.attractorMatrix;
	markRenderer = EnemyManager::markRenderer;// ●グローバルでとれるようにしたからこの必要がない

	// エネミーデータをオブジェクトにセット
	position = enemyData->position;
	axisZ.direction = enemyData->defaultDirection;

	// ステートマシンの初期化
	stateMachine.initialize(enemyData->state);

	// ブラックボードの初期化
	ZeroMemory(isNoticingPlayer, sizeof(bool) * gameMasterNS::PLAYER_NUM);
	ZeroMemory(noticedTimePlayer, sizeof(float) * gameMasterNS::PLAYER_NUM);
	movingTarget = NULL;
	attackTarget = NULL;
	attentionDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	isPayingNewAttention = false;
	nextIndexOfRoute = 0;

	// センサーの初期化
	canSense = false;
	cntSensorInterval = 0.0f;
	ZeroMemory(isSensingPlayer, sizeof(bool) * gameMasterNS::PLAYER_NUM);
	ZeroMemory(distanceToPlayer, sizeof(float) * gameMasterNS::PLAYER_NUM);

	// 経路探索の初期化
	canSearch = false;
	cntSensorInterval = 0.0f;
	naviMesh = NavigationMesh::getNaviMesh();
	edgeList = NULL;
	naviFaceIndex = -1;

	// 攻撃の初期化
	canAttack = false;
	cntAttackInterval = 0.0f;
	isAttacking = false;
	attackTime = 0.0f;
	attackDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 移動の初期化
	destination = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	moveDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQuaternionIdentity(&moveMotion);
	isArraved = false;
	movingTime = 0.0f;
	isDestinationLost = false;
	onJump = false;
	jumping = false;

	// コリジョンと物理挙動の初期化
	sphereCollider.initialize(&position, staticMesh->mesh);
	difference = DIFFERENCE_FIELD;
	onGround = false;
	onGroundBefore = false;
	groundNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	isHitPlayer = false;
	friction = 1.0f;

	// オブジェクト初期化(initialize()は派生クラス)
	onGravity = true;
	radius = sphereCollider.getRadius();
	{// オブジェクトタイプと衝突対象の指定
		using namespace ObjectType;
		treeCell.type = ENEMY;
		treeCell.target = PLAYER | ENEMY | BULLET | TREE;
	}

	// 初期ステートごとのセットアップ
	switch (enemyData->state)
	{
	case CHASE:
		isNoticingPlayer[gameMasterNS::PLAYER_1P] = true;
		isNoticingPlayer[gameMasterNS::PLAYER_2P] = true;
		break;
	case PATROL:
		isDestinationLost = true;
		break;
	case REST:
		break;
	case ATTACK_TREE:
		break;
	case DIE:
		break;
	}

#ifdef _DEBUG
#ifdef RENDER_SENSOR
	hearingSphere[0].initialize(&center, NOTICEABLE_DISTANCE_PLAYER[enemyData->type]);
	hearingSphere[1].initialize(&center, NOTICEABLE_DISTANCE_PLAYER[enemyData->type] * SHOT_SOUND_SCALE);
#endif
#endif
}


//=============================================================================
// デストラクタ
//=============================================================================
Enemy::~Enemy()
{
	if (edgeList != NULL)
	{
		edgeList->terminate();
		SAFE_DELETE(edgeList);
	}

	// 追跡マークの削除
	deleteMark();

	numOfEnemy--;
}


//=============================================================================
// 事前処理
//=============================================================================
void Enemy::preprocess(float frameTime)
{
	// センサーのプレイヤー感知フラグを初期化
	isSensingPlayer[gameMasterNS::PLAYER_1P] = false;
	isSensingPlayer[gameMasterNS::PLAYER_2P] = false;

	// センサーを再実行可能にするまでの間隔を計算
	cntSensorInterval += frameTime;
	if (cntSensorInterval > SENSOR_UPDATE_INTERVAL[enemyData->type])
	{
		cntSensorInterval = 0.0f;
		canSense = true;
	}

	// 経路探索を再実行可能にするまでの間隔を計算
	cntPathSearchInterval += frameTime;
	if (cntPathSearchInterval >= PATH_SEARCH_INTERVAL_WHEN_CHASE)
	{
		cntPathSearchInterval = 0.0f;
		canSearch = true;
	}

	// 攻撃を再実行可能にするまでの間隔を計算
	cntAttackInterval += frameTime;
	if (cntAttackInterval >= ATTACK_INTERVAL[enemyData->type])
	{
		cntAttackInterval = 0.0f;
		canAttack = true;
	}

	// 目的地ロストを判定するために移動時間を計算
	movingTime += frameTime;
	if (movingTime > MOVE_LIMIT_TIME)
	{
		movingTime = 0.0f;
		isDestinationLost = true;
	}

	onJump = false;
	friction = 1.0f;		// 摩擦係数初期化
	acceleration *= 0.0f;	// 加速度を初期化

	if (enemyData->type == TIGER || enemyData->type == WOLF)
	{
		animationManager->switchDefault();
	}


#ifdef _DEBUG
	if (enemyData->state == CHASE)
	{
		// 追跡ステートで両プレイヤーに認識されていない状態はエラー
		assert(isNoticingPlayer[gameMasterNS::PLAYER_1P] || isNoticingPlayer[gameMasterNS::PLAYER_2P]);
	}
#ifdef RENDER_SENSOR
	debugSensor();
#endif// RENDER_SENSOR
#endif// _DEBUG

#ifdef _DEBUG
	if (enemyData->enemyID == debugEnemyID)
	{
		controlCamera(frameTime);
		moveOperation();
		enemyData->state = stateMachineNS::ENEMY_STATE::REST;

		if (input->wasKeyPressed('8'))
		{
			destination = player->center;
			movingTarget = &destination;
		}
		if (input->wasKeyPressed('7'))
		{
			searchPath();
		}
		if (input->isKeyDown('M'))
		{
			move(frameTime);
		}
		if (input->wasKeyPressed('U'))
		{
			naviMesh->isHitGrounding(NULL, &faceNumber, center);
		}
	}
#endif// _DEBUG
}


//=============================================================================
// 事後処理
//=============================================================================
void Enemy::postprocess(float frameTime)
{
	// 接地処理
	grounding();
	// 壁ずり処理
	wallScratch();
	// 物理挙動
	physicalBehavior();
	// 物理の更新
	updatePhysics(frameTime);
	// 姿勢の更新
	updatePosture(frameTime);
	// オブジェクトの更新
	Object::update();
	// 到着判定
	checkArrival();
	// ブラックボードの更新
	updataBlackBoard(frameTime);
	// ステートの更新
	int stateNumber = stateMachine.run(frameTime, this);
	// エネミーデータの更新
	enemyData->state = stateNumber;
	enemyData->position = position;
	enemyData->direction = axisZ.direction;
	// 自動破棄を行うか確認
	checkAutoDestruction();

	//for (int i = 0; i < 2; i++)
	//{
	//	footsteps(player[i].position, i);
	//}
}
#pragma endregion


#pragma region [Action Query]
//=============================================================================
// センサー
//=============================================================================
bool Enemy::sensor()
{
	canSense = false;
	using namespace gameMasterNS;
	distanceToPlayer[PLAYER_1P] = between2VectorLength(center, player[gameMasterNS::PLAYER_1P].center);
	distanceToPlayer[PLAYER_2P] = between2VectorLength(center, player[gameMasterNS::PLAYER_2P].center);
	if (eyeSensor(PLAYER_1P) || earSensor(PLAYER_1P)) { isSensingPlayer[PLAYER_1P] = true; }
	if (eyeSensor(PLAYER_2P) || earSensor(PLAYER_2P)) { isSensingPlayer[PLAYER_2P] = true; }

	if(isSensingPlayer[PLAYER_1P] || isSensingPlayer[PLAYER_2P])
	{
		return true;
	}
	return false;
}


//=============================================================================
// パス検索
//=============================================================================
void Enemy::searchPath()
{
	canSearch = false;
	isArraved = false;

	// 移動ターゲットの現在位置までのエッジリストを取得
	//naviMesh->pathSearch(&edgeList, &naviFaceIndex, center, *movingTarget);
	// 移動ターゲットの現在位置を目的地に設定
	destination = *movingTarget;

#ifdef _DEBUG
	if (enemyData->enemyID == debugEnemyID)
	{
		// 移動ターゲットの現在位置までのエッジリストを取得
		naviMesh->pathSearch(&edgeList, &naviFaceIndex, center, *movingTarget);
		// 移動ターゲットの現在位置を目的地に設定
		destination = *movingTarget;

		naviMesh->debugEdgeList = &edgeList;
	}
#endif
}


//=============================================================================
// 攻撃
//=============================================================================
void Enemy::attack()
{
	canAttack = false;
	cntAttackInterval = 0.0f;
	isAttacking = true;
}


//=============================================================================
// 移動
//=============================================================================
void Enemy::move(float frameTime)
{
	if (isAttacking)
	{
		attacking(frameTime);
		if (enemyData->type == TIGER || enemyData->type == WOLF)
		{
			animationManager->switchAttack();
		}
	}
	else
	{
		steering(frameTime);
		if (enemyData->type == TIGER || enemyData->type == WOLF)
		{
			animationManager->switchMove();
		}
	}
}
#pragma endregion


#pragma region [Action Implementation]
//=============================================================================
// 視覚センサー
//=============================================================================
bool Enemy::eyeSensor(int playerType)
{
	float horizontalAngle;					// 正面方向とプレイヤーの水平角度
	float verticalAngle;					// 正面方向とプレイヤーの垂直角度

	if (distanceToPlayer[playerType] > VISIBLE_DISTANCE[enemyData->type])
	{// 視認可能距離ではなかった
		return false;
	}	

	// プレイヤーが水平視野角内に入ったか調べる
	D3DXVECTOR3 horizontalVecToPlayer = player[playerType].center - center;
	slip(horizontalVecToPlayer, axisY.direction);
	slip(horizontalVecToPlayer, reverseAxisY.direction);
	formedRadianAngle(&horizontalAngle, axisZ.direction, horizontalVecToPlayer);
	if (horizontalAngle > HORIZONTAL_HALF_VIEWING_ANGLE[enemyData->type])
	{// 視野角外だった
		return false;
	}

	// プレイヤーが垂直視野角内に入ったか調べる
	D3DXVECTOR3 verticalVecToPlayer = player[playerType].center - center;
	slip(verticalVecToPlayer, axisX.direction);
	slip(verticalVecToPlayer, reverseAxisX.direction);
	formedRadianAngle(&verticalAngle, axisZ.direction, verticalVecToPlayer);
	if (verticalAngle > VERTICAL_HALF_VIEWING_ANGLE[enemyData->type])
	{// 視野角外だった
		return false;
	}

	// プレイヤーとの間に障害物がないか調べる
	Ray ray;
	ray.initialize(center, player[playerType].center - center);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	{
		if (ray.distance < distanceToPlayer[playerType])
		{// 接地メッシュがあった
			return false;
		}
	}

	return true;	// プレイヤーが視野に入っている
}


//=============================================================================
// 聴覚センサー
//=============================================================================
bool Enemy::earSensor(int playerType)
{
	// プレイヤーの物音を聞き取れる距離（聴覚距離）範囲内か調べる
	if (distanceToPlayer[playerType] < NOTICEABLE_DISTANCE_PLAYER[enemyData->type])
	{// 聴覚距離範囲内である
		return true;
	}

	// プレイヤーの銃声を聞き取れる距離か調べる
	if (distanceToPlayer[playerType] < NOTICEABLE_DISTANCE_PLAYER[enemyData->type] * SHOT_SOUND_SCALE &&
		player[playerType].getBulletManager()->getIsLaunched())
	{// 銃声を聞き取れた
		return true;
	}

	return false;
}


//=============================================================================
// 攻撃
//=============================================================================
void Enemy::attacking(float frameTime)
{
	if (isAttacking == false) { return; }

	// 攻撃ベクトル作成
	if (attackTime == 0.0f)
	{
		attackDirection = attackTarget->center - center;
		slip(attackDirection, groundNormal);
		D3DXVec3Normalize(&attackDirection, &attackDirection);
		// 初速を与える
		speed += attackDirection * ATTACK_SPEED[enemyData->type];
	}
	// 攻撃時間終了判定
	attackTime += frameTime;
	if (attackTime >= ATTACK_TIME[enemyData->type])
	{
		stopAttacking();
		isAttacking = false;
		attackTime = 0.0f;
	}
	if (onGround)
	{
		// 速度がある程度維持されるよう加速度を加算（地面摩擦係数の打ち消し）
		acceleration += attackDirection * ATTACK_SPEED[enemyData->type] * MULTIPLICATION_TO_MAKE_ATTACK_ACC;
	}
}


//=============================================================================
// ステアリング
//=============================================================================
void Enemy::steering(float frameTime)
{
	D3DXVECTOR3 newMoveDirection;
	D3DXVECTOR3 tempDirection;


	// ナビメッシュによる移動ベクトル生成（ステアリング）
	naviMesh->steering(&moveDirection, &naviFaceIndex, center, destination, &edgeList);

	////moveDirection += newMoveDirection;

	////if (oldMoveDirection == D3DXVECTOR3(0, 0, 0))
	////{
	////	oldMoveDirection = axisZ.direction;
	////}
	//if (moveDirection == D3DXVECTOR3(0, 0, 0))
	//{
	//	tempDirection = axisZ.direction;
	//}
	//else
	//{
	//	tempDirection = moveDirection;
	//}

	////カーブモーション
	//float rotationRadian;
	//if (formedRadianAngle(&rotationRadian, tempDirection, newMoveDirection))
	//{
	//	Base::anyAxisRotationSlerp(&moveMotion, groundNormal, rotationRadian, 1);
	//}
	//D3DXMATRIX matrix;
	//D3DXMatrixIdentity(&matrix);
	//D3DXMatrixRotationQuaternion(&matrix, &moveMotion);
	//D3DXVec3TransformCoord(&moveDirection, &tempDirection, &matrix);

	//moveDirection = newMoveDirection;
	
	// 加速度に加算
	acceleration += moveDirection * MOVE_ACC[enemyData->type];
}
#pragma endregion


#pragma region [Postprocess Implementation]
//=============================================================================
// 接地処理
//=============================================================================
void Enemy::grounding()
{
	onGroundBefore = onGround;
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(center, gravityDirection);
	bool hit = gravityRay.rayIntersect(attractorMesh, *attractorMatrix);
	groundNormal = gravityRay.normal;

	if (hit == false)
	{// エネミーは地面の無い空中にいる
		onGround = false;
		return;
	}

	float halfHeight = size.y / 2;
	if (halfHeight + difference >= gravityRay.distance)
	{// エネミーは地上に接地している
		onGround = true;

		if (onJump)
		{
			// めり込み補正（現在位置 + 重力方向 * めり込み距離）
			setPosition(center + gravityRay.direction * (gravityRay.distance - halfHeight));
			// 重力方向に落ちるときだけ移動ベクトルのスリップ（面方向へのベクトル成分の削除）
			if (speed.y < 0) setSpeed(slip(speed, gravityRay.normal));
		}
		else
		{
			// めり込み補正（現在位置 + 重力方向 * めり込み距離）
			setPosition(position + gravityRay.direction * (gravityRay.distance - halfHeight));
			// 移動ベクトルのスリップ（面方向へのベクトル成分の削除）
			setSpeed(slip(speed, gravityRay.normal));
			// 直前フレームで空中にいたならジャンプ終了とする
			if (onGroundBefore == false) jumping = false;
		}
	}
	else
	{// エネミーは地面のある空中にいる
		onGround = false;
	}
}


//=============================================================================
// 壁ずり処理
//=============================================================================
void Enemy::wallScratch()
{
	ray.update(center, D3DXVECTOR3(speed.x, 0, speed.z));

	// 壁ずり処理
	if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	{
		// めり込み補正
		position += ray.normal * (radius / 2 - ray.distance);
		// 移動ベクトルのスリップ（面方向へのベクトル成分の削除）
		speed = slip(speed, ray.normal);
	}
}


//=============================================================================
// 物理挙動
//=============================================================================
void Enemy::physicalBehavior()
{
	// 接地していないときのみ重力加速度をかける
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(position, gravityDirection);
	if (onGround == false)
	{
		setGravity(gravityDirection, GRAVITY_FORCE);
	}

	//if (onGround)
	//{
	//	// 地上にいる場合は重力方向も切る
	//	// ↓これを外すと最後に加速度.yに入っていた重力加速度がスリップして坂道滑り続ける
	//	acceleration.y = 0.0f;
	//}

	// 空中に浮くタイミングで加速度切る
	if (onGround == false && onGroundBefore)
	{
		acceleration *= 0.0f;
	}

	// 着地するタイミングで速度が低下する
	if (onGround && onGroundBefore == false)
	{
		friction *= GROUND_FRICTION;
	}

	// 地上摩擦係数
	if (onGround)
	{
		friction *= MOVE_FRICTION;
	}

	//// 停止
	//float speedPerSecond = D3DXVec3Length(&speed);
	//if (isExecutingMoveOperation == false &&
	//	speedPerSecond < STOP_SPEED)
	//{
	//	speed *= 0.0f;
	//}

	// 落下速度限界の設定
	if (speed.y < -FALL_SPEED_MAX)
	{
		speed.y = -FALL_SPEED_MAX;
	}
}


//=============================================================================
// 物理の更新
//=============================================================================
void Enemy::updatePhysics(float frameTime)
{
	// 加速度の影響を速度に与える
	speed += acceleration * frameTime;
	// 速度に摩擦の影響を与える
	speed *= friction;
	// 速度の影響を位置に与える
	position += speed * frameTime;
}


//=============================================================================
// 姿勢の更新
//=============================================================================
void Enemy::updatePosture(float frameTime)
{
	moveDirection = slip(moveDirection, axisY.direction);
	// moveDirectionは移動関数で更新される
	postureControl(axisZ.direction, moveDirection, frameTime * 6/*1秒の6倍を指定 = 0.1秒で姿勢変更*/);
}


//=============================================================================
// 到着判定
//=============================================================================
void Enemy::checkArrival()
{
	float distance = between2VectorLength(destination, position);

	if (distance <= ARRIVAL_DISTANCE)
	{
		isArraved = true;
	}
	// ●通り過ぎ対策をしたい
	//D3DXVECTOR3 point = nearestPointOnLine(enemyData->position/*1フレーム前の座標*/, position, destination);
}


//=============================================================================
// ブラックボードの更新
//=============================================================================
void Enemy::updataBlackBoard(float frameTime)
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		// プレイヤーの認識
		if (isSensingPlayer[i])
		{
			isNoticingPlayer[i] = true;
			noticedTimePlayer[i] = 0.0f;
		}

		// プレイヤーの記憶
		if (isNoticingPlayer[i])
		{
			noticedTimePlayer[i] += frameTime;
			if (noticedTimePlayer[i] >= PLAYER_MEMORIZED_TIME[enemyData->type])
			{
				noticedTimePlayer[i] = 0.0f;
				isNoticingPlayer[i] = false;
			}
		}
	}

	isPayingNewAttention = false;
}
#pragma endregion


#pragma region [Prepare State Change]
//=============================================================================
// 追跡ステートの準備
//=============================================================================
void Enemy::prepareChase()
{
	// 最初から追跡ステートであるエネミーを作るとこの関数を経由しない
	// 様々な問題が出てくるので注意されたし

	cntPathSearchInterval = 0.0f;
	playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_EnemyActive, false, NULL, false, NULL };
	SoundInterface::SE->playSound(&playParameters);

	// 追跡マークの作成
	createMark();
}


//=============================================================================
// 警戒ステートの準備
//=============================================================================
void Enemy::preparePatrol()
{
	isDestinationLost = true;

	// 追跡マークの削除
	deleteMark();
}


//=============================================================================
// 休憩ステートの準備
//=============================================================================
void Enemy::prepareRest()
{
	// 追跡マークの削除
	deleteMark();
}


//=============================================================================
// ツリー攻撃ステートの準備
//=============================================================================
void Enemy::prepareAttackTree()
{
	setAttackTarget(enemyData->targetTree);
	setMovingTarget(enemyData->targetTree->getPosition());

	// 追跡マークの削除
	deleteMark();
}


//=============================================================================
// 死亡ステートの準備
//=============================================================================
void Enemy::prepareDie()
{
	// 死亡エフェクトの再生
	deathEffect = new DeathEffect(&center);
	deathEffect->scale *= DEATH_EFFECT_SCALE[enemyData->type];
	effekseerNS::play(0, deathEffect);

	// 追跡マークの削除
	deleteMark();
}
#pragma endregion


#pragma region [State]
//=============================================================================
//「追跡」ステート
//=============================================================================
void Enemy::chase(float frameTime)
{
	if (canSense)
	{
		sensor();
	}

	if (canSearch)
	{
		setPlayerChaseTarget();
		searchPath();
	}

	//D3DXQUATERNION quaternion;
	//D3DXQuaternionIdentity(&quaternion);
	//Base::postureControl(&quaternion, D3DXVECTOR3(0, 0, 0), player[chasingPlayer].position - position, 1.0f);
	//D3DXMATRIX matrix;
	//D3DXMatrixIdentity(&matrix);
	//D3DXMatrixRotationQuaternion(&matrix, &quaternion);
	////D3DXVec3TransformCoord(&markDirection, &markDirection, &matrix);
	//markFront->rotation.x = 

	// 追跡マーク（表）の更新
	markFront->position = position + D3DXVECTOR3(0.0f, MARK_FLOATING_HEIGHT, 0.0f);
	formedRadianAngle(&markFront->rotation.y, D3DXVECTOR3(0.0f, 0.0f, 1.0f), player[chasingPlayer].position - position);
	if (position.x < player[chasingPlayer].position.x) { markFront->rotation.y = -markFront->rotation.y; }
	// 追跡マーク（裏）の更新
	markBack->position = position + D3DXVECTOR3(0.0f, MARK_FLOATING_HEIGHT, 0.0f);
	formedRadianAngle(&markBack->rotation.y, D3DXVECTOR3(0.0f, 0.0f, 1.0f), player[chasingPlayer].position - position);
	if (position.x < player[chasingPlayer].position.x)
	{
		markBack->rotation.y = D3DX_PI - markBack->rotation.y;
	}
	else
	{
		markBack->rotation.y = -(D3DX_PI - markBack->rotation.y);
	}

	// 移動
	move(frameTime);
}


//=============================================================================
//「警戒」ステート
//=============================================================================
void Enemy::patrol(float frameTime)
{
	if (canSense)
	{
		sensor();
	}

	if (isPayingNewAttention)
	{
		static const float TEMP_DISTANCE = 20.0f;
		D3DXVec3Normalize(&attentionDirection, &attentionDirection);
		destination = position + attentionDirection * TEMP_DISTANCE;

		isDestinationLost = false;
		setMovingTarget(&destination);
		searchPath();
	}

	move(frameTime);
}


//=============================================================================
//「休憩」ステート
//=============================================================================
void Enemy::rest(float frameTime)
{

}


//=============================================================================
//「ツリー攻撃」ステート
//=============================================================================
void Enemy::attackTree(float frameTime)
{
	if (canSense)
	{
		sensor();
	}

	if (canSearch)
	{
		//setPlayerChaseTarget();
		searchPath();
	}

	move(frameTime);
}


//=============================================================================
//「死亡」ステート
//=============================================================================
void Enemy::die(float frameTime)
{
	// 死ぬ寸前の振動
	speed.x += (rand() % 8) / 10.0f;
	speed.x -= 0.4f;
	speed.z += (rand() % 8) / 10.0f;
	speed.z -= 0.4f;

	if (enemyData->type == TIGER || enemyData->type == WOLF)
	{
		animationManager->switchDead();
	}


	// 死亡時
	if (cntTimeDie > DIE_STATE_TIME)
	{
		enemyData->isAlive = false;
		enemyData->deadTime = gameMaster->getGameTime();

		// アイテムドロップ
		if (rand() % ITEM_DROP_PROBABILITY_DENOMINATOR[enemyData->type] == 0)
		{
			ItemManager* itemManager = ItemManager::get();
			itemNS::ItemData itemData;
			itemData.itemID = itemManager->issueNewItemID();
			itemData.type = itemNS::BATTERY;
			itemData.defaultPosition = position;
			itemData.defaultDirection = axisZ.direction;
			itemManager->createItem(itemData);
		}
	}

	// 死亡ステート時間のカウントアップ
	cntTimeDie += frameTime;
}
#pragma endregion


#pragma region [Sound]
//=============================================================================
// サウンドプレイパラメータの取得
//=============================================================================
PLAY_PARAMETERS Enemy::getPlayParameters(SE_3D soundType, int enemyType)
{
	PLAY_PARAMETERS parameter = {
		ENDPOINT_VOICE_LIST::ENDPOINT_S3D,	//エンドポイントボイスID
		NULL,								// サウンドID ↓で設定
		false,								// ループ
		NULL,								// 再生速度
		true,								// 3Dサウンドか
		-1,									// プレイヤーID
	};

	switch (soundType)
	{
	//------
	// 足音
	//------
	case FOOT_STEPS_SE:
		switch (enemyType)
		{
		case WOLF:
			parameter.soundId = S3D_LIST::S3D_FOOTSTEP_01;// 仮
			break;

		case TIGER:
			parameter.soundId = S3D_LIST::S3D_FOOTSTEP_02;// 仮
			break;

		case BEAR:
			parameter.soundId = S3D_LIST::S3D_FOOTSTEP_03;// 仮
			break;
		}
		break;

	//--------
	// 攻撃音
	//--------
	case ATTACK_SE:
	{
	}
	break;

	//--------
	// 死亡音
	//--------
	case DIE_SE:
	{
	}
	break;

	}// switch(soundType)

	return parameter;
}


//=============================================================================
// 足音の処理
//=============================================================================
void Enemy::footsteps(D3DXVECTOR3 playerPos, int playerID)
{
	PLAY_PARAMETERS playParameter = getPlayParameters(FOOT_STEPS_SE, enemyData->type);
	playParameter.playerID = playerID;

	// よくわからんから蔡よろしく!

	//float distance = D3DXVec3Length(&(position - playerPos));
	//float volume = 0.0f;
	//if (distance < DISTANCE_MAX)
	//{
	//	volume = (DISTANCE_MAX - distance) / DISTANCE_MAX;
	//}

	//SoundInterface::S3D->SetVolume(footStepsParameters[playerID], volume);
}
#pragma endregion


#pragma region [Other]
//=============================================================================
// リスポーンと破棄を行うか自己判定
//=============================================================================
void Enemy::reset()
{
	if (isDestinationLost)
	{
		position = enemyData->position;
		axisZ.direction = enemyData->defaultDirection;
		enemyData->state = enemyData->defaultState;
	}

}


//=============================================================================
// 自動破棄を行うか確認
//=============================================================================
void Enemy::checkAutoDestruction()
{
	if (position.y <= AUTO_DESTRUCTION_HEIGHT)
	{// 島の下に落下した場合
		enemyData->isAlive = false;
	}
}


//=============================================================================
// 適切なプレイヤーをターゲットに設定する
//=============================================================================
void Enemy::setPlayerChaseTarget()
{
	if (isNoticingPlayer[gameMasterNS::PLAYER_1P] && isNoticingPlayer[gameMasterNS::PLAYER_2P] == false)
	{
		setMovingTarget(&player[gameMasterNS::PLAYER_1P].position);
		attackTarget = &player[gameMasterNS::PLAYER_1P];
		chasingPlayer = gameMasterNS::PLAYER_1P;
	}
	else if (isNoticingPlayer[gameMasterNS::PLAYER_1P] == false && isNoticingPlayer[gameMasterNS::PLAYER_2P])
	{
		setMovingTarget(&player[gameMasterNS::PLAYER_2P].position);
		attackTarget = &player[gameMasterNS::PLAYER_2P];
		chasingPlayer = gameMasterNS::PLAYER_2P;
	}
	else if (isNoticingPlayer[gameMasterNS::PLAYER_1P] && isNoticingPlayer[gameMasterNS::PLAYER_2P])
	{
		if (distanceToPlayer[gameMasterNS::PLAYER_1P] < distanceToPlayer[gameMasterNS::PLAYER_2P])
		{
			setMovingTarget(&player[gameMasterNS::PLAYER_1P].position);
			attackTarget = &player[gameMasterNS::PLAYER_1P];
			chasingPlayer = gameMasterNS::PLAYER_1P;
		}
		else
		{
			setMovingTarget(&player[gameMasterNS::PLAYER_2P].position);
			attackTarget = &player[gameMasterNS::PLAYER_2P];
			chasingPlayer = gameMasterNS::PLAYER_2P;

		}
	}
}


//=============================================================================
// デバッグ用目的地設定
//=============================================================================
void Enemy::setDebugDestination()
{
	// 目的地のセット
	static const float TEMP_DISTANCE = 20.0f;
	D3DXVECTOR3 tempDir;
	tempDir = D3DXVECTOR3((float)(rand() % 1000), 0, float(rand() % 1000));
	tempDir -= D3DXVECTOR3(500, 0, 500);
	D3DXVec3Normalize(&tempDir, &tempDir);
	tempDir *= TEMP_DISTANCE;
	destination = position + tempDir;

#if 0	// 山頂を目的地とする
	destination = D3DXVECTOR3(-30, 158, 35);
#endif
	setMovingTarget(&destination);
}


//=============================================================================
// 巡回路リングバッファの更新
//=============================================================================
void Enemy::updatePatrolRoute()
{
	if (onGround && isArraved)
	{// 到着していたら次の座標に移動ターゲットを更新する
		nextIndexOfRoute %= enemyData->numRoute;
		movingTarget = &enemyData->patrolRoute[nextIndexOfRoute];
	}

	if (isDestinationLost)
	{//●

	}
}


//=============================================================================
// 追跡マークの作成
//=============================================================================
void Enemy::createMark()
{
	// 表
	markFront = new enemyChaseMarkNS::StandardChaseMark(position + D3DXVECTOR3(0.0f, MARK_FLOATING_HEIGHT, 0.0f));
	markFront->scale = MARK_SCALE[enemyData->type];
	markRenderer->generateMark(markFront);
	// 裏
	markBack = new enemyChaseMarkNS::StandardChaseMark(position + D3DXVECTOR3(0.0f, MARK_FLOATING_HEIGHT, 0.0f));
	markBack->scale = MARK_SCALE[enemyData->type];
	markBack->rotation = D3DXVECTOR3(D3DX_PI / 2.0f, 0, 0);
	markRenderer->generateMark(markBack);
}


//=============================================================================
// 追跡マークの破棄
//=============================================================================
void Enemy::deleteMark()
{
	if (markFront != NULL)
	{
		markRenderer->deleteMark(markFront);
		markFront = NULL;	// ダングリングポインタにならないはずだが仕様変更等に備えて
	}

	if (markBack != NULL)
	{
		markRenderer->deleteMark(markBack);
		markBack = NULL;	// ダングリングポインタにならないはずだが仕様変更等に備えて
	}
}
#pragma endregion


#pragma region [Getter & Setter]
//=============================================================================
// Getter
//=============================================================================
int Enemy::getEnemyID() { return enemyData->enemyID; }
int Enemy::getNumOfEnemy() { return numOfEnemy; }
EnemyData* Enemy::getEnemyData() { return enemyData; }
BoundingSphere*  Enemy::getSphereCollider() { return &sphereCollider; }
LPD3DXMESH Enemy::getMesh() { return box->mesh; }
bool Enemy::getNoticedOfPlayer(int playerType) { return isNoticingPlayer[playerType]; }
bool Enemy::getIsAttacking() { return isAttacking; }
int Enemy::getChasingPlayer() { return chasingPlayer; };


//=============================================================================
// Setter
//=============================================================================
void Enemy::resetNumOfEnemy()
{
	numOfEnemy = 0;
}

void Enemy::damage(int _damage)
{
	enemyData->hp -= _damage;
	if (enemyData->hp <= 0)
	{
		enemyData->hp = 0;
	}
}

void Enemy::stopAttacking()
{
	isAttacking = false;
	attackTime = 0.0f;
	speed *= ATTACKED_FRICTION;// 減速
}

void Enemy::setAttention(D3DXVECTOR3 setting)
{
	attentionDirection = setting;
	isPayingNewAttention = true;
}

void Enemy::setMovingTarget(D3DXVECTOR3* _target)
{
	movingTarget = _target;
}

void Enemy::setAttackTarget(Object* _target)
{
	attackTarget = _target;
}
#pragma endregion


#pragma region [Debug]
#ifdef _DEBUG
//=============================================================================
// デバッグ環境を設定
//=============================================================================
void Enemy::setDebugEnvironment()
{
	device = getDevice();
	input = getInput();
	keyTable = KEY_TABLE_1P;
	reverseValueXAxis = CAMERA_SPEED;		//操作Ｘ軸
	reverseValueYAxis = CAMERA_SPEED;		//操作Ｙ軸
}


//=============================================================================
// カメラの操作と更新
//=============================================================================
void Enemy::controlCamera(float frameTime)
{
	// 操作軸反転操作
	if (input->wasKeyPressed(keyTable.reverseCameraX))reverseValueXAxis *= -1;
	if (input->wasKeyPressed(keyTable.reverseCameraY))reverseValueYAxis *= -1;
	// マウス操作
	camera->rotation(D3DXVECTOR3(0, 1, 0), (float)(input->getMouseRawX() * reverseValueXAxis));
	camera->rotation(camera->getHorizontalAxis(), (float)(input->getMouseRawY() * reverseValueYAxis));
	// コントローラ操作
	if (input->getController()[0]->checkConnect()) {
		camera->rotation(D3DXVECTOR3(0, 1, 0), input->getController()[0]->getRightStick().x*0.1f*frameTime*reverseValueXAxis);
		camera->rotation(camera->getHorizontalAxis(), input->getController()[0]->getRightStick().y*0.1f*frameTime*reverseValueYAxis);
	}
}


//=============================================================================
// 移動操作
//=============================================================================
void Enemy::moveOperation()
{
	//前へ進む
	if (input->isKeyDown(keyTable.front)) {
		move(D3DXVECTOR2(0, -1), camera->getDirectionX(), camera->getDirectionZ());
	}
	//後ろへ進む
	if (input->isKeyDown(keyTable.back)) {
		move(D3DXVECTOR2(0, 1), camera->getDirectionX(), camera->getDirectionZ());
	}
	//左へ進む
	if (input->isKeyDown(keyTable.left)) {
		move(D3DXVECTOR2(-1, 0), camera->getDirectionX(), camera->getDirectionZ());
	}
	//右へ進む
	if (input->isKeyDown(keyTable.right))
	{
		move(D3DXVECTOR2(1, 0), camera->getDirectionX(), camera->getDirectionZ());
	}
}


//=============================================================================
// 移動
//=============================================================================
void Enemy::move(D3DXVECTOR2 operationDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ)
{
	if (operationDirection.x == 0 && operationDirection.y == 0)return;//入力値が0以下ならば移動しない
	//Y軸方向への成分を削除する
	D3DXVECTOR3 front = slip(cameraAxisZ, axisY.direction);
	D3DXVECTOR3 right = slip(cameraAxisX, axisY.direction);
	D3DXVec3Normalize(&front, &front);//正規化
	D3DXVec3Normalize(&right, &right);//正規化

	//操作方向をカメラのXZ方向に準拠した移動ベクトルへ変換する
	D3DXVECTOR3 moveDirection = operationDirection.x*right + -operationDirection.y*front;
	if (onGround)
	{
		acceleration += moveDirection * MOVE_ACC[enemyData->type] * 3; 
	}
	else
	{
		acceleration += moveDirection * MOVE_ACC[enemyData->type] * AIR_MOVE_ACC_MAGNIFICATION;
	}

	//姿勢制御
	postureControl(getAxisZ()->direction, moveDirection, 0.1f);
}


//=============================================================================
// デバッグセンサー
//=============================================================================
void Enemy::debugSensor()
{
	D3DXQUATERNION	rayQuaternion1, rayQuaternion2;
	D3DXMATRIX		rayMatrix;
	D3DXVECTOR3		direction;

	// 視界のレイを作る
	for (int height = -1; height < 2; height += 2)
	{
		for (int width = -1; width < 2; width += 2)
		{
			D3DXQuaternionIdentity(&rayQuaternion1);
			D3DXQuaternionIdentity(&rayQuaternion2);
			D3DXMatrixIdentity(&rayMatrix);
			direction = axisZ.direction;

			D3DXQuaternionRotationAxis(&rayQuaternion1,
				&axisX.direction, height * VERTICAL_HALF_VIEWING_ANGLE[enemyData->type]);

			D3DXQuaternionRotationAxis(&rayQuaternion2,
				&axisY.direction, width * HORIZONTAL_HALF_VIEWING_ANGLE[enemyData->type]);

			rayQuaternion2 *= rayQuaternion1;
			D3DXMatrixRotationQuaternion(&rayMatrix, &rayQuaternion2);
			D3DXVec3TransformCoord(&direction, &direction, &rayMatrix);

			if (height == -1 && width == -1) { eyeAngleRay[0].initialize(center, direction); }
			if (height == -1 && width == 1) { eyeAngleRay[1].initialize(center, direction); }
			if (height == 1 && width == -1) { eyeAngleRay[2].initialize(center, direction); }
			if (height == 1 && width == 1) { eyeAngleRay[3].initialize(center, direction); }
		}
	}

	// 視界レイを白で描画
	for (int i = 0; i < 4; i++)
	{
		eyeAngleRay[i].color = D3DXCOLOR(255, 255, 255, 155);
	}

	bool sound = false;
	if (canSense)
	{
		if (sensor())
		{
			// 視界に入ったら赤点滅
			for (int i = 0; i < 4; i++)
			{
				eyeAngleRay[i].color = D3DXCOLOR(255, 0, 0, 255);
			}

			sound = true;
		}

		if (sound)
		{
			// 音を鳴らす
			//playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_AnnounceTelop, false, NULL, false, NULL };
			//SoundInterface::SE->playSound(&playParameters);	//SE再生
		}

	}
}


//=============================================================================
// ImGUIへの出力
//=============================================================================
void Enemy::outputGUI()
{

	if (ImGui::CollapsingHeader("EnemyInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//位置
		ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//回転
		ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);						//スケール
		ImGui::SliderFloat("radius", &radius, 0, limitTop);								//半径
		ImGui::SliderFloat("alpha", &alpha, 0, 255);									//透過値
		ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);						//速度
		ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//加速度
		ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);					//重力
		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));
		ImGui::Checkbox("onGravity", &onGravity);										//重力有効化フラグ
		ImGui::Checkbox("onActive", &onActive);											//アクティブ化フラグ
	}
}
#endif
#pragma endregion
