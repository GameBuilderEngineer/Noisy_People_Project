//-----------------------------------------------------------------------------
// エネミー基底クラス [Enemy.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#include "Enemy.h"
#include "ImguiManager.h"
#include "Sound.h"
using namespace enemyNS;
using namespace stateMachineNS;


int Enemy::numOfEnemy = 0;			// エネミーの総数
#ifdef _DEBUG
int Enemy::debugEnemyID = -1;		// デバッグするエネミーのID
#endif//_DEBUG


//=============================================================================
// コンストラクタ
//=============================================================================
Enemy::Enemy(StaticMesh* _staticMesh, enemyNS::EnemyData* _enemyData)
{
	numOfEnemy++;								// エネミーの数を加算
	enemyData = _enemyData;						// エネミーデータをセット

	// ステートマシンの初期化
	stateMachine.initialize(enemyData->defaultState);

	// ナビゲーションメッシュ
	naviMesh = NavigationMesh::getNaviMesh();
	edgeList = NULL;
	naviFaceIndex = -1;
	shouldSearchPath = false;

	// 移動
	onMove = false;
	movingTarget = NULL;
	destination = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 初期設定
	onGravity = true;							// 重力有効化
	difference = DIFFERENCE_FIELD;				// フィールド補正差分を設定

	// フラグ初期化
	onGround = true;
	onGroundBefore = true;
	onJump = false;
	jumping = false;

	// オブジェクト初期化
	position = enemyData->position;
	axisZ.direction = enemyData->direction;
	sphereCollider.initialize(&position, _staticMesh->mesh);
	radius = sphereCollider.getRadius();
	Object::initialize(&position);
	postureControl(axisZ.direction, enemyData->defaultDirection, 1);

#ifdef RENDER_SENSOR
	hearingSphere[0].initialize(&centralPosition, NOTICEABLE_DISTANCE_PLAYER[enemyData->type]);
	hearingSphere[1].initialize(&centralPosition, NOTICEABLE_DISTANCE_PLAYER[enemyData->type] * SHOT_SOUND_SCALE);
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

	numOfEnemy--;
}


//=============================================================================
// 更新処理
//=============================================================================
void Enemy::update(float frameTime)
{
#ifdef _DEBUG
	if (enemyData->enemyID == debugEnemyID)
	{
		moveOperation();			// 移動操作
		controlCamera(frameTime);	// カメラ操作
		if (input->wasKeyPressed('7'))
		{
			destination = position;
		}
		if (input->wasKeyPressed('8'))
		{
			naviMesh->pathSearch(&edgeList, &naviFaceIndex, centralPosition, destination);
			naviMesh->dumpEdgeList();
			naviMesh->affectToEdgeVertex();
			naviMesh->debugRenderEdge(edgeList);
			setMovingTarget(&destination);
			setMove(true);
		}
	}
#ifdef RENDER_SENSOR
	debugSensor();				// 視界
	// プレイヤーへの注視レイを描画する
	gazePlayer.initialize(centralPosition, *player->getCentralPosition() - centralPosition);
	gazePlayer.color = D3DXCOLOR(255, 0, 0, 255);
#endif// RENDER_SENSOR
#endif// _DEBUG

	// センサー
	if (canUseSensor)
	{
		if (eyeSensor(gameMasterNS::PLAYER_1P) || earSensor(gameMasterNS::PLAYER_1P))
		{
			isNoticedPlayer1 = true;
		}
		if (eyeSensor(gameMasterNS::PLAYER_2P) || earSensor(gameMasterNS::PLAYER_2P))
		{
			isNoticedPlayer2 = true;
		}
		canUseSensor = false;
	}

	// 経路探索
	if (shouldSearchPath)
	{
		naviMesh->pathSearch(&edgeList, &naviFaceIndex, centralPosition, *movingTarget);
		shouldSearchPath = false;
	}

	// 移動
	if (onMove)
	{
		steering();
	}
	// 接地処理
	grounding();
	// 物理挙動
	physicalBehavior();
	// 物理の更新
	updatePhysics(frameTime);
	// オブジェクトの更新
	Object::update();
	// 中心座標の更新
	updateCentralCood();
	// ステートの更新
	int stateNumber = stateMachine.run(frameTime, enemyData->type);
	// エネミーデータの更新
	enemyData->state = stateNumber;
	enemyData->position = position;
	enemyData->direction = axisZ.direction;
}


