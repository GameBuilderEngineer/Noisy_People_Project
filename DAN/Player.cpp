//===================================================================================================================================
//【Player.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/24
// [更新日]2019/11/05
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Player.h"
#include "ImguiManager.h"
#include "UtilityFunction.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace playerNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Player::Player() 
{
	Object::initialize(&D3DXVECTOR3(0,10.0f,0));
	ZeroMemory(&keyTable, sizeof(OperationKeyTable));
	type				= ObjectType::PLAYER; 
	onGravity			= true;
	//state				= NORMAL;
	invincibleTimer		= 0.0f;					//無敵時間
	onGround			= false;				//接地判定
	reverseValueXAxis	= CAMERA_SPEED;			//操作Ｘ軸
	reverseValueYAxis	= CAMERA_SPEED;			//操作Ｙ軸
	onJump				= false;				//ジャンプフラグ
	difference			= DIFFERENCE_FIELD;		//フィールド補正差分

	isShotAble			= true;
	isJumpAble			= true;
	isVisionAble		= true;
	isSkyVisionAble		= true;
	isShiftAble			= true;

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
	radius				= bodyCollide.getRadius();				// メッシュ半径を取得
	centralPosition		= position + bodyCollide.getCenter();	// 中心座標を設定
	D3DXMatrixIdentity(&centralMatrixWorld);					// 中心座標ワールドマトリクスを初期化
	power				= MAX_POWER;							//キャラクター電力確認用

	//通常状態
	state = new NormalState(this);

	//シューティングアクション
	bulletManager		= new BulletManager;

	//デジタルアクション
	//デジタルシフト
	shiftLine.start = position;
	shiftLine.end	= position+axisZ.direction;
	shiftTimer		= 0.0f;
	isShifting		= false;


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
	state->update(frameTime);

	//デジタルシフト更新
	if (shiftTimer > 0) {
		shiftTimer -= frameTime;
		float rate = 1.0f -(shiftTimer/SHIFT_TIME);
		D3DXVec3Lerp(&position, &shiftLine.start, &shiftLine.end,rate);
	}
	else if(isShifting){
		isShifting = false;
	}


	//switch (state)
	//{
	//case NORMAL:
	//	break;
	//
	//case VISION:
	//	break;
	//
	//case SKY_VISION:
	//	break;
	//
	//case SHIFT:
	//	break;
	//}

	//バレットマネージャーの更新
	bulletManager->update(frameTime);

	// 操作(状態別)
	state->operation();

	//shot();						//弾の発射
	//digitalShift();				//デジタルシフト
	//moveOperation();			// 移動操作
	//jumpOperation();			// ジャンプ操作

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

	state->physics();

	// 以下の順番入れ替え禁止（衝突より後に物理がくる）
	//grounding();				// 接地処理
	//physicalBehavior();			// 物理挙動
	//updatePhysics(frameTime);	// 物理の更新

	//controlCamera(frameTime);	// カメラ操作


	//照準レイの更新
	aimingRay.update(camera->position + camera->getDirectionZ(), camera->getDirectionZ());
	//照射位置の算出
	collideDistance = MAX_DISTANCE;
	aimingPosition = aimingRay.start + (aimingRay.direction * collideDistance);
	if (aimingRay.rayIntersect(attractorMesh, *attractorMatrix) && aimingRay.distance <= MAX_DISTANCE )
	{
		if (collideDistance > aimingRay.distance)
		{
			collideDistance = aimingRay.distance;
			aimingPosition = aimingRay.start + (aimingRay.direction* aimingRay.distance);//衝突位置
		}
	}

	//姿勢制御:射撃方向へ向く
	D3DXVECTOR3 front;
	Base::between2VectorDirection(&front, position, aimingPosition);	//射撃方向ベクトルの算出
	front = slip(front, axisY.direction);								//Y軸方向への成分を削除する
	D3DXVec3Normalize(&front, &front);									//正規化
	postureControl(getAxisZ()->direction, front, 0.3f);
	
	// オブジェクトの更新
	Object::update();

	//球コリジョンを中心座標へ補正
	centralPosition = position + bodyCollide.getCenter();
	D3DXMatrixTranslation(&centralMatrixWorld, centralPosition.x, centralPosition.y, centralPosition.z);
	axisX.update(centralPosition, axisX.direction);
	axisY.update(centralPosition, axisY.direction);
	axisZ.update(centralPosition, axisZ.direction);
	reverseAxisX.update(centralPosition, -axisX.direction);
	reverseAxisY.update(centralPosition, -axisY.direction);
	reverseAxisZ.update(centralPosition, -axisZ.direction);

	//発射位置の更新
	launchPosition = centralPosition + axisZ.direction*radius;
	//狙撃レイの更新
	Base::between2VectorDirection(&shootingRay.direction, launchPosition, aimingPosition);
	shootingRay.update(launchPosition, shootingRay.direction);
	shootingRay.rayIntersect(attractorMesh, *attractorMatrix);
}

