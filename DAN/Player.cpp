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
#include "MoveP1.h"
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
	onGravity = true;
	//タイマー
	invincibleTimer = 0.0f;					//無敵時間
	recoveryPowerTimer = 0.0f;					//自動電力回復時間

	//汎用
	reverseValueXAxis = CAMERA_SPEED;			//操作Ｘ軸
	reverseValueYAxis = -CAMERA_SPEED;		//操作Ｙ軸

	//物理
	onGround = false;				//接地判定
	difference = DIFFERENCE_FIELD;		//フィールド補正差分

	//カメラ遷移
	cameraTransitionTimer = 0.0f;
	cameraTransitionTime = 0.0f;
	nowCameraTransing = false;

	//基本アクション
	onJump = false;				//ジャンプフラグ

	groundDistance = 0.0f;

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Player::~Player()
{
	SAFE_DELETE(bulletManager);
	SAFE_DELETE(digitalShiftEffect);
	SAFE_DELETE(state);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
//プレイヤータイプごとに初期化内容を変更
void Player::initialize(PlayerTable info)
{
	device = getDevice();
	input = getInput();
	infomation = info;
	keyTable = KEY_TABLE[infomation.playerType];
	Object::initialize(&(D3DXVECTOR3)START_POSITION[infomation.playerType]);

	// コライダの初期化
	bodyCollide.initialize(&position, staticMeshNS::reference(infomation.modelType)->mesh);
	setRadius(bodyCollide.getRadius());	// メッシュ半径を取得

	//サイズの設定
	setSize(D3DXVECTOR3(0.5f, 1.7f, 0.5f));

	// HPの設定
	hp = MAX_HP;

	//電力の設定
	power = MAX_POWER;							//キャラクター電力確認用

	//通常状態
	state = new normalNS::NormalState(this);
	enableOperation(ENABLE_CAMERA);

	//シューティングアクション
	bulletManager = new BulletManager;
	collideAxisX = false;

	//デジタルアクション
	//デジタルシフト
	shiftLine.start		= position;
	shiftLine.end		= position+axisZ.direction;
	//選択ライトを再生状態にしておく。
	digitalShiftEffect	= new DigitalShiftEffect;
	playSelectLight();
	

	//再生パラメータの作成
	//memset(playParameters, 0, sizeof(playParameters));
	//FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	shiftStartSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_ShiftStart, false ,NULL,false,NULL };
	shiftFinishSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_ShiftFinish, false ,NULL,false,NULL };
	visionSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Vision, false ,NULL,false,NULL };
	visionStartSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_VisionStart, false ,NULL,false,NULL };
	visionFinishSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_VisionFinish, false ,NULL,false,NULL };
	skyVisionStartSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_SkyVisionStart, false ,NULL,false,NULL };
	skyVisionFinishSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_SkyVisionStart, false ,NULL,false,NULL };

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Player::update(float frameTime)
{
	// 事前処理
	this->frameTime = frameTime;
	friction = 1.0f;
	isExecutingMoveOperation = false;
	onJump = false;
	acceleration *= 0.0f;
	onGroundBefore = onGround;


	//自動電力回復
	recoveryPower();

	//状態別更新
	state->update(frameTime);

	//バレットマネージャーの更新
	bulletManager->update(frameTime);

	//デジタルシフトエフェクトの更新
	digitalShiftEffect->update(frameTime);

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
	if (input->isKeyDown('J'))
	{// 高速移動
		position += speed;
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

	// オブジェクトの更新
	Object::update();

}

//===================================================================================================================================
//【本体以外の他のオブジェクト描画】
//===================================================================================================================================
void Player::otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//バレットの描画
	bulletManager->render(view, projection, cameraPosition);
	//デジタルシフトエフェクトの描画
	digitalShiftEffect->render(view, projection, cameraPosition);

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
	if (onGround)
	{
		if (onJump)
		{

		}
		else {
			dot = D3DXVec3Dot(&gravityRay.normal, &D3DXVECTOR3(0, 1, 0));
			if (dot > MAX_SLOPE) {
				speed = slip(speed, gravityRay.normal);
			}
			else {
				speed = slip(speed, gravityRay.normal);
			}
		}
	}
}

