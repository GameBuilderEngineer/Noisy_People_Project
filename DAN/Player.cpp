//===================================================================================================================================
//【Player.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/24
// [更新日]2019/11/21
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Player.h"
#include "ImguiManager.h"
#include "UtilityFunction.h"
#include "MoveP.h"
#include "NormalState.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace playerNS;

//[基本処理]
#pragma region Basic
//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Player::Player() 
{
	{//オブジェクトタイプと衝突対象の指定
		using namespace ObjectType;
		treeCell.type = PLAYER;
		treeCell.target = PLAYER | ENEMY | TREE;
	}

	ZeroMemory(&keyTable, sizeof(OperationKeyTable));
	onGravity			= true;
	invincibleTimer		= 0.0f;					//無敵時間
	onGround			= false;				//接地判定
	reverseValueXAxis	= CAMERA_SPEED;			//操作Ｘ軸
	reverseValueYAxis	= -CAMERA_SPEED;		//操作Ｙ軸
	onJump				= false;				//ジャンプフラグ
	difference			= DIFFERENCE_FIELD;		//フィールド補正差分
	isShotAble			= true;
	isJumpAble			= true;
	isVisionAble		= true;
	isSkyVisionAble		= true;
	isShiftAble			= true;
	enableShift			= false;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Player::~Player()
{
	SAFE_DELETE(bulletManager);
	SAFE_DELETE(state);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
//プレイヤータイプごとに初期化内容を変更
void Player::initialize(PlayerTable info)
{
	device				= getDevice();
	input				= getInput();
	infomation			= info;
	keyTable			= KEY_TABLE[infomation.playerType];
	Object::initialize(&(D3DXVECTOR3)START_POSITION[infomation.playerType]);

	// コライダの初期化
	bodyCollide.initialize(	&position, staticMeshNS::reference(infomation.modelType)->mesh);
	setRadius(bodyCollide.getRadius());	// メッシュ半径を取得

	//サイズの設定
	setSize(D3DXVECTOR3(0.5f, 1.7f, 0.5f));

	// HPの設定
	hp = MAX_HP;

	//電力の設定
	power				= MAX_POWER;							//キャラクター電力確認用

	//通常状態
	state = new normalNS::NormalState(this);

	//シューティングアクション
	bulletManager		= new BulletManager;

	//デジタルアクション
	//デジタルシフト
	shiftLine.start = position;
	shiftLine.end	= position+axisZ.direction;
	//shiftTimer		= 0.0f;
	//isShifting		= false;

	//再生パラメータの作成
	//memset(playParameters, 0, sizeof(playParameters));
	//FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	shiftStartSE		= { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_ShiftStart, false ,NULL,false,NULL};
	shiftFinishSE		= { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_ShiftFinish, false ,NULL,false,NULL};
	visionSE			= { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Vision, false ,NULL,false,NULL};
	visionStartSE		= { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_VisionStart, false ,NULL,false,NULL};
	visionFinishSE		= { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_VisionFinish, false ,NULL,false,NULL};
	skyVisionStartSE	= { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_SkyVisionStart, false ,NULL,false,NULL};
	skyVisionFinishSE	= { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_SkyVisionStart, false ,NULL,false,NULL};

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Player::update(float frameTime)
{
	// 事前処理
	friction = 1.0f;
	isExecutingMoveOperation = false;
	onJump = false;
	acceleration *= 0.0f;

	//状態別更新
	state->update(frameTime);

/*
	//デジタルシフト更新
	if (shiftTimer > 0) {
		shiftTimer -= frameTime;
		float rate = 1.0f -(shiftTimer/SHIFT_TIME);
		D3DXVec3Lerp(&position, &shiftLine.start, &shiftLine.end,rate);
		isShiftAble = false;//ボタンUIの透明化変化確認のため一時的に
	}
	else if(isShifting){
		isShifting = false;
		isShiftAble = true;//ボタンUIの透明化変化確認のため一時的に
	}
*/

	//バレットマネージャーの更新
	bulletManager->update(frameTime);

	// 操作(状態別)
	state->operation();

#ifdef _DEBUG
	if (input->isKeyDown('H'))
	{// ジャンプ中にHで飛べます
		acceleration.y = 0;
		acceleration.x *= 2;
		acceleration.z *= 2;
		speed.y += 1.5f;
	}
	if (input->wasKeyPressed(keyTable.reset))
	{// リセット
		reset();
	}
#endif // DEBUG

	//物理更新(状態別)
	state->physics();

	//カメラ操作（状態別）
	state->controlCamera();

	//照準方向の更新
	//updateAiming();
	//姿勢制御:狙撃方向へ向く
	//updatePostureByAiming();

	// オブジェクトの更新
	Object::update();

	//狙撃方向の更新
	//updateShooting();
}

//===================================================================================================================================
//【本体以外の他のオブジェクト描画】
//===================================================================================================================================
void Player::otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	bulletManager->render(view,projection,cameraPosition);
#ifdef _DEBUG
	aimingRay.render(collideDistance);
	shootingRay.render(MAX_DISTANCE);
	Object::debugRender();
#endif // _DEBUG
}

//===================================================================================================================================
//【状態遷移】
//===================================================================================================================================
void Player::transState(int next)
{
	state->nextType = next;
	AbstractState* nextState = state->transition();
	SAFE_DELETE(state);
	state = nextState;
}
#pragma endregion

//[衝突]
#pragma region BodyCollision
//===================================================================================================================================
// 接地処理
//===================================================================================================================================
void Player::grounding()
{


	//※※※※※※※※※※※※※※※※※※※※※※※※※※※※
	//※ [変更予定]                                         ※
	//※ メッシュとの衝突判定はフィールドだけではないので、 ※
	//※ 関数で複数のオブジェクトと接地判定ができるようにす ※
	//※ る                                                 ※
	//※※※※※※※※※※※※※※※※※※※※※※※※※※※※
	//bool hit = gravityRay.rayIntersect(attractorMesh, *attractorMatrix);
	//groundNormal = gravityRay.normal;

	//if (hit == false)
	//{// プレイヤーは地面の無い空中にいる
	//	onGround = false;
	//	return;
	//}

	
	if (onGround)
	{
		if (onJump)
		{
			// 重力方向に落ちるときだけ移動ベクトルのスリップ（面方向へのベクトル成分の削除）
			//if (speed.y < 0) speed = slip(speed, gravityRay.normal);
		}
		else {
			dot = D3DXVec3Dot(&gravityRay.normal, &D3DXVECTOR3(0, 1, 0));
			if (dot > 60.0f / 90.0f) {
				speed = slip(speed, gravityRay.normal);
			}
			else {
				speed = slip(speed, gravityRay.normal);
			}
			//	
			//	D3DXVECTOR3 speedDirection;
			//	D3DXVec3Normalize(&speedDirection, &speed);
			//	
			//	speedDirection = slip(speedDirection, gravityRay.normal);
			//	D3DXVec3Normalize(&speedDirection, &speedDirection);
			//
			//	if (speedDirection.y > 0)
			//	{
			//		speedDirection *= -1.0f;
			//	}
			//
			//	speed = D3DXVec3Length(&speed)*speedDirection;
			//}
		}
	}

	//if (radius + difference >= gravityRay.distance)
	//{// プレイヤーは地上に接地している
	//	onGround = true;
	
	//	if (onJump)
	//	{
	//		// めり込み補正（現在位置 + 重力方向 * めり込み距離）
	//		setPosition(center + gravityRay.direction * (gravityRay.distance - radius));
	//		// 重力方向に落ちるときだけ移動ベクトルのスリップ（面方向へのベクトル成分の削除）
	//		if (speed.y < 0) setSpeed(slip(speed, gravityRay.normal));
	//	}
	//	else
	//	{
	//		// めり込み補正（現在位置 + 重力方向 * めり込み距離）
	//		setPosition(position + gravityRay.direction * (gravityRay.distance - radius));
	//		// 移動ベクトルのスリップ（面方向へのベクトル成分の削除）
	//		setSpeed(slip(speed, gravityRay.normal));
	//		// 直前フレームで空中にいたならジャンプ終了とする
	//		if (onGroundBefore == false) jumping = false;
	//	}
	//}
	//else
	//{// プレイヤーは地面のある空中にいる
	//	onGround = false;
	//}
}

bool Player::grounding(LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	onGroundBefore = onGround;
	if (onJump)
	{
		onGround = false;
		return false;
	}

	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(center, gravityDirection);

	bool hit = gravityRay.rayIntersect(mesh, matrix);

	if (hit && size.y/2 + difference >= gravityRay.distance)
	{
		onGround = true;
		position += gravityRay.direction*(gravityRay.distance - size.y / 2);
		return true;
	}

	onGround = false;
	return false;
}

//===================================================================================================================================
// 壁ずり処理
//===================================================================================================================================
void Player::wallScratch()
{
	//スピード
	//ray.update(center, D3DXVECTOR3(speed.x,0, speed.z));
	// 壁ずり処理
	//if (ray.rayIntersect(attractorMesh, *attractorMatrix)&& radius/2 >= ray.distance)
	//{
	//	position += ray.normal*(radius/2-ray.distance);		//めり込み補正
	//	speed = slip(speed, ray.normal);		// 移動ベクトルのスリップ（面方向へのベクトル成分の削除）
	//}
}

//===================================================================================================================================
//【めりこみ補正】
//===================================================================================================================================
void Player::insetCorrection()
{
	//Z軸めり込み補正
	//ray.update(center, axisZ.direction);
	//if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	//{
	//	position += ray.normal*(radius / 2 - ray.distance);
	//}
	////-Z軸めり込み補正
	//ray.update(center, reverseAxisZ.direction);
	//if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	//{
	//	position += ray.normal*(radius / 2 - ray.distance);
	//}
	////X軸めり込み補正
	//ray.update(center, axisX.direction);
	//if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	//{
	//	position += ray.normal*(radius / 2 - ray.distance);
	//}
	////-X軸めり込み補正
	//ray.update(center, reverseAxisX.direction);
	//if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	//{
	//	position += ray.normal*(radius / 2 - ray.distance);
	//}
}

bool Player::insetCorrection(int axisID, float distance, LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	using namespace objectNS;
	switch (axisID)
	{
	case AXIS_X:	return insetCorrection(axisX,			distance, mesh, matrix); break;
	case AXIS_RX:	return insetCorrection(reverseAxisX,	distance, mesh, matrix); break;
	case AXIS_Y:	return insetCorrection(axisY,			distance, mesh, matrix); break;
	case AXIS_RY:	return insetCorrection(reverseAxisY,	distance, mesh, matrix); break;
	case AXIS_Z:	return insetCorrection(axisZ,			distance, mesh, matrix); break;
	case AXIS_RZ:	return insetCorrection(reverseAxisZ,	distance, mesh, matrix); break;
	}
	return false;
}
bool Player::insetCorrection(Ray ray, float distance, LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	if (!ray.rayIntersect(mesh, matrix))return false;

	if (distance >= ray.distance)
	{
		position += ray.normal*(distance - ray.distance);
		return true;
	}
	return false;
}
#pragma endregion

//[物理]
#pragma region PhysicsMovement
//===================================================================================================================================
//【重力設定(レイ)】
//[内容]レイを使用して重力源のメッシュの法線を取りだし、その法線を重力方向とする
//[引数]
// D3DXVECTOR3* attractorPosition	：引力発生地点
// LPD3DXMESH _attractorMesh		：（レイ処理用）引力発生メッシュ
// D3DXMATRIX _attractorMatrix		：（レイ処理用）引力発生行列
//[戻値]なし
//===================================================================================================================================
void Player::configurationGravityWithRay(D3DXVECTOR3* attractorPosition, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{

	//※※※※※※※※※※※※※※※※※※※※※※※※※※※※
	//※ [変更予定]                                         ※
	//※ 重力方向は一様のためこの関数自体機能する必要なし   ※
	//※ バレットにも便宜的に使用しているためバレットの対応 ※
	//※ が完了するまでは実行する。                         ※
	//※                                                    ※
	//※※※※※※※※※※※※※※※※※※※※※※※※※※※※

	//レイ判定を行うために必要な要素をセット
	//attractorMesh = _attractorMesh;
	//attractorMatrix = _attractorMatrix;
}

//===================================================================================================================================
// 物理挙動
//===================================================================================================================================
void Player::physicalBehavior()
{
	//------------
	// 加速度処理
	//------------
	// 重力処理
 	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(position, gravityDirection);
	// 空中で重力をかける（地上では重力をかけない）
	setGravity(gravityDirection, GRAVITY_FORCE);

	// 地上摩擦係数
	if (onGround)
	{
		friction *= MOVE_FRICTION;
	}

	// 落下速度限界の設定
	if (speed.y < -FALL_SPEED_MAX)
	{
		speed.y = -FALL_SPEED_MAX;
	}
}


//===================================================================================================================================
// 物理の更新
//===================================================================================================================================
void Player::updatePhysics(float frameTime)
{
	// 加速度の影響を速度に与える
	speed += acceleration * frameTime;

	if (onGround) {
		//摩擦方向を算出
		D3DXVECTOR3 frictionDirection;
		D3DXVec3Normalize(&frictionDirection,&speed);

		dot = D3DXVec3Dot(&gravityRay.normal, &D3DXVECTOR3(0, 1, 0));
		if (dot > 60.0f / 90.0f) {
			// 速度に摩擦の影響を与える
			speed -= frictionDirection * MOVE_FRICTION * GRAVITY_FORCE * frameTime;
		}
		else {
			// 速度に摩擦の影響を与える
			speed -= frictionDirection * MOVE_FRICTION * GRAVITY_FORCE  * frameTime;

			//斜面滑りベクトル
			D3DXVECTOR3 slipVector;
			D3DXVec3Normalize(&slipVector, &slip(D3DXVECTOR3(0, -1, 0), gravityRay.normal));
			if (D3DXVec3Length(&speed) < MAX_SLIP * dot)
			{
				speed += slipVector * MOVE_ACC * frameTime;
			}
		}
	}

	D3DXVECTOR3 ySpeed			= D3DXVECTOR3(0.0f,speed.y,0.0f);
	D3DXVECTOR3 yAcceleration	= D3DXVECTOR3(0.0f, acceleration.y, 0.0f);

	D3DXVECTOR3 xzSpeed			= D3DXVECTOR3(speed.x,0.0f, speed.z);
	D3DXVECTOR3 xzAcceleration	= D3DXVECTOR3(acceleration.x, 0.0f, acceleration.z);

	//速度に最大速度で制限をかける
	if (D3DXVec3Length(&xzSpeed) > MAX_SPEED)
	{
		speed -= xzAcceleration * frameTime;
	}

	position += speed * frameTime;		// 速度の影響を位置に与える

}
#pragma endregion

//[操作]
#pragma region Operation
//===================================================================================================================================
//【移動操作】
// [処理内容]入力された２次元ベクトルに応じてカメラ情報に基づき、速度へ加算処理を行う。
//===================================================================================================================================
void Player::moveOperation()
{
	MOVEP *MoveP = GetMovePAdr();
	if (!MoveP->IsAttack)
	{
	}
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
	//コントローラスティックによる移動
	if (input->getController()[infomation.playerType]->checkConnect()) {
		move(input->getController()[infomation.playerType]->getLeftStick()*0.001f, camera->getDirectionX(), camera->getDirectionZ());
	}
}

//===================================================================================================================================
//【ジャンプ操作】
// [処理内容]（1）初入力でジャンプ踏切フラグをオン　(2)ジャンプを更新
//===================================================================================================================================
void Player::jumpOperation()
{
	if (input->getMouseRButtonTrigger() || input->getController()[infomation.playerType]->wasButton(BUTTON_JUMP))
	{
		jump();
	}

	if (input->getMouseRButton() || input->getController()[infomation.playerType]->isButton(BUTTON_JUMP))
	{

	}
}


//===================================================================================================================================
//【カメラの操作/更新】
//===================================================================================================================================
void Player::controlCamera(float frameTime)
{
	//操作軸反転操作
	if (input->wasKeyPressed(keyTable.reverseCameraX))reverseValueXAxis *= -1;
	if (input->wasKeyPressed(keyTable.reverseCameraY))reverseValueYAxis *= -1;

	//マウス操作
	camera->rotation(D3DXVECTOR3(0, 1, 0), (float)(input->getMouseRawX() * reverseValueXAxis));
	camera->rotation(camera->getHorizontalAxis(), (float)(input->getMouseRawY() * reverseValueYAxis));
	
	//コントローラ操作
	if (input->getController()[infomation.playerType]->checkConnect()) {
		camera->rotation(D3DXVECTOR3(0, 1, 0), input->getController()[infomation.playerType]->getRightStick().x*0.1f*frameTime*reverseValueXAxis);
		camera->rotation(camera->getHorizontalAxis(), input->getController()[infomation.playerType]->getRightStick().y*0.1f*frameTime*reverseValueYAxis);
	}
	//カメラの注視位置を更新（相対位置にも加算）
	D3DXQUATERNION cameraGazeRelativeQ = camera->relativeQuaternion;
	Base::anyAxisRotation(&cameraGazeRelativeQ, D3DXVECTOR3(0, 1, 0), -90);
	D3DXVec3Normalize(&cameraGazeRelative,&(D3DXVECTOR3)cameraGazeRelativeQ);
	//カメラ注視位置の更新
	cameraGaze = position
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction*CAMERA_GAZE.y;
	//カメラの更新
	camera->update();
	aimingRay.update(camera->position + camera->getDirectionZ(), camera->getDirectionZ());
}
#pragma endregion

//[アクション]
#pragma region Action
//===================================================================================================================================
//【移動】
// [処理内容1]入力された２次元ベクトルに応じてカメラ情報に基づき、速度へ加算処理を行う。
// [処理内容2]移動後の姿勢制御を行う。
//===================================================================================================================================
void Player::move(D3DXVECTOR2 operationDirection,D3DXVECTOR3 cameraAxisX,D3DXVECTOR3 cameraAxisZ)
{

	if (operationDirection.x == 0 && operationDirection.y == 0)return;//入力値が0以下ならば移動しない
	isExecutingMoveOperation = true;

//Y軸方向への成分を削除する
D3DXVECTOR3 front = slip(cameraAxisZ, axisY.direction);
D3DXVECTOR3 right = slip(cameraAxisX, axisY.direction);
D3DXVec3Normalize(&front, &front);//正規化
D3DXVec3Normalize(&right, &right);//正規化

//操作方向をカメラのXZ方向に準拠した移動ベクトルへ変換する
D3DXVECTOR3 moveDirection = operationDirection.x*right + -operationDirection.y*front;

if (onGround)
{
	acceleration = moveDirection * MOVE_ACC * dash();
}
else
{
	acceleration.x += moveDirection.x * MOVE_ACC_WHEN_NOT_GROUND;
	acceleration.z += moveDirection.z * MOVE_ACC_WHEN_NOT_GROUND;
}
}

//===================================================================================================================================
//【ダッシュ】
// [処理内容]入力に応じてダッシュ倍率を返す
//===================================================================================================================================
float Player::dash()
{
	if (input->isKeyDown(keyTable.dash))
		return playerNS::DASH_MAGNIFICATION;
	return 1.0f;
}

//===================================================================================================================================
//【ジャンプ】
//===================================================================================================================================
void Player::jump()
{
	if (onGround && !onJump)
	{
		onJump = true;	// ジャンプ踏切フラグをオンにする
		speed += axisY.direction * JUMP_SPEED;	// 初速を加える
	}
}

//===================================================================================================================================
//【射撃】
//===================================================================================================================================
bool Player::shot()
{
	if (!input->getMouseLButton() &&
		!input->getController()[infomation.playerType]->isButton(BUTTON_BULLET))return false;
	bulletManager->launch(shootingRay);
	return true;
}

//===================================================================================================================================
//【デジタルシフト】
//===================================================================================================================================
bool Player::digitalShift()
{
	if (!input->getMouseLButton() &&
		!input->getController()[infomation.playerType]->isButton(BUTTON_DIGITA_SHIFT))return false;

	//シフトレイが衝突していた場合
	if (enableShift)
	{
		transState(DIGITAL_SHIFT);
	}

	return true;

}

//===================================================================================================================================
//【シフトレイとの衝突処理】
//===================================================================================================================================
void Player::collideShiftRay(LPD3DXMESH mesh, D3DXMATRIX matrix)
{

	//シフトレイをカメラからのレイとして更新
	shiftRay.update(camera->position, camera->getDirectionZ());

	//シフトレイと衝突&&最短距離更新
	if (shiftRay.rayIntersect(mesh, matrix))
	{
		if (enableShift == false)
		{
			//シフトを有効にする
			enableShift = true;
			//デジタルシフトを行うラインを設定
			shiftLine.start = position;
			shiftLine.end = shiftLine.start + shiftRay.direction*shiftRay.distance;
		}
		else if (shiftRay.distance < Base::between2VectorLength(shiftLine.start, shiftLine.end))
		{
			//デジタルシフトを行うラインを設定
			shiftLine.start = position;
			shiftLine.end = shiftLine.start + shiftRay.direction*shiftRay.distance;
		}
	}
	else {
		//シフトを無効にする
		enableShift = false;
	}

}


//===================================================================================================================================
//【デジタルシフト実行】
//===================================================================================================================================
bool Player::executionDigitalShift()
{
	if (!enableShift)return false;
	//デジタルシフト開始
	position = shiftLine.end;
	enableShift = false;
	return true;
}

//===================================================================================================================================
//【ビジョン】
//===================================================================================================================================
bool Player::vision()
{
	if (!input->wasKeyPressed(keyTable.vision) &&
		!input->getController()[infomation.playerType]->isButton(BUTTON_VISION))return false;
	transState(VISION);
	return true;
}

//===================================================================================================================================
//【ビジョンのキャンセル】
//===================================================================================================================================
bool Player::cancelVision()
{
	if (!input->wasKeyPressed(keyTable.vision) &&
		!input->getController()[infomation.playerType]->isButton(BUTTON_VISION))return false;
	transState(NORMAL);
	return true;
}

//===================================================================================================================================
//【スカイビジョン】
//===================================================================================================================================
bool Player::skyVision()
{
	if (!input->wasKeyPressed(keyTable.skyVision) &&
		!input->getController()[infomation.playerType]->isButton(BUTTON_SKY_VISION))return false;
	transState(SKY_VISION);
	return true;
}

//===================================================================================================================================
//【スカイビジョンのキャンセル】
//===================================================================================================================================
bool Player::cancelSkyVision()
{
	if (!input->wasKeyPressed(keyTable.skyVision) &&
		!input->getController()[infomation.playerType]->isButton(BUTTON_SKY_VISION))return false;
	transState(NORMAL);
	return true;
}

//===================================================================================================================================
//【射撃方向を更新する】
//===================================================================================================================================
void Player::updateAiming(LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	aimingRay.update(camera->position + camera->getDirectionZ(), camera->getDirectionZ());
	//照射位置の算出
	collideDistance = MAX_DISTANCE;
	aimingPosition = aimingRay.start + (aimingRay.direction * collideDistance);
	if (aimingRay.rayIntersect(mesh, matrix) && aimingRay.distance <= MAX_DISTANCE)
	{
		if (collideDistance > aimingRay.distance)
		{
			collideDistance = aimingRay.distance;
			aimingPosition = aimingRay.start + (aimingRay.direction* aimingRay.distance);//衝突位置
		}
	}

}

//===================================================================================================================================
//【射撃方向へ姿勢を向ける】
//===================================================================================================================================
void Player::updatePostureByAiming()
{
	D3DXVECTOR3 front;
	Base::between2VectorDirection(&front, position, aimingPosition);	//射撃方向ベクトルの算出
	front = slip(front, axisY.direction);								//Y軸方向への成分を削除する
	D3DXVec3Normalize(&front, &front);									//正規化
	postureControl(getAxisZ()->direction, front, 0.3f);
}

//===================================================================================================================================
//【発射位置の更新】
//===================================================================================================================================
void Player::updateShooting(LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	//発射位置の更新
	launchPosition = center + axisZ.direction*radius;
	//狙撃レイの更新
	Base::between2VectorDirection(&shootingRay.direction, launchPosition, aimingPosition);
	shootingRay.update(launchPosition, shootingRay.direction);
	if (shootingRay.rayIntersect(mesh, matrix) &&
		shootingRay.distance < MAX_DISTANCE)
	{
		//衝突時、狙撃レイの衝突位置にレティクル位置を置く
		aimingPosition = shootingRay.start + (shootingRay.direction * shootingRay.distance);
	}
	else {
		//非衝突の場合、最大距離を入れる。
		shootingRay.distance = MAX_DISTANCE;
	}
}

#pragma endregion

//[デバッグ]
#pragma region Debug
//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void Player::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("PlayerInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;
		ImGui::Text("state[%s]", state->stateName.c_str());
		ImGui::Text("shootingRay.distance %.02f", shootingRay.distance);
		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));
		ImGui::Text("power %d", power);													//電力
		ImGui::Text("BulletRmaining [%d/%d]",
			bulletManager->getRemaining(),bulletNS::MAGAZINE_NUM);						//残弾数
		ImGui::Text("ReloadTime:%.02f", bulletManager->getReloadTime());				//リロード時間
		ImGui::Text("dot:%.02f", dot);

		ImGui::Text("[shiftRay] start(%.02f,%.02f,%.02f):distance(%.02f)",
			shiftRay.start.x, shiftRay.start.y, shiftRay.start.z,
			shiftRay.distance);

		ImGui::Text("[shiftLine] start(%.02f,%.02f,%.02f):end(%.02f,%.02f,%.02f)",
			shiftLine.start.x, shiftLine.start.y, shiftLine.start.z,
			shiftLine.end.x, shiftLine.end.y, shiftLine.end.z);

		ImGui::Text(enableShift ? "enableShift:On":"enableShift:Off");
		
		ImGui::Text(onGround ? "onGround:On":"onGround:Off");

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//位置
		ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//回転
		ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);						//スケール
		ImGui::SliderFloat("radius", &radius, 0, limitTop);								//半径
		ImGui::SliderFloat("alpha", &alpha, 0, 255);									//透過値
		ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);						//速度
		ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//加速度
		ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);					//重力

		ImGui::Checkbox("onGravity", &onGravity);										//重力有効化フラグ
		ImGui::Checkbox("onActive", &onActive);											//アクティブ化フラグ
	}
