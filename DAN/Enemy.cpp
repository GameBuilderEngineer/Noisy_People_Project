//-----------------------------------------------------------------------------
// エネミー基底クラス [Enemy.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#include "Enemy.h"
#include "ImguiManager.h"

using namespace enemyNS;

int Enemy::numOfEnemy = 0;


//=============================================================================
// コンストラクタ
//=============================================================================
Enemy::Enemy(): StaticMeshObject(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2))
{
	numOfEnemy++;

	difference = DIFFERENCE_FIELD;			//フィールド補正差分
	onGravity = true;
	sphereCollider.initialize(&position, staticMesh->mesh);
	radius = sphereCollider.getRadius();
	friction = 1.0f;

	staticMesh->materials->Diffuse;
  	int a = 1;
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
	// 摩擦係数のリセット
	friction = 1.0f;
	isGoingMoveOperation = false;

	StaticMeshObject::update();



#ifdef _DEBUG
	moveOperation();
	controlCamera(frameTime);
#endif
	groundingWork();			// 接地処理
	updatePhysicalBehavior();	// 物理挙動
	updatePhysics(frameTime);	// 物理の更新
}


//=============================================================================
// 描画処理
//=============================================================================
void Enemy::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	StaticMeshObject::render(
		*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// 物理挙動の更新
//=============================================================================
void Enemy::updatePhysicalBehavior()
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

	// 移動入力がないとき重力以外の加速度を切る
	if (isGoingMoveOperation == false)
	{
		acceleration.x = 0.0f;
		acceleration.z = 0.0f;
	}

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
void Enemy::groundingWork()
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
//重力設定(レイ)
//[内容]レイを使用して重力源のメッシュの法線を取りだしその法線を重力方向とする
//[引数]
// D3DXVECTOR3* attractorPosition	：引力発生地点
// LPD3DXMESH _attractorMesh		：（レイ処理用）引力発生メッシュ
// D3DXMATRIX _attractorMatrix		：（レイ処理用）引力発生行列
//[戻値]なし
//=============================================================================
void Enemy::configurationGravityWithRay(D3DXVECTOR3* attractorPosition, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	// レイ判定を行うために必要な要素をセット
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	//// 重力線を作成
	//D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	//gravityRay.initialize(position, gravityDirection);		//重力レイの初期化

	//// レイ判定
	//if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	//{// 重力線上にポリゴンが衝突していた場合、ポリゴン法線を重力方向とし、姿勢を法線と一致させる。
	//	setGravity(-gravityRay.normal, GRAVITY_FORCE);
	//}
	//else
	//{// 衝突ポリゴンが存在しない場合は、重力線をそのまま重力方向とし、姿勢を重力線と一致させる。
	//	setGravity(gravityDirection, GRAVITY_FORCE);
	//}
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

	StaticMeshObject::initialize(&(D3DXVECTOR3)START_POSITION);
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
		ImGui::Checkbox("onRender", &onRender);											//描画有効化フラグ
		ImGui::Checkbox("onLighting", &onLighting);										//光源処理フラグ
		ImGui::Checkbox("onTransparent", &onTransparent);								//透過フラグ

		ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);					//透過値の操作有効フラグ
	}
#endif // _DEBUG
}


//=============================================================================
// Getter
//=============================================================================
int Enemy::getNumOfEnemy(){ return numOfEnemy; }
EnemyData* Enemy::getEnemyData() { return enemyData; }


//=============================================================================
// Setter
//=============================================================================
void Enemy::setDataToEnemy(EnemyData* _enemyData){	enemyData = _enemyData;}
void Enemy::setCamera(Camera* _camera) { camera = _camera; }
