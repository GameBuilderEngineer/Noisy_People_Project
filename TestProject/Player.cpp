//===================================================================================================================================
//【Player.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/24
// [更新日]2019/10/04
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Player.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace playerNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Player::Player() :StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_STATIC_MESH))
{
	ZeroMemory(&keyTable, sizeof(OperationKeyTable));
	onGravity = true;
	radius = 5.0f;
	activation();
	state = GROUND;

	invincibleTimer = 0.0f;					//無敵時間
	onGround = false;						//接地判定
	reverseValueXAxis = CAMERA_SPEED;		//操作Ｘ軸
	reverseValueYAxis = CAMERA_SPEED;		//操作Ｙ軸
	onJump = false;							//ジャンプフラグ
	difference = DIFFERENCE_FIELD;			//フィールド補正差分
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Player::~Player()
{

}

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
		ImGui::Checkbox("onRender", &onRender);											//描画有効化フラグ
		ImGui::Checkbox("onLighting", &onLighting);										//光源処理フラグ
		ImGui::Checkbox("onTransparent", &onTransparent);								//透過フラグ

		ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);						//透過値の操作有効フラグ
	}
#endif // _DEBUG
}


//===================================================================================================================================
//【初期化】
//===================================================================================================================================
//プレイヤータイプごとに初期化内容を変更
void Player::initialize(int playerType, int modelType) {
	device = getDevice();
	input = getInput();
	type = playerType;
	keyTable = KEY_TABLE_1P;
	StaticMeshObject::initialize(&(D3DXVECTOR3)START_POSITION);
	bodyCollide.initialize(&position, staticMesh->mesh);
	radius = bodyCollide.getRadius();
}

//===================================================================================================================================
//【更新】
//[処理内容1]移動処理
//[処理内容2]ジャンプ処理
//[処理内容3]重力処理
//[処理内容4]接地処理
//===================================================================================================================================
void Player::update(float frameTime)
{
	//前処理
	setSpeed(D3DXVECTOR3(0, 0, 0));	//速度（移動量）の初期化
	onJump = false;					//ジャンプフラグ

	//移動処理
	moveOperation();

	//ジャンプ
	if (input->wasKeyPressed(keyTable.jump) ||
		input->getController()[type]->wasButton(BUTTON_JUMP))
	{
		onJump = true;
	}

	//接地処理
	updateGround(frameTime,onJump);

	//加速度処理
	if (D3DXVec3Length(&acceleration) > 0.05f)
	{//加速度が小さい場合、加算しない
		speed += acceleration;
	}

	//位置更新
	position += speed*frameTime;
	
	//姿勢制御
	postureControl(axisY.direction, -gravityRay.direction,3.0f * frameTime);
	
	//オブジェクト：更新
	Object::update();

	//カメラの操作
	controlCamera(frameTime);

}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
//======================
//【トゥーンレンダー】
//======================
void Player::toonRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition,
	LPD3DXEFFECT effect, LPDIRECT3DTEXTURE9 textureShade, LPDIRECT3DTEXTURE9 textureLine)
{
	//Object::toonRender(device,view,projection, cameraPosition,effect,textureShade,textureLine);
	// 他のオブジェクトの描画
	//otherRender(device,view,projection,cameraPosition);
}
//======================
//【通常描画】
//======================
void Player::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	StaticMeshObject::render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH),view,projection, cameraPosition);
	//他のオブジェクトの描画
	//otherRender(device,view,projection,cameraPosition);
}
//======================
//【本体以外の他のオブジェクト描画】
//======================
void Player::otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//デバッグ時描画
#ifdef _DEBUG
	bodyCollide.render(matrixWorld);
#endif // _DEBUG
}

//===================================================================================================================================
//【移動操作】
// [処理内容1]入力された２次元ベクトルに応じてカメラ情報に基づき、速度へ加算処理を行う。
//===================================================================================================================================
void Player::moveOperation()
{
	//キーによる移動
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

	//リセット
	if (input->wasKeyPressed(keyTable.reset))
	{
		reset();
	}
	//コントローラスティックによる移動
	if (input->getController()[type]->checkConnect()) {
		move(input->getController()[type]->getLeftStick()*0.001f, camera->getDirectionX(), camera->getDirectionZ());
	}

}

//===================================================================================================================================
//【移動】
// [処理内容1]入力された２次元ベクトルに応じてカメラ情報に基づき、速度へ加算処理を行う。
// [処理内容2]移動後の姿勢制御を行う。
//===================================================================================================================================
void Player::move(D3DXVECTOR2 operationDirection,D3DXVECTOR3 cameraAxisX,D3DXVECTOR3 cameraAxisZ)
{
	if (operationDirection.x == 0 && operationDirection.y == 0)return;//入力値が0以下ならば移動しない
	//Y軸方向への成分を削除する
	D3DXVECTOR3 front = slip(cameraAxisZ, axisY.direction);
	D3DXVECTOR3 right = slip(cameraAxisX, axisY.direction);
	D3DXVec3Normalize(&front, &front);//正規化
	D3DXVec3Normalize(&right, &right);//正規化

	//操作方向をカメラのXZ方向に準拠した移動ベクトルへ変換する
	D3DXVECTOR3 moveDirection = operationDirection.x*right + -operationDirection.y*front;
	if (onGround) {
		addSpeed(moveDirection*SPEED);
	}
	else {
		addSpeed(moveDirection*SPEED/10);
	}
	//姿勢制御
	postureControl(getAxisZ()->direction, moveDirection, 0.1f);
}

//===================================================================================================================================
//【ジャンプ】
// [処理内容]加速度へ加算処理を行う
//===================================================================================================================================
void Player::jump()
{
	acceleration += axisY.direction * JUMP_FORCE+speed/0.9f;
	onGround = false;
}

//===================================================================================================================================
//【リセット】
//===================================================================================================================================
void Player::reset()
{
	position = START_POSITION;
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
//【地上モード 更新処理】
//===================================================================================================================================
void Player::updateGround(float frameTime, bool _onJump)
{
	//===========
	//【接地処理】
	//===========
	//重力線を作成
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0,-1,0);
	gravityRay.initialize(position, gravityDirection);								//重力レイの初期化
	if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix)&&
		radius + difference >= gravityRay.distance )
	{
		onGround = true;
		onGravity = false;
		//めり込み補正
		//現在位置+ 重力方向*(めり込み距離)
		setPosition(position + gravityRay.direction * (gravityRay.distance-radius));
		//移動ベクトルのスリップ（面方向へのベクトル成分の削除）
		setSpeed(slip(speed, gravityRay.normal));
		acceleration *= 0;
		if (_onJump)jump();//ジャンプ
	}
	else {
		//空中
		onGround = false;
		onGravity = true;
	}
	setGravity(gravityDirection, GRAVITY_FORCE*frameTime);//重力処理

}

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
	attractorMesh =_attractorMesh;
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
	if (input->getController()[type]->checkConnect()) {
		camera->rotation(D3DXVECTOR3(0, 1, 0), input->getController()[type]->getRightStick().x*0.1f*frameTime*reverseValueXAxis);
		camera->rotation(camera->getHorizontalAxis(), input->getController()[type]->getRightStick().y*0.1f*frameTime*reverseValueYAxis);
	}

	camera->setUpVector(axisY.direction);
	camera->update();
}

//===================================================================================================================================
//【setter】
//===================================================================================================================================
void Player::setCamera(Camera* _camera) { camera = _camera; }

//===================================================================================================================================
//【getter】
//===================================================================================================================================
int Player::getState() { return state; }
BoundingSphere* Player::getBodyCollide() { return &bodyCollide; }