//===================================================================================================================================
//【本体以外の他のオブジェクト描画】
//===================================================================================================================================
void Player::otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#ifdef _DEBUG
	aimingRay.render(collideDistance);
	shootingRay.render(MAX_DISTANCE);
	bodyCollide.render(centralMatrixWorld);
	bulletManager->render(view,projection,cameraPosition);
	Object::debugRender();
#endif // _DEBUG
}

// [衝突]
#pragma region BodyCollision
//===================================================================================================================================
// 接地処理
//===================================================================================================================================
void Player::grounding()
{

	onGroundBefore = onGround;
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(centralPosition, gravityDirection);
	
	bool hit = gravityRay.rayIntersect(attractorMesh, *attractorMatrix);
	groundNormal = gravityRay.normal;

	if (hit == false)
	{// プレイヤーは地面の無い空中にいる
		onGround = false;
		return;
	}

	if (radius + difference >= gravityRay.distance)
	{// プレイヤーは地上に接地している
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
	{// プレイヤーは地面のある空中にいる
		onGround = false;
	}
}


//===================================================================================================================================
// 壁ずり処理
//===================================================================================================================================
void Player::wallScratch()
{
	//スピード
	ray.update(centralPosition, D3DXVECTOR3(speed.x,0, speed.z));
	// 壁ずり処理
	if (ray.rayIntersect(attractorMesh, *attractorMatrix)&& radius/2 >= ray.distance)
	{
		position += ray.normal*(radius/2-ray.distance);		//めり込み補正
		speed = slip(speed, ray.normal);		// 移動ベクトルのスリップ（面方向へのベクトル成分の削除）
	}
}

//===================================================================================================================================
//【めりこみ補正】
//===================================================================================================================================
void Player::insetCorrection()
{
	//Z軸めり込み補正
	ray.update(centralPosition, axisZ.direction);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	{
		position += ray.normal*(radius / 2 - ray.distance);
	}
	//-Z軸めり込み補正
	ray.update(centralPosition, reverseAxisZ.direction);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	{
		position += ray.normal*(radius / 2 - ray.distance);
	}
	//X軸めり込み補正
	ray.update(centralPosition, axisX.direction);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	{
		position += ray.normal*(radius / 2 - ray.distance);
	}
	//-X軸めり込み補正
	ray.update(centralPosition, reverseAxisX.direction);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	{
		position += ray.normal*(radius / 2 - ray.distance);
	}
}
#pragma endregion

// [物理]
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
	//レイ判定を行うために必要な要素をセット
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	//重力線を作成
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.initialize(position, gravityDirection);		//重力レイの初期化

	//レイ判定
	if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	{//重力線上にポリゴンが衝突していた場合、ポリゴン法線を重力方向とし、姿勢を法線と一致させる。
		//postureControl(axisY.direction, gravityRay.normal, 1.0f);
		setGravity(-gravityRay.normal, GRAVITY_FORCE);
	}
	else
	{//衝突ポリゴンが存在しない場合は、重力線をそのまま重力方向とし、姿勢を重力線と一致させる。
		//postureControl(axisY.direction, gravityDirection, 1.0f);
		setGravity(gravityDirection, GRAVITY_FORCE);
	}
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
	if (onGround == false)
	{
	}
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
		// 速度に摩擦の影響を与える
		speed -= frictionDirection * MOVE_FRICTION*GRAVITY_FORCE * frameTime;
		//速度に最大速度で制限をかける
		if (D3DXVec3Length(&speed) > MAX_SPEED)
		{
			speed = MAX_SPEED * frictionDirection;
		}
		wallScratch();					//壁ずり
		position += speed* frameTime;	// 速度の影響を位置に与える
	}
	else {
		wallScratch();						//壁ずり
		position += speed * frameTime;		// 速度の影響を位置に与える
	}
	insetCorrection();//移動後のめり込み補正
}