//===================================================================================================================================
// 対オブジェクト接地処理
//===================================================================================================================================
bool Player::grounding(LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	//ジャンプ時、接地処理しない
	if (onJump)
	{
		onGround = false;
		return false;
	}

	//重力方向、レイを設定
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(center, gravityDirection);

	//レイによる衝突検知
	bool hit = gravityRay.rayIntersect(mesh, matrix);
	MOVEP *MoveP = GetMovePAdr();
	if (hit && infomation.playerType == gameMasterNS::PLAYER_1P) {
		MoveP->GroundDistance = gravityRay.distance + size.y / 2;
	}
	else if (infomation.playerType == gameMasterNS::PLAYER_1P)
	{
		MoveP->GroundDistance = 10.0f;
	}

	MOVEP1 *MoveP1 = GetMoveP1Adr();
	if (hit && infomation.playerType == gameMasterNS::PLAYER_2P) {
		MoveP1->GroundDistance = gravityRay.distance + size.y / 2;
	}
	else if (infomation.playerType == gameMasterNS::PLAYER_2P)
	{
		MoveP1->GroundDistance = 10.0f;
	}

	//レイ衝突時、レイ距離が、プレイヤー高さより小さい場合食い込み状態となっているので、
	if (hit && size.y / 2 + difference >= gravityRay.distance)
	{
		//接地状態とみなし、
		onGround = true;
		//食い込んでいる距離分、位置を補正する
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
bool Player::insetCorrection(int axisID, float distance, LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	using namespace objectNS;
	switch (axisID)
	{
	case AXIS_X:	
		collideAxisX = insetCorrection(&axisX, distance, mesh, matrix);
		return collideAxisX; break;
	case AXIS_RX:	return insetCorrection(&reverseAxisX, distance, mesh, matrix); break;
	case AXIS_Y:	return insetCorrection(&axisY, distance, mesh, matrix); break;
	case AXIS_RY:	return insetCorrection(&reverseAxisY, distance, mesh, matrix); break;
	case AXIS_Z:	return insetCorrection(&axisZ, distance, mesh, matrix); break;
	case AXIS_RZ:	return insetCorrection(&reverseAxisZ, distance, mesh, matrix); break;
	}
	return false;
}
bool Player::insetCorrection(Ray* ray, float distance, LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	Ray tmp = *ray;

	if (!tmp.rayIntersect(mesh, matrix))
	{
		ray->distance = 100000.0f;
		return false; 
	}

	ray->distance = tmp.distance;

	if (distance >= tmp.distance)
	{
		position += tmp.normal*(distance - tmp.distance);
	}
	return true;
}
#pragma endregion

//[物理]
#pragma region PhysicsMovement

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

	MOVEP *MoveP = GetMovePAdr();
	MoveP->MoveY = speed.y;

	if (onGround) {
		//摩擦方向を算出
		D3DXVECTOR3 frictionDirection;
		D3DXVec3Normalize(&frictionDirection,&speed);
		dot = D3DXVec3Dot(&gravityRay.normal, &D3DXVECTOR3(0, 1, 0));
		if (dot > MAX_SLOPE) {
			// 速度に摩擦の影響を与える
			speed -= frictionDirection * MOVE_FRICTION * GRAVITY_FORCE * frameTime;
		}
		else {
			// 速度に摩擦の影響を与える
			speed -= frictionDirection * MOVE_FRICTION * GRAVITY_FORCE  * frameTime;

			//斜面滑りベクトル
			D3DXVECTOR3 slipVector;
			D3DXVec3Normalize(&slipVector, &slip(D3DXVECTOR3(0, -1, 0), gravityRay.normal));
			if (D3DXVec3Length(&speed) < MAX_SLIP)
			{
				speed += slipVector * MOVE_ACC * 2 * frameTime;
			}
		}
	}

	D3DXVECTOR3 ySpeed = D3DXVECTOR3(0.0f, speed.y, 0.0f);
	D3DXVECTOR3 yAcceleration = D3DXVECTOR3(0.0f, acceleration.y, 0.0f);

	D3DXVECTOR3 xzSpeed = D3DXVECTOR3(speed.x, 0.0f, speed.z);
	D3DXVECTOR3 xzAcceleration = D3DXVECTOR3(acceleration.x, 0.0f, acceleration.z);

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
	MOVEP1 *MoveP1 = GetMoveP1Adr();

	if (!MoveP->IsDie || !MoveP1->IsDie)
	{

		//移動操作が無効
		if (!whetherValidOperation(ENABLE_MOVE))return;

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

}

//===================================================================================================================================
//【ジャンプ操作】
// [処理内容]（1）初入力でジャンプ踏切フラグをオン　(2)ジャンプを更新
//===================================================================================================================================
void Player::jumpOperation()
{
	MOVEP *MoveP = GetMovePAdr();
	MOVEP1 *MoveP1 = GetMoveP1Adr();

	//ジャンプ操作が無効
	if (!whetherValidOperation(ENABLE_JUMP))return;

	//死亡時操作を受け付けない
	switch (infomation.playerType)
	{
	case gameMasterNS::PLAYER_1P:		if (MoveP->IsDie)return;		break;
	case gameMasterNS::PLAYER_2P:		if (MoveP1->IsDie)return;		break;
	}

	//入力確認
	if ((input->getMouseRButtonTrigger() || input->getController()[infomation.playerType]->wasButton(BUTTON_JUMP)))
	{
		jump();
		PLAY_PARAMETERS jumpVoice = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::Voice_Man_Jump, false, NULL, false, NULL };
		SoundInterface::SE->playSound(&jumpVoice);	//SE再生
	}

	//接地フラグ切替
	switch (infomation.playerType)
	{
	case gameMasterNS::PLAYER_1P:	MoveP->IsGround		= onGround;	break;
	case gameMasterNS::PLAYER_2P:	MoveP1->IsGround	= onGround;	break;
	}

}

#pragma endregion

//[カメラ]
#pragma region Camera
//===================================================================================================================================
//【カメラの操作/更新】
//===================================================================================================================================
void Player::controlCamera(float frameTime)
{
	//操作可能かチェック
	if (!(validOperation & ENABLE_CAMERA))return;

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
	D3DXVec3Normalize(&cameraGazeRelative, &(D3DXVECTOR3)cameraGazeRelativeQ);
	//カメラ注視位置の更新
	cameraGaze = position
		+ axisY.direction*CAMERA_GAZE.y;
	if (collideAxisX && CAMERA_GAZE.x > axisX.distance)
	{
		cameraGaze += cameraGazeRelative * axisX.distance;
	}
	else {
		cameraGaze += cameraGazeRelative * CAMERA_GAZE.x;
	}


	//カメラの更新
	camera->update();
	aimingRay.update(camera->position + camera->getDirectionZ(), camera->getDirectionZ());
}

//===================================================================================================================================
//【スカイカメラの操作/更新】
//===================================================================================================================================
void Player::controlSkyCamera()
{
	//操作可能かチェック
	if (!(validOperation & ENABLE_CAMERA))return;

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
	D3DXVec3Normalize(&cameraGazeRelative, &(D3DXVECTOR3)cameraGazeRelativeQ);

	//カメラ注視位置の更新
	cameraGaze = position
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction*SKY_HEIGH;

	//カメラの更新
	camera->update();
	aimingRay.update(camera->position + camera->getDirectionZ(), camera->getDirectionZ());
}

//===================================================================================================================================
//【カメラを通常のプレイヤー視点へ戻す】
//===================================================================================================================================
void Player::returnTransitionCamera(float time)
{
	nextGaze = position
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction*CAMERA_GAZE.y;
	startTransitionCamera(time, cameraGaze, nextGaze);
}

//===================================================================================================================================
//【カメラ遷移開始】
//===================================================================================================================================
void Player::startTransitionCamera(float time, D3DXVECTOR3 before, D3DXVECTOR3 next)
{
	//遷移中にする
	nowCameraTransing = true;

	//直前のカメラ位置を設定する
	beforeGaze = before;

	//次のカメラ位置を設定する
	nextGaze = next;

	//遷移時間を設定
	cameraTransitionTime = time;

	//カメラの操作許可を切る
	disableOperation(ENABLE_CAMERA);

	//遷移タイマーを0で初期化
	cameraTransitionTimer = 0.0f;
}

//===================================================================================================================================
//【カメラ遷移】
//===================================================================================================================================
void Player::transitionCamera()
{
	//遷移中でなければ実行しない
	if (!nowCameraTransing)return;

	//カメラ遷移時間更新
	if (cameraTransitionTime > cameraTransitionTimer)
	{
		cameraTransitionTimer += frameTime;
	}

	//設定時間 > 遷移タイマー
	if (cameraTransitionTime > cameraTransitionTimer)
	{
		float rate = cameraTransitionTimer / cameraTransitionTime;
		D3DXVec3Lerp(&cameraGaze, &beforeGaze, &nextGaze, rate);
	}
	else
	{
		//カメラ遷移中フラグを終了
		nowCameraTransing = false;
		//カメラ位置を遷移先へ設定
		cameraGaze = nextGaze;
		//遷移が完了しているので、カメラの操作を許可
		enableOperation(ENABLE_CAMERA);
	}

}

#pragma endregion

//[自動アクション]
#pragma region AutoAction
//===================================================================================================================================
//【電力の自動回復】
//===================================================================================================================================
void Player::recoveryPower()
{
	if (power >= FULL_POWER)return;
	//1秒に1度回復
	if (recoveryPowerTimer < 1.0f)
	{
		recoveryPowerTimer += frameTime;
		return;
	}

	recoveryPowerTimer = 0.0f;
	power = UtilityFunction::clamp(power + AUTO_RECOVERY_POWER, 0, FULL_POWER);
}
//===================================================================================================================================
//【リスポーン】
//===================================================================================================================================
void Player::respawn()
{
	//position = START_POSITION[infomation.playerType];
	//speed = acceleration = D3DXVECTOR3(0, 0, 0);
	//quaternion = D3DXQUATERNION(0, 0, 0, 1);
	//axisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 0, 0));
	//axisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));
	//axisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1));
	//reverseAxisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-1, 0, 0));
	//reverseAxisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -1, 0));
	//reverseAxisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, -1));
	//Object::update();
}

#pragma endregion

//[アクション]
#pragma region Action
//===================================================================================================================================
//【移動】
// [処理内容1]入力された２次元ベクトルに応じてカメラ情報に基づき、速度へ加算処理を行う。
// [処理内容2]移動後の姿勢制御を行う。
//===================================================================================================================================
void Player::move(D3DXVECTOR2 operationDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ)
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
	MOVEP *MoveP = GetMovePAdr();
	MOVEP1 *MoveP1 = GetMoveP1Adr();

	//入力確認
	if (!input->getMouseLButton() && !input->getController()[infomation.playerType]->isButton(BUTTON_BULLET))
	{
		return false;
	}

	//死亡時操作を受け付けない
	switch (infomation.playerType)
	{
	case gameMasterNS::PLAYER_1P:		if (MoveP->IsDie)	return false;	break;
	case gameMasterNS::PLAYER_2P:		if (MoveP1->IsDie)	return false;	break;
	}

	//弾の発射
	if (bulletManager->launch(shootingRay))
	{
		//エフェクトの再生
		bulletNS::Muzzle* muzzle = new bulletNS::Muzzle(&launchPosition, &matrixRotation);
		effekseerNS::play(0, muzzle);
		return true;
	}

	return false;
}