#endif // _DEBUG
}

//===================================================================================================================================
//【リセット】
//===================================================================================================================================
void Player::reset()
{
	position	= START_POSITION[infomation.playerType];
	speed		= acceleration = D3DXVECTOR3(0, 0, 0);
	quaternion	= D3DXQUATERNION(0, 0, 0, 1);
	axisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 0, 0));
	axisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));
	axisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1));
	reverseAxisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-1, 0, 0));
	reverseAxisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -1, 0));
	reverseAxisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, -1));
	Object::update();
}
#pragma endregion

//[Setter]
#pragma region Setter
//===================================================================================================================================
//【setter】
//===================================================================================================================================
void Player::setCamera(Camera* _camera) { 
	camera = _camera; 
	aimingRay.initialize(camera->position, camera->getDirectionZ());
	aimingRay.color = D3DXCOLOR(255,123,193,255);
	shootingRay.color = D3DXCOLOR(120, 10, 20, 255);
}
void Player::setInfomation(PlayerTable info) { infomation = info; };
void Player::addpower(int add)
{
	power = UtilityFunction::clamp( power + add, MIN_POWER, MAX_POWER);		//電力回復
}					
void Player::pullpower(int pull)
{
	power = UtilityFunction::clamp( power - pull, MIN_POWER, MAX_POWER);		//電力消費
}
void Player::damage(int _damage)
{
	hp -= _damage;
	if (hp < 0) hp = 0;
}
void Player::setValidOperation(int value) 
{
	validOperation = value;
}
void Player::enableOperation(int value) 
{
	validOperation |= value;
}
void Player::disableOperation(int value) 
{
	validOperation &= ~value;
}
#pragma endregion

