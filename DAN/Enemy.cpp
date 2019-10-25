//-----------------------------------------------------------------------------
// エネミー基底クラス [Enemy.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#include "Enemy.h"
#include "ImguiManager.h"
using namespace enemyNS;

int Enemy::numOfEnemy = 0;			// エネミーの数


//=============================================================================
// コンストラクタ
//=============================================================================
Enemy::Enemy(StaticMesh* _staticMesh, enemyNS::EnemyData* _enemyData)
{
	numOfEnemy++;							// エネミーの数を加算
	enemyData = _enemyData;					// エネミーデータをセット
	difference = DIFFERENCE_FIELD;			// 必要性要検討

	onGravity = true;
	position = enemyData->position;
	axisZ.direction = enemyData->direction;
	sphereCollider.initialize(&position, _staticMesh->mesh);
	radius = sphereCollider.getRadius();
	friction = 1.0f;

	Object::initialize(&position);
	Object::axisZ.direction = axisZ.direction;
	postureControl(axisZ.direction, enemyData->defaultDirection, 1);
}


//=============================================================================
// デストラクタ
//=============================================================================
Enemy::~Enemy()
{
	numOfEnemy--;
}


//=============================================================================
// 更新処理
//=============================================================================
void Enemy::update(float frameTime)
{
	// 事前処理
	previousWork();

#ifdef _DEBUG
	//moveOperation();
	//controlCamera(frameTime);
#endif

	grounding();				// 接地処理
	physicalBehavior();			// 物理挙動
	updatePhysics(frameTime);	// 物理の更新
	Object::update();			// オブジェクトの更新

	postureControl(axisZ.direction, enemyData->defaultDirection, 1.0f * frameTime);

	// エネミーデータの更新
	enemyData->position = position;
	enemyData->direction = axisZ.direction;
	if (enemyData->state == DEAD)
	{
		enemyData->isAlive = false;
	}
}


void Enemy::previousWork()
{
	friction = 1.0f;
	isGoingMoveOperation = false;
}


//=============================================================================
// 物理挙動の更新
//=============================================================================
void Enemy::physicalBehavior()
{
	//------------
	// 加速度処理
	//------------
	// 重力処理
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(position, gravityDirection);
	if (onGround == false)
	{
		setGravity(gravityDirection, GRAVITY_FORCE);
	}

	//// 移動入力がないとき重力以外の加速度を切る
	//if (isGoingMoveOperation == false)
	//{
	//	acceleration.x = 0.0f;
	//	acceleration.z = 0.0f;
	//}

	// 空中に浮くタイミングで加速度はリセットされる
	if (onGround == false && onGroundBefore)
	{
		acceleration *= 0.0f;
	}

	//----------
	// 速度処理
	//----------
	// 着地するタイミングで速度が低下する
	if (onGround && onGroundBefore == false)
	{
		friction *= 0.1f;
	}

	// 地上摩擦係数
	if (onGround)
	{
		friction *= MOVE_FRICTION;
	}

	//// 停止
	//if (speed.x < 0.1f && speed.x > -0.1f &&
	//	speed.y < 0.1f && speed.y > -0.1f &&
	//	speed.z < 0.1f && speed.z > -0.1f)
	//{
	//	speed *= 0.0f;
	//}

	//// 落下速度限界の設定
	//if (speed.y > -54.0f)
	//{
	//	speed.y = -54.0f;
	//}
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
// 接地処理
//=============================================================================
void Enemy::grounding()
{
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(position, gravityDirection);

	//if (oldFrameGroundPosition.y > position.y)
	//{
	//	//gravityRay.update(position, -gravityDirection);
	//	//if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	//	//{
	//	//	setPosition(position + gravityRay.direction * (gravityRay.distance + radius));
	//	//}

	//	setSpeed(D3DXVECTOR3(0.0f, 30.0f, 0.0f));
	//}

	onGroundBefore = onGround;
	if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	{
		if (radius + difference >= gravityRay.distance)
		{// 地上
			onGround = true;
			// めり込み補正（現在位置 + 重力方向 * めり込み距離）
			setPosition(position + gravityRay.direction * (gravityRay.distance - radius));
			// 移動ベクトルのスリップ（面方向へのベクトル成分の削除）
			setSpeed(slip(speed, gravityRay.normal));			
		}
		else
		{// 空中（地面がある場所で空中）
			onGround = false;
		}
	}
	else
	{// 空中（地面がない）
		onGround = false;
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
	isGoingMoveOperation = true;

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
		acceleration = moveDirection * MOVE_ACC;
	}
	else
	{
		acceleration = moveDirection * MOVE_ACC / 10.0f;
	}

	//姿勢制御
	postureControl(getAxisZ()->direction, moveDirection, 0.1f);
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
	camera->setUpVector(axisY.direction);
	camera->update();
}


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

	Object::initialize(&(D3DXVECTOR3)START_POSITION);
}


//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void Enemy::outputGUI()
{
#ifdef _DEBUG

	//ImGui::Text(sceneName.c_str());

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
#endif // _DEBUG
}


//=============================================================================
// Getter
//=============================================================================
int Enemy::getEnemyID() { return enemyData->enemyID; }
int Enemy::getNumOfEnemy(){ return numOfEnemy; }
EnemyData* Enemy::getEnemyData() { return enemyData; }


//=============================================================================
// Setter
//=============================================================================
void Enemy::setDataToEnemy(EnemyData* _enemyData){	enemyData = _enemyData;}
void Enemy::setCamera(Camera* _camera) { camera = _camera; }
void Enemy::resetNumOfEnemy() { numOfEnemy = 0; }