//===================================================================================================================================
//【デジタルシフト】
//===================================================================================================================================
bool Player::digitalShift()
{
	//シフト操作不能
	if (!whetherValidOperation(ENABLE_SHIFT))	return false;

	if (!input->getMouseLButton() &&
		!input->getController()[infomation.playerType]->wasButton(BUTTON_DIGITA_SHIFT))return false;
	if (power < COST_SHIFT)return false;


	//電力の使用
	power -= COST_SHIFT;

	//次の遷移先を設定
	nextGaze = shiftLine.end
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction*CAMERA_GAZE.y;

	//カメラ遷移を開始
	startTransitionCamera(SKY_TRANSITION_TIME, cameraGaze, nextGaze);

	//デジタルシフトの開始エフェクトを再生
	digitalShiftEffect->play(DigitalShiftEffectNS::START_SHIFT, center);

	transState(DIGITAL_SHIFT);
	return true;

}

//===================================================================================================================================
//【シフトレイ（ボリュームレイ）との衝突処理】
//===================================================================================================================================
bool Player::collideShiftRay(Cylinder target)
{
	//シフトレイをカメラからのレイとして更新
	shiftRay.update(camera->position, camera->getDirectionZ());

	//カメラからの半径付きレイを設定
	Cylinder volumeRayFromCamera;
	volumeRayFromCamera.centerLine.start = camera->position;
	volumeRayFromCamera.centerLine.end = camera->position + camera->getDirectionZ()*10000.0f;
	volumeRayFromCamera.radius = 3.0f;

	//
	D3DXVECTOR3 nearest =
		Base::nearestPointOnLine(
			shiftRay.start,
			shiftRay.start + shiftRay.direction*10000.0f,
			target.centerLine.start);

	//カメラからのレイ上での距離
	float distanceOnRay = Base::between2VectorLength(nearest, camera->position);

	//円柱間の距離
	float distance = Base::between2LineDistance(target.centerLine, volumeRayFromCamera.centerLine);

	//円柱同士の衝突時距離
	float radiusDistance = target.radius + volumeRayFromCamera.radius;

	if (distance <= radiusDistance)
	{
		D3DXVECTOR3 shiftDirection;
		//デジタルシフト有効でない場合：初期シフト位置の設定
		if (!whetherValidOperation(ENABLE_SHIFT))
		{
			//シフトを有効にする
			enableOperation(ENABLE_SHIFT);
			//デジタルシフトを行うラインを設定
			shiftLine.start = this->position;
			shiftLine.end = target.centerLine.start;
			//デジタルシフトの方向を算出
			Base::between2VectorDirection(&shiftDirection, shiftLine.start, shiftLine.end);//方向算出
			shiftDirection = slip(shiftDirection, D3DXVECTOR3(0, 1, 0));//Y方向削除
			D3DXVec3Normalize(&shiftDirection, &shiftDirection);//正規化
			//シフト位置の決定
			shiftPosition = shiftLine.end;
			shiftPosition += target.height*D3DXVECTOR3(0, 1, 0);
			shiftPosition -= shiftDirection*target.radius;
			//最短シフト距離更新
			shiftDistance = distance + distanceOnRay;//レイ間距離+レイ上距離
			return true;
		}
		//最短シフト距離よりもより短い場合：シフト位置の再設定
		else if (shiftDistance > distance + distanceOnRay)//レイ間距離+レイ上距離
		{
			//デジタルシフトを行うラインを設定
			shiftLine.start = this->position;
			shiftLine.end = target.centerLine.start;
			//デジタルシフトの方向を算出
			Base::between2VectorDirection(&shiftDirection, shiftLine.start, shiftLine.end);
			shiftDirection = slip(shiftDirection, D3DXVECTOR3(0, 1, 0));//Y方向削除
			D3DXVec3Normalize(&shiftDirection, &shiftDirection);//正規化
			//シフト位置の決定
			shiftPosition = shiftLine.end;
			shiftPosition += target.height*D3DXVECTOR3(0, 1, 0);
			shiftPosition -= shiftDirection * target.radius;
			//最短シフト距離更新
			shiftDistance = distance + distanceOnRay;//レイ間距離+レイ上距離
			return true;
		}
		return false;
	}

	//条件
	//円柱衝突もしていない場合や、
	//円柱衝突していたとしても、最短シフト先でない　
	return false;

}