#pragma endregion

// [操作]
#pragma region Operation
//===================================================================================================================================
//【移動操作】
// [処理内容]入力された２次元ベクトルに応じてカメラ情報に基づき、速度へ加算処理を行う。
//===================================================================================================================================
void Player::moveOperation()
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
		if (jumping == false) onJump = true;	// ジャンプ踏切フラグをオンにする
	}
	if (input->getMouseRButton() || input->getController()[infomation.playerType]->isButton(BUTTON_JUMP))
	{
		jump();
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

// [アクション]
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
	if (onGround && onJump)
	{
		speed += axisY.direction * JUMP_SPEED;	// 初速を加える
		jumping = true;
	}

	if (speed.y > 0)
	{
		acceleration.y += JUMP_CONTROL_SPEED;		// 上昇中は入力継続で飛距離が伸びる
	}
}

//===================================================================================================================================
//【射撃】
//===================================================================================================================================
void Player::shot()
{
	if (!input->getMouseLButton() && 
		!input->getController()[infomation.playerType]->isButton(BUTTON_BULLET))return;
		bulletManager->launch(shootingRay);
}

//===================================================================================================================================
//【デジタルシフト】
//===================================================================================================================================
void Player::digitalShift()
{
	if (!input->wasKeyPressed(keyTable.skyVision) &&
		!input->getController()[infomation.playerType]->isButton(BUTTON_SKY_VISION))return;
	if (isShifting)return;//シフト中：デジタルシフトしない

	//シフトレイをカメラからのレイとして更新
	shiftRay.update(camera->position, camera->getDirectionZ());

	//シフトレイが衝突していたら場合
	if (shiftRay.rayIntersect(attractorMesh, *attractorMatrix))
	{
		//デジタルシフト開始
		shiftLine.start = position;
		shiftLine.end = shiftRay.start + shiftRay.direction*shiftRay.distance;
		isShifting = true;
		shiftTimer = SHIFT_TIME;
	}

}

#pragma endregion

// [デバッグ]
#pragma region Debug
//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void Player::outputGUI()
{
#ifdef _DEBUG

	//ImGui::Text(sceneName.c_str());

	if (ImGui::CollapsingHeader("PlayerInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;
		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));
		ImGui::Text("power %d", power);													//電力
		ImGui::Text("BulletRmaining [%d/%d]",
			bulletManager->getRemaining(),bulletNS::MAGAZINE_NUM);						//残弾数
		ImGui::Text("ReloadTime:%.02f", bulletManager->getReloadTime());				//リロード時間

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

//===================================================================================================================================
//【setter】
//===================================================================================================================================
void Player::setCamera(Camera* _camera) { 
	camera = _camera; 
	aimingRay.initialize(camera->position, camera->getDirectionZ());
	aimingRay.color = D3DCOLOR_ARGB(255,253,193,0);
	shootingRay.color = D3DCOLOR_ARGB(255,190, 20, 20);
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


//===================================================================================================================================
//【getter】
//===================================================================================================================================
int Player::getHp() { return hp; }
int Player::getPower() { return power; }
int Player::getState() { return state->type; }
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
D3DXVECTOR3* Player::getCentralPosition() { return &centralPosition; }
bool Player::getWhetherExecutingMoveOpe() { return isExecutingMoveOperation; }
bool Player::getOnGround() { return onGround; }
D3DXVECTOR3* Player::getGroundNormal() { return &groundNormal; }
D3DXMATRIX* Player::getcentralMatrixWorld() { return &centralMatrixWorld; }




//(仮)通常状態

NormalState::NormalState(Player* player):AbstractState() 
{
	this->player = player;
	type = NORMAL;
}

void NormalState::start()
{

}

void NormalState::update(float frameTime)
{
	this->frameTime = frameTime;
}


void NormalState::operation()
{
	// 操作
	player->shot();						//弾の発射
	player->digitalShift();				//デジタルシフト
	player->moveOperation();			// 移動操作
	player->jumpOperation();			// ジャンプ操作
}

void NormalState::physics()
{
	player->grounding();				// 接地処理
	player->physicalBehavior();			// 物理挙動
	player->updatePhysics(frameTime);	// 物理の更新
	player->controlCamera(frameTime);
}


AbstractState* NormalState::transition()
{
	return new NormalState(player);
}

void NormalState::end()
{

}

