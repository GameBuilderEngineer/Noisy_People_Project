//===================================================================================================================================
//【TmpObject.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/24
// [更新日]2019/10/18
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "TmpObject.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace tmpObjNS;


//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
TmpObject::TmpObject()
{
	Object::initialize(&(D3DXVECTOR3)tmpObjNS::START_POSITION);
	ZeroMemory(&keyTable, sizeof(OperationKeyTable));

	onGravity = false;
	activation();
	onGround = false;						//接地判定
	reverseValueXAxis = CAMERA_SPEED;		//操作Ｘ軸
	reverseValueYAxis = CAMERA_SPEED;		//操作Ｙ軸
	onJump = false;							//ジャンプフラグ
	difference = DIFFERENCE_FIELD;			//フィールド補正差分
}


//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
TmpObject::~TmpObject()
{

}


//===================================================================================================================================
//【初期化】
//===================================================================================================================================
//仮オブジェクトタイプごとに初期化内容を変更
void TmpObject::initialize(int tmpObjectType, int modelType)
{
	device = getDevice();
	input = getInput();
	type = tmpObjectType;
	keyTable = KEY_TABLE_1P;
	Object::initialize(&(D3DXVECTOR3)START_POSITION);

	bodyCollide.initialize(&position, staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2)->mesh);	// コライダの初期化
	ItemListboxMesh = 0;									// メッシュの種類(リストボックス)
	staticMeshNo = staticMeshNS::YAMADA_ROBOT2;				// メッシュのID
	radius = bodyCollide.getRadius();						// メッシュ半径を取得
	centralPosition = position + bodyCollide.getCenter();	// 中心座標を設定
	D3DXMatrixIdentity(&centralMatrixWorld);				// 中心座標ワールドマトリクスを初期化
}


//===================================================================================================================================
//【更新】
//===================================================================================================================================
void TmpObject::update(float frameTime)
{
	// 事前処理
	friction = 1.0f;
	isExecutingMoveOperation = false;
	onJump = false;
	centralPosition = position + bodyCollide.getCenter();
	acceleration *= 0.0f;

	// 操作
	moveOperation();			// 移動操作
	jumpOperation();			// ジャンプ操作
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

	// 以下の順番入れ替え禁止（衝突より後に物理がくる）
	grounding();				// 接地処理
	wallScratch();				// 壁ずり処理
	physicalBehavior();			// 物理挙動
	updatePhysics(frameTime);	// 物理の更新
	controlCamera(frameTime);	// カメラ操作

	//// カメラは回るがキャラクターが連動してくれない
	//姿勢制御
	//if (onGround)
	//{
	//	postureControl(axisY.direction, groundNor, 3.0f * frameTime);
	//}

	Object::update();	// オブジェクトの更新
	D3DXMatrixTranslation(&centralMatrixWorld, centralPosition.x, centralPosition.y, centralPosition.z);
	axisX.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._11, centralMatrixWorld._12, centralMatrixWorld._13));
	axisY.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._21, centralMatrixWorld._22, centralMatrixWorld._23));
	axisZ.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._31, centralMatrixWorld._32, centralMatrixWorld._33));
	reverseAxisX.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._11, centralMatrixWorld._12, centralMatrixWorld._13));
	reverseAxisY.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._21, centralMatrixWorld._22, centralMatrixWorld._23));
	reverseAxisZ.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._31, centralMatrixWorld._32, centralMatrixWorld._33));
}


//===================================================================================================================================
//【描画】
//===================================================================================================================================
//void TmpObject::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
//{
//	Object::render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH),view,projection, cameraPosition);
//}


//===================================================================================================================================
//【本体以外の他のオブジェクト描画】
//===================================================================================================================================
void TmpObject::otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#ifdef _DEBUG
	//bodyCollide.render(centralMatrixWorld);
#endif // _DEBUG
}