//===================================================================================================================================
//【シフトレイ（ボリュームレイ）との衝突処理】
//===================================================================================================================================
void Player::collideShiftRay(D3DXVECTOR3 position)
{
	//シフトレイをカメラからのレイとして更新
	shiftRay.update(camera->position, camera->getDirectionZ());

	D3DXVECTOR3 nearest =
		Base::nearestPointOnLine(
			shiftRay.start,
			shiftRay.start + shiftRay.direction*10000.0f,
			position);


	float radiusDistance = Base::between2VectorLength(nearest, position);
	float distance = Base::between2VectorLength(nearest, camera->position);

	if (radiusDistance <= 3.0f)
	{
		//デジタルシフト有効でない場合
		if (!whetherValidOperation(ENABLE_SHIFT))
		{
			//シフトを有効にする
			enableOperation(ENABLE_SHIFT);
			//デジタルシフトを行うラインを設定
			shiftLine.start = this->position;
			shiftLine.end = position - shiftRay.direction*3.0f;
			shiftDistance = distance;
		}
		else if (shiftDistance > distance)
		{
			//デジタルシフトを行うラインを設定
			shiftLine.start = this->position;
			shiftLine.end = position - shiftRay.direction*3.0f;
			shiftDistance = distance;
		}
	}

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
		//デジタルシフト有効でない場合
		if (!whetherValidOperation(ENABLE_SHIFT))
		{
			//シフトを有効にする
			enableOperation(ENABLE_SHIFT);
			//デジタルシフトを行うラインを設定
			shiftLine.start = position;
			shiftLine.end = camera->position + shiftRay.direction*shiftRay.distance;
		}
		else if (shiftRay.distance < Base::between2VectorLength(shiftLine.start, shiftLine.end))
		{
			//デジタルシフトを行うラインを設定
			shiftLine.start = position;
			shiftLine.end = camera->position + shiftRay.direction*shiftRay.distance;
		}
	}

}