//=============================================================================
// 事前処理
//=============================================================================
void Enemy::preprocess(float frameTime)
{
	// センサー更新計算処理
	sensorTime += frameTime;
	if (sensorTime > SENSOR_UPDATE_INTERVAL[enemyData->type])
	{
		sensorTime = 0.0f;
		canUseSensor = true;
	}

	onJump = false;
	friction = 1.0f;		// 摩擦係数初期化
	acceleration *= 0.0f;	// 加速度を初期化
}


//=============================================================================
// 視覚センサー
//=============================================================================
bool Enemy::eyeSensor(int playerType)
{
	float distanceBetweenPlayerAndEnemy;	// プレイヤーとの距離
	float horizontalAngle;					// 正面方向とプレイヤーの水平角度
	float verticalAngle;					// 正面方向とプレイヤーの垂直角度

	// プレイヤーが視認可能な距離にいるか調べる
	distanceBetweenPlayerAndEnemy = D3DXVec3Length(&(*player[playerType].getCentralPosition() - centralPosition));
	if (distanceBetweenPlayerAndEnemy > VISIBLE_DISTANCE[enemyData->type])
	{// 視認可能距離ではなかった
		return false;
	}	

	// プレイヤーが水平視野角内に入ったか調べる
	D3DXVECTOR3 horizontalVecToPlayer = *player[playerType].getCentralPosition() - centralPosition;
	slip(horizontalVecToPlayer, axisY.direction);
	slip(horizontalVecToPlayer, reverseAxisY.direction);
	formedRadianAngle(&horizontalAngle, axisZ.direction, horizontalVecToPlayer);
	if (horizontalAngle > HORIZONTAL_HALF_VIEWING_ANGLE[enemyData->type])
	{// 視野角外だった
		return false;
	}

	// プレイヤーが垂直視野角内に入ったか調べる
	D3DXVECTOR3 verticalVecToPlayer = *player[playerType].getCentralPosition() - centralPosition;
	slip(verticalVecToPlayer, axisX.direction);
	slip(verticalVecToPlayer, reverseAxisX.direction);
	formedRadianAngle(&verticalAngle, axisZ.direction, verticalVecToPlayer);
	if (verticalAngle > VERTICAL_HALF_VIEWING_ANGLE[enemyData->type])
	{// 視野角外だった
		return false;
	}

	// プレイヤーとの間に障害物がないか調べる
	Ray ray;
	ray.initialize(centralPosition, *player[playerType].getCentralPosition() - centralPosition);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	{
		if (ray.distance < distanceBetweenPlayerAndEnemy)
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
	float distanceBetweenPlayerAndEnemy = D3DXVec3Length(&(*player[playerType].getPosition() - position));

	// プレイヤーの物音を聞き取れる距離（聴覚距離）範囲内か調べる
	if (distanceBetweenPlayerAndEnemy < NOTICEABLE_DISTANCE_PLAYER[enemyData->type])
	{// 聴覚距離範囲内である
		return true;
	}

	// プレイヤーの銃声を聞き取れる距離か調べる
	if (distanceBetweenPlayerAndEnemy < NOTICEABLE_DISTANCE_PLAYER[enemyData->type] * SHOT_SOUND_SCALE &&
		player[playerType].getWhetherShot())
	{// 銃声を聞き取れた
		return true;
	}

	return false;
}


//=============================================================================
// ステアリング
//=============================================================================
void Enemy::steering()
{
	D3DXVECTOR3 moveDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (edgeList != NULL && edgeList->isEnpty())
	{
		// エッジリストの破棄
		edgeList->terminate();
		SAFE_DELETE(edgeList);
	}

	if(edgeList == NULL)
	{
		// 目的地に直線移動
		moveDirection = destination - position;
		D3DXVec3Normalize(&moveDirection, &moveDirection);
	}
	else
	{	
		// ナビメッシュによる移動ベクトル生成
		naviMesh->steering(&moveDirection, &naviFaceIndex, centralPosition, edgeList);
	}

	acceleration += moveDirection * MOVE_ACC[enemyData->type];
}


//=============================================================================
// 接地処理
//=============================================================================
void Enemy::grounding()
{
	onGroundBefore = onGround;
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(centralPosition, gravityDirection);
	bool hit = gravityRay.rayIntersect(attractorMesh, *attractorMatrix);

	if (hit == false)
	{// エネミーは地面の無い空中にいる
		onGround = false;
		return;
	}

	if (radius + difference >= gravityRay.distance)
	{// エネミーは地上に接地している
		onGround = true;

		if (onJump)
		{
			// めり込み補正（現在位置 + 重力方向 * めり込み距離）
			setPosition(centralPosition + gravityRay.direction * (gravityRay.distance - radius));
			// 重力方向に落ちるときだけ移動ベクトルのスリップ（面方向へのベクトル成分の削除）
			if (speed.y < 0) setSpeed(slip(speed, gravityRay.normal));
		}
		else
		{
			// めり込み補正（現在位置 + 重力方向 * めり込み距離）
			setPosition(position + gravityRay.direction * (gravityRay.distance - radius));
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
// 中心座標系の更新
//=============================================================================
void Enemy::updateCentralCood()
{
	centralPosition = position + sphereCollider.getCenter();
	D3DXMatrixTranslation(&centralMatrixWorld, centralPosition.x, centralPosition.y, centralPosition.z);
	axisX.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._11, centralMatrixWorld._12, centralMatrixWorld._13));
	axisY.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._21, centralMatrixWorld._22, centralMatrixWorld._23));
	axisZ.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._31, centralMatrixWorld._32, centralMatrixWorld._33));
	reverseAxisX.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._11, centralMatrixWorld._12, centralMatrixWorld._13));
	reverseAxisY.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._21, centralMatrixWorld._22, centralMatrixWorld._23));
	reverseAxisZ.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._31, centralMatrixWorld._32, centralMatrixWorld._33));
}