// [衝突]
#pragma region BodyCollision
//===================================================================================================================================
// 接地処理
//===================================================================================================================================
void TmpObject::grounding()
{
	onGroundBefore = onGround;
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(centralPosition, gravityDirection);
	bool hit = gravityRay.rayIntersect(attractorMesh, *attractorMatrix);

	if (hit == false)
	{// 仮オブジェクトは地面の無い空中にいる
		onGround = false;
		return;
	}

	if (radius /*+ difference*/ >= gravityRay.distance)
	{// 仮オブジェクトは地上に接地している
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
	{// 仮オブジェクトは地面のある空中にいる
		onGround = false;
	}
}


//===================================================================================================================================
// 壁ずり処理
//===================================================================================================================================
void TmpObject::wallScratch()
{
	//// 軽量実装版（めり込み補正時に微妙にガタガタする）
	//ray.update(centralPosition, D3DXVECTOR3(speed.x, 0, speed.z));
	//if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	//{
	//	if (radius >= ray.distance)
	//	{
	//		// めり込み補正（現在位置 + 進行方向 * めり込み距離）
	//		setPosition(position + (-ray.normal)* /*ray.direction */ (ray.distance - radius));
	//		// 移動ベクトルのスリップ（面方向へのベクトル成分の削除）
	//		setSpeed(slip(speed, ray.normal));
	//		// 抵抗摩擦
	//		friction *= WALL_FRICTION;
	//	}
	//}

	bool hit = false;

	// スピードベクトルから壁の法線情報を取得する
	ray.update(centralPosition, D3DXVECTOR3(speed.x, 0, speed.z));
	if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	{
		ray.update(centralPosition, -ray.normal);
		// 壁までの距離を取得する
		hit = ray.rayIntersect(attractorMesh, *attractorMatrix);
	}

	// 壁ずり処理
	if (hit)
	{
		if (radius >= ray.distance)
		{
			// めり込み補正（現在位置 + 壁法線方向 * めり込み距離）
			setPosition(position + (-ray.normal) * (ray.distance - radius));

			// 移動ベクトルのスリップ（面方向へのベクトル成分の削除）
			setSpeed(slip(speed, ray.normal));
		}
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
void TmpObject::configurationGravityWithRay(D3DXVECTOR3* attractorPosition, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
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

//
////===================================================================================================================================
//// 物理挙動
////===================================================================================================================================
//void TmpObject::physicalBehavior()
//{
//	//------------
//	// 加速度処理
//	//------------
//	// 重力処理
//	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
//	gravityRay.update(position, gravityDirection);
//	if (onGround == false)
//	{
//		// 空中で重力をかける（地上では重力をかけない）
//		setGravity(gravityDirection, GRAVITY_FORCE);
//	}
//
//	//// 移動入力がないとき加速度を切る
//	//if (isExecutingMoveOperation == false)
//	//{
//	//	acceleration.x = 0.0f;
//	//	acceleration.z = 0.0f;
//	//}
//
//	//if (onGround)
//	//{
//	//	// 地上にいる場合は重力方向も切る
//	//	// ↓これを外すと最後に加速度.yに入っていた重力加速度がスリップして坂道滑り続ける
//	//	acceleration.y = 0.0f;
//	//}
//
//
//	// 空中に浮くタイミングで加速度切る
//	if (onGround == false && onGroundBefore)
//	{
//		acceleration *= 0.0f;
//	}
//
//	//----------
//	// 速度処理
//	//----------
//	// 着地するタイミングで速度が低下する
//	if (onGround && onGroundBefore == false)
//	{
//		friction *= GROUND_FRICTION;
//	}
//
//	// 地上摩擦係数
//	if (onGround)
//	{
//		friction *= MOVE_FRICTION;
//	}
//
//	//// 停止
//	//float speedPerSecond = D3DXVec3Length(&speed);
//	//if (isExecutingMoveOperation == false &&
//	//	speedPerSecond < STOP_SPEED)
//	//{
//	//	speed *= 0.0f;
//	//}
//
//	// 落下速度限界の設定
//	if (speed.y < -FALL_SPEED_MAX)
//	{
//		speed.y = -FALL_SPEED_MAX;
//	}
//}


//===================================================================================================================================
// 物理の更新
//===================================================================================================================================
void TmpObject::updatePhysics(float frameTime)
{
	// 加速度の影響を速度に与える
	speed += acceleration * frameTime;
	// 速度に摩擦の影響を与える
	speed *= friction;
	// 速度の影響を位置に与える
	position += speed * frameTime;
}
#pragma endregion

// [操作]
#pragma region Operation
//===================================================================================================================================
//【移動操作】
// [処理内容]入力された２次元ベクトルに応じてカメラ情報に基づき、速度へ加算処理を行う。
//===================================================================================================================================
void TmpObject::moveOperation()
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
	if (input->getController()[type]->checkConnect()) {
		move(input->getController()[type]->getLeftStick()*0.001f, camera->getDirectionX(), camera->getDirectionZ());
	}
}


//===================================================================================================================================
//【ジャンプ操作】
// [処理内容]（1）初入力でジャンプ踏切フラグをオン　(2)ジャンプを更新
//===================================================================================================================================
void TmpObject::jumpOperation()
{
	if (input->wasKeyPressed(keyTable.jump) || input->getController()[type]->wasButton(BUTTON_JUMP))
	{
		if (jumping == false) onJump = true;	// ジャンプ踏切フラグをオンにする
	}
	if (input->isKeyDown(keyTable.jump) || input->getController()[type]->isButton(BUTTON_JUMP))
	{
		jump();
	}
}


//===================================================================================================================================
//【カメラの操作/更新】
//===================================================================================================================================
void TmpObject::controlCamera(float frameTime)
{
	//操作軸反転操作
	if (input->wasKeyPressed(keyTable.reverseCameraX))reverseValueXAxis *= -1;
	if (input->wasKeyPressed(keyTable.reverseCameraY))reverseValueYAxis *= -1;
	//マウス操作
	camera->rotation(D3DXVECTOR3(0, 1, 0), (float)(input->getMouseRawX() * reverseValueXAxis));
	camera->rotation(camera->getHorizontalAxis(), (float)(input->getMouseRawY() * reverseValueYAxis));
	//コントローラ操作
	if (input->getController()[type]->checkConnect()) {
		camera->rotation(D3DXVECTOR3(0, 1, 0), input->getController()[type]->getRightStick().x*0.1f*frameTime*reverseValueXAxis);
		camera->rotation(camera->getHorizontalAxis(), input->getController()[type]->getRightStick().y*0.1f*frameTime*reverseValueYAxis);
	}

	camera->setUpVector(axisY.direction);
	camera->update();
}
#pragma endregion

// [アクション]
#pragma region Action
//===================================================================================================================================
//【移動】
// [処理内容1]入力された２次元ベクトルに応じてカメラ情報に基づき、速度へ加算処理を行う。
// [処理内容2]移動後の姿勢制御を行う。
//===================================================================================================================================
void TmpObject::move(D3DXVECTOR2 operationDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ)
{
	isExecutingMoveOperation = true;

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
		acceleration = moveDirection * MOVE_ACC * dash();
	}
	else
	{
		//acceleration += moveDirection * MOVE_ACC_WHEN_NOT_GROUND;
		acceleration.x += moveDirection.x * MOVE_ACC_WHEN_NOT_GROUND;
		acceleration.z += moveDirection.z * MOVE_ACC_WHEN_NOT_GROUND;
	}
	//姿勢制御
	postureControl(getAxisZ()->direction, moveDirection, 0.1f);
}


//===================================================================================================================================
//【ダッシュ】
// [処理内容]入力に応じてダッシュ倍率を返す
//===================================================================================================================================
float TmpObject::dash()
{
	if (input->isKeyDown(keyTable.dash))
		return tmpObjNS::DASH_MAGNIFICATION;
	return 1.0f;
}

//===================================================================================================================================
//【ジャンプ】
//===================================================================================================================================
void TmpObject::jump()
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
#pragma endregion

// [デバッグ]
#pragma region Debug
//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void TmpObject::outputGUI()
{
#ifdef _DEBUG

	//ImGui::Text(sceneName.c_str());

	if (ImGui::CollapsingHeader("TmpObjectInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;
		int oldMeshId = staticMeshNo;

		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));


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

			//エネミーの状態
		const char* listboxMesh[] = { "PLAYER", "WOLF", "TIGER","BEAR" ,"BATTERY" };
		ImGui::ListBox("Mesh", &ItemListboxMesh, listboxMesh, TMPOBJ_LIST::TMPOBJ_MAX);
		switch (ItemListboxMesh)
		{
		case TMPOBJ_LIST::TMPOBJ_PLAYER:
			staticMeshNo = staticMeshNS::YAMADA_ROBOT2;
			break;
		case TMPOBJ_LIST::TMPOBJ_WOLF:
			staticMeshNo = staticMeshNS::SAMPLE_REDBULL;
			break;
		case TMPOBJ_LIST::TMPOBJ_TIGER:
			staticMeshNo = staticMeshNS::SAMPLE_BUNNY;
			break;
		case TMPOBJ_LIST::TMPOBJ_BEAR:
			staticMeshNo = staticMeshNS::SAMPLE_HAT;
			break;
		case TMPOBJ_LIST::TMPOBJ_BATTERY:
			staticMeshNo = staticMeshNS::SAMPLE_SCISSORS;
			break;
		default:
			break;
		}
		if (oldMeshId != staticMeshNo)
		{
			resetMesh(staticMeshNo);
			oldMeshId = staticMeshNo;
		}
	}
#endif // _DEBUG
}

//===================================================================================================================================
//【リセット】
//===================================================================================================================================
void TmpObject::reset()
{
	position = START_POSITION;
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

//===================================================================================================================================
//【リセットメッシュ】
//===================================================================================================================================
void TmpObject::resetMesh(int meshId)
{
	bodyCollide.~BoundingSphere();
	bodyCollide.initialize(&position, staticMeshNS::reference(meshId)->mesh);	// コライダの初期化
	radius = bodyCollide.getRadius();											// メッシュ半径を取得
	centralPosition = position + bodyCollide.getCenter();						// 中心座標を設定
}

#pragma endregion

//===================================================================================================================================
//【setter】
//===================================================================================================================================
void TmpObject::setCamera(Camera* _camera) { camera = _camera; }


//===================================================================================================================================
//【getter】
//===================================================================================================================================
int TmpObject::getState() { return state; }
BoundingSphere* TmpObject::getBodyCollide() { return &bodyCollide; }