//===================================================================================================================================
//【デジタルシフト先選択表示エフェクト再生】
//===================================================================================================================================
void Player::playSelectLight()
{
	//選択エフェクトライト表示
	digitalShiftEffect->playSelectLight(infomation.playerType+1,&shiftLine.end);
}

//===================================================================================================================================
//【デジタルシフト先選択表示エフェクト再生】
//===================================================================================================================================
void Player::shownSelectLight(bool shown)
{
	//選択エフェクトライト表示
	digitalShiftEffect->shownSelectLigth(shown);
}

//===================================================================================================================================
//【デジタルシフト実行】
//===================================================================================================================================
bool Player::executionDigitalShift()
{
	//デジタルシフト開始
	position = shiftPosition;

	//センターも同時に更新
	center = position + D3DXVECTOR3(0.0f, size.y / 2, 0.0f);

	//デジタルシフトの終了エフェクトを再生
	digitalShiftEffect->play(DigitalShiftEffectNS::END_SHIFT, center);

	return true;
}

//===================================================================================================================================
//【スカイビジョン実行】
//===================================================================================================================================
bool Player::executionSkyVision()
{


	return true;
}

//===================================================================================================================================
//【ビジョン実行】
//===================================================================================================================================
bool Player::executionVision()
{


	return true;
}