//=============================================================================
// 重力発生メッシュ（接地メッシュ）の設定
//=============================================================================
void Enemy::setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;
}


//=============================================================================
// 重力発生メッシュ（接地メッシュ）の設定
//=============================================================================
void Enemy::setPlayer(Player* _player)
{
	player = _player;
}


//=============================================================================
// エネミーのオブジェクトの数を初期化
//=============================================================================
void Enemy::resetNumOfEnemy()
{
	numOfEnemy = 0;
}


//=============================================================================
// 足音の処理
//=============================================================================
void Enemy::footsteps(D3DXVECTOR3 playerPos, int playerID)
{
	float distance = D3DXVec3Length(&(position - playerPos));
	float volume = 0.0f;
	if (distance < DISTANCE_MAX)
	{
		volume = (DISTANCE_MAX - distance) / DISTANCE_MAX;
	}

	SoundInterface::S3D->SetVolume(playParameters[playerID], volume);
}


//=============================================================================
// Getter
//=============================================================================
int Enemy::getEnemyID() { return enemyData->enemyID; }
int Enemy::getNumOfEnemy() { return numOfEnemy; }
EnemyData* Enemy::getEnemyData() { return enemyData; }
BoundingSphere*  Enemy::getSphereCollider() { return &sphereCollider; }
D3DXVECTOR3*  Enemy::getCentralPosition() { return &centralPosition; }
D3DXMATRIX* Enemy::getCentralMatrixWorld() { return &centralMatrixWorld; }


//=============================================================================
// Setter
//=============================================================================
void Enemy::setMove(bool setting) { onMove = setting; }
void Enemy::setMovingTarget(D3DXVECTOR3* _target) { movingTarget = _target; }


// [デバッグ]
#pragma region Debug
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
		acceleration += moveDirection * MOVE_ACC[enemyData->type];
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

			if (height == -1 && width == -1) { eyeAngleRay[0].initialize(centralPosition, direction); }
			if (height == -1 && width == 1) { eyeAngleRay[1].initialize(centralPosition, direction); }
			if (height == 1 && width == -1) { eyeAngleRay[2].initialize(centralPosition, direction); }
			if (height == 1 && width == 1) { eyeAngleRay[3].initialize(centralPosition, direction); }
		}
	}

	// 視界レイを白で描画
	for (int i = 0; i < 4; i++)
	{
		eyeAngleRay[i].color = D3DXCOLOR(255, 255, 255, 155);
	}

	// 聴覚距離を描画
	// バウンディングスフィアで描画する

	bool sound = false;
	if (canUseSensor)
	{
		if (eyeSensor(gameMasterNS::PLAYER_1P) || eyeSensor(gameMasterNS::PLAYER_2P))
		{
			// 視界に入ったら赤点滅
			for (int i = 0; i < 4; i++)
			{
				eyeAngleRay[i].color = D3DXCOLOR(255, 0, 0, 255);
				sound = true;
			}
		}

		if (earSensor(gameMasterNS::PLAYER_1P) || earSensor(gameMasterNS::PLAYER_2P))
		{

			sound = true;
		}
	}

	if (sound)
	{
		// 音を鳴らす
		FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
		PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, GAME_SE_LIST::SE_GAME_EnemyAttack, false ,NULL,false,NULL,true, filterParameters };
		SoundInterface::SE->playSound(&playParameters);	//SE再生
	}
}

//===================================================================================================================================
// ImGUIへの出力
//===================================================================================================================================
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