//[Getter]
#pragma region Getter
//===================================================================================================================================
//【getter】
//===================================================================================================================================
int Player::getHp() { return hp; }
int Player::getPower() { return power; }
int Player::getState() { return state->type; }
bool Player::whetherValidOperation(int operation) {
	if (validOperation & operation)	return true;
	return false;
}
bool Player::canShot() { return isShotAble; }
bool Player::canJump() { return isJumpAble; }
bool Player::canDoVision(){ return isVisionAble; }
bool Player::canDoSkyVision() { return isSkyVisionAble; }
bool Player::canShift() { return isShiftAble; }
BoundingSphere* Player::getBodyCollide() { return &bodyCollide; }
PlayerTable* Player::getInfomation() { return &infomation; }
D3DXVECTOR3* Player::getCameraGaze() { return &cameraGaze; }
D3DXVECTOR3* Player::getAiming() { return &aimingPosition; }
Bullet* Player::getBullet(int i) { return bulletManager->getBullet(i); }
int Player::getShootingNum() { return bulletManager->getNum(); }
bool Player::getWhetherExecutingMoveOpe() { return isExecutingMoveOperation; }
bool Player::getOnGround() { return onGround; }
D3DXVECTOR3* Player::getGroundNormal() { return &groundNormal; }
LPD3DXMESH Player::getMesh() { return box->mesh; };
BulletManager*  Player::getBulletManager() { return bulletManager; }
#pragma endregion