//===================================================================================================================================
//【ビジョン】
//===================================================================================================================================
bool Player::vision()
{
	if (!input->wasKeyPressed(keyTable.vision) &&
		!input->getController()[infomation.playerType]->wasButton(BUTTON_VISION))return false;

	returnTransitionCamera(0.0f);

	transState(VISION);
	return true;
}

//===================================================================================================================================
//【ビジョンのキャンセル】
//===================================================================================================================================
bool Player::cancelVision()
{
	if (!input->wasKeyPressed(keyTable.vision) &&
		!input->getController()[infomation.playerType]->wasButton(BUTTON_VISION))return false;

	returnTransitionCamera(0.0f);

	transState(NORMAL);
	return true;
}

//===================================================================================================================================
//【スカイビジョン】
//===================================================================================================================================
bool Player::skyVision()
{
	//入力検知
	if (!input->wasKeyPressed(keyTable.skyVision) &&
		!input->getController()[infomation.playerType]->wasButton(BUTTON_SKY_VISION))return false;

	//次のカメラ遷移先としてスカイカメラ位置を設定する
	nextGaze = position
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction*SKY_HEIGH;

	//カメラ遷移を開始
	startTransitionCamera(SKY_TRANSITION_TIME, cameraGaze, nextGaze);

	//スカイビジョンへ遷移
	transState(SKY_VISION);

	return true;
}

//===================================================================================================================================
//【スカイビジョンのキャンセル】
//===================================================================================================================================
bool Player::cancelSkyVision()
{
	//入力検知
	if (!input->wasKeyPressed(keyTable.skyVision) &&
		!input->getController()[infomation.playerType]->wasButton(BUTTON_SKY_VISION))return false;

	//次のカメラ位置を地上プレイヤーカメラ位置に設定する
	nextGaze = position
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction;

	//カメラ遷移を開始
	startTransitionCamera(SKY_RETURN_TIME, cameraGaze, nextGaze);

	//通常状態へ遷移
	transState(NORMAL);

	return true;
}

//===================================================================================================================================
//【射撃方向を更新する】
//===================================================================================================================================
void Player::updateAiming(LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	aimingRay.update(*camera->target + camera->getDirectionZ(), camera->getDirectionZ());
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
	MOVEP *MoveP = GetMovePAdr();

	//発射位置の更新
	launchPosition = center;// +axisZ.direction*radius;
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


		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));

		//電力関係
		ImGui::Text("power %d", power);													//電力
		ImGui::Text("recoveryPowerTimer:%.02f", recoveryPowerTimer);					//電力回復時間


		//シューティング関係
		ImGui::Text("bulletList->nodeNum [%d]", bulletManager->getNum());				//実際のバレットリストのノードの数
		ImGui::Text("BulletRmaining [%d/%d]",
			bulletManager->getRemaining(), bulletNS::MAGAZINE_NUM);						//残弾数
		ImGui::Text("ReloadTime:%.02f", bulletManager->getReloadTime());				//リロード時間
		ImGui::Text("shootingRay.distance %.02f", shootingRay.distance);

		//カメラ遷移
		ImGui::Text(nowCameraTransing ? "nowCameraTransing:ON" : "nowCameraTransing:OFF");
		ImGui::Text("CameraTransTime : %.02f / %.02f",
			cameraTransitionTimer, cameraTransitionTime);
		//デジタルシフトレイ
		ImGui::Text("[shiftRay] start(%.02f,%.02f,%.02f):distance(%.02f)",
			shiftRay.start.x, shiftRay.start.y, shiftRay.start.z,
			shiftRay.distance);
		ImGui::Text("[shiftLine] start(%.02f,%.02f,%.02f):end(%.02f,%.02f,%.02f)",
			shiftLine.start.x, shiftLine.start.y, shiftLine.start.z,
			shiftLine.end.x, shiftLine.end.y, shiftLine.end.z);
		ImGui::Text("[shiftDistance] %.02f", shiftDistance);

		//操作有効フラグ
		//シフト
		ImGui::Text(whetherValidOperation(ENABLE_SHIFT) ? "ENABLE_SHIFT:ON":"ENABLE_SHIFT:OFF");
		ImGui::Text(whetherValidOperation(ENABLE_JUMP) ? "ENABLE_JUMP:ON" : "ENABLE_JUMPT:OFF");
		ImGui::Text(whetherValidOperation(ENABLE_VISION) ? "ENABLE_VISION:ON" : "ENABLE_VISION:OFF");
		ImGui::Text(whetherValidOperation(ENABLE_SKY_VISION) ? "ENABLE_SKY_VISION:ON" : "ENABLE_SKY_VISION:OFF");

		//接地関係
		ImGui::Text(onGround ? "onGround:ON" : "onGround:OFF");							//接地フラグ
		ImGui::Text("dot:%.02f", dot);

		//オブジェクトパラメータ
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
	position = START_POSITION[infomation.playerType];
	speed = acceleration = D3DXVECTOR3(0, 0, 0);
	quaternion = D3DXQUATERNION(0, 0, 0, 1);
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
	aimingRay.color = D3DXCOLOR(255, 123, 193, 255);
	shootingRay.color = D3DXCOLOR(120, 10, 20, 255);
}
void Player::setInfomation(PlayerTable info) { infomation = info; };
void Player::addpower(int add)
{
	power = UtilityFunction::clamp(power + add, MIN_POWER, MAX_POWER);		//電力回復
}
void Player::pullpower(int pull)
{
	power = UtilityFunction::clamp(power - pull, MIN_POWER, MAX_POWER);		//電力消費
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
bool Player::canShot() { return whetherValidOperation(ENABLE_SHOT); }
bool Player::canJump() { return whetherValidOperation(ENABLE_JUMP); }
bool Player::canDoVision() { return whetherValidOperation(ENABLE_VISION); }
bool Player::canDoSkyVision() { return whetherValidOperation(ENABLE_SKY_VISION); }
bool Player::canShift() { return whetherValidOperation(ENABLE_SHIFT); }
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

