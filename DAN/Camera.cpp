//===================================================================================================================================
//【Camera.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/12/11
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Camera.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace cameraNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Camera::Camera()
{
	D3DXQuaternionIdentity(&relativeQuaternion);
	posture = D3DXQUATERNION(0, 0, 0, 1);
	D3DXMatrixRotationQuaternion(&world, &posture);
	nearZ = INIT_NEAR_Z;
	farZ = INIT_FAR_Z;
	betweenGaze = relativeDistance = 1.0f;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Camera::~Camera()
{
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
HRESULT Camera::initialize(DWORD _windowWidth, DWORD _windowHeight)
{
	setAspect(_windowWidth,_windowHeight);
	updateOrtho();
	return S_OK;
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Camera::update()
{
	if (target != NULL) {
		gazePosition = *target;
		position = *target;
	}
	else {
		position = (D3DXVECTOR3)relativeQuaternion;
	}
	
	D3DXVECTOR3 axisY(world._21, world._22, world._23);
	if(targetY != NULL)Base::postureControl(&posture,axisY,*targetY ,0.1f);

	D3DXVECTOR3 relativePosition = (D3DXVECTOR3)relativeQuaternion;
	D3DXVec3Normalize(&relativePosition,&relativePosition);
	relativePosition *= relativeDistance;

	//姿勢クォータニオンから姿勢行列を作成する
	D3DXMatrixRotationQuaternion(&world, &posture);
	position +=
		relativePosition.x*D3DXVECTOR3(world._11,world._12,world._13)+
		relativePosition.y*D3DXVECTOR3(world._21,world._22,world._23)+
		relativePosition.z*D3DXVECTOR3(world._31,world._32,world._33);
	gazePosition += 
		relativeGaze.x*D3DXVECTOR3(world._11,world._12,world._13)+
		relativeGaze.y*D3DXVECTOR3(world._21,world._22,world._23)+
		relativeGaze.z*D3DXVECTOR3(world._31,world._32,world._33);
	setViewProjection();
}

//===================================================================================================================================
//【カメラの描画準備】
//===================================================================================================================================
void Camera::renderReady()
{
	LPDIRECT3DDEVICE9 device = getDevice();
	device->SetTransform(D3DTS_VIEW, &view);
	device->SetTransform(D3DTS_PROJECTION, &projection);
}

//===================================================================================================================================
//【カメラの任意軸回転処理】
//===================================================================================================================================
void Camera::rotation(D3DXVECTOR3 axis,float degree)
{
	D3DXQUATERNION conjugateQ;
	D3DXQUATERNION rotationQ(0,0,0,1);
	float radian = D3DXToRadian(degree);
	D3DXQuaternionRotationAxis(&rotationQ, &axis, radian);
	D3DXQuaternionConjugate(&conjugateQ, &rotationQ);

	D3DXQUATERNION temporaryQ;
	//共役*回転対象*回転クォータニオン
	temporaryQ = conjugateQ * relativeQuaternion * rotationQ;

	D3DXVECTOR3 relativePosition = (D3DXVECTOR3)temporaryQ;
	float relativeLength = D3DXVec3Length(&relativePosition);
	if (limit & ROTATION_TOP_LIMIT && 
		relativePosition.y > relativeLength-limitValueRotaionTop)return;
	if (limit & ROTATION_BOTTOM_LIMIT && 
		relativePosition.y < -relativeLength+limitValueRotaionBottom)return;

	//共役*回転対象*回転クォータニオン
	relativeQuaternion = temporaryQ;
}

//===================================================================================================================================
//【回転時、相対位置ベクトルに高さ（上方向）制限をセットする】
//===================================================================================================================================
void Camera::setLimitRotationTop(float value)
{
	enableLimit(ROTATION_TOP_LIMIT);
	limitValueRotaionTop = value;
}

//===================================================================================================================================
//【回転時、相対位置ベクトルに高さ（下方向）制限をセットする】
//===================================================================================================================================
void Camera::setLimitRotationBottom(float value)
{
	enableLimit(ROTATION_BOTTOM_LIMIT);
	limitValueRotaionBottom = value;
}

//===================================================================================================================================
//【制限パラメータを有効にする】
//===================================================================================================================================
void Camera::enableLimit(int limitParameter)
{
	limit |= limitParameter;
}

//===================================================================================================================================
//【制限パラメータを無効にする】
//===================================================================================================================================
void Camera::disableLimit(int limitParameter)
{
	limit &= ~limitParameter;
}

//===================================================================================================================================
//【ターゲット位置をロックオンする】
//===================================================================================================================================
void Camera::lockOn(D3DXVECTOR3 lockOnTarget,float frameTime)
{
	D3DXVECTOR3 axis(0, 1, 0);
	float radian;

	//プレイヤーとロックオン対象の方向ベクトル
	D3DXVECTOR3 lockOnTargetDirection;
	between2VectorDirection(&lockOnTargetDirection, *target, lockOnTarget);
	lockOnTargetDirection = slip(lockOnTargetDirection, getDirectionY());
	D3DXVec3Normalize(&lockOnTargetDirection, &lockOnTargetDirection);
	D3DXVECTOR3 front = slip(getDirectionZ(), getDirectionY());
	D3DXVec3Normalize(&front, &front);
	if (formedRadianAngle(&radian, front, lockOnTargetDirection))
	{
		rotation(axis, D3DXToDegree(radian));
	}

}

//===================================================================================================================================
//【めり込み補正】
//===================================================================================================================================
bool Camera::insetCorrection(LPD3DXMESH mesh, D3DXMATRIX matrix) 
{
	Ray ray;
	if (target)		ray.initialize(*target, -getDirectionZ());
	else			ray.initialize(position, -getDirectionZ());

	if (ray.rayIntersect(mesh, matrix) && ray.distance < betweenGaze)
	{
		relativeDistance = max(0.01f,ray.distance);
	}
	else {
		relativeDistance = betweenGaze;//固定値の代入
	}

	return false;
}

//===================================================================================================================================
//【球による衝突判定】
//===================================================================================================================================
bool Camera::sphereCollide(D3DXVECTOR3 position, float radius)
{
	return false;
}

//===================================================================================================================================
//【レイによる衝突判定】
//===================================================================================================================================
bool Camera::rayCollide(LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	return false;
}

//===================================================================================================================================
//【注視点間距離の設定】
//===================================================================================================================================
void Camera::setGazeDistance(float newValue)
{
	betweenGaze = relativeDistance = newValue;
}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void Camera::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("Camera Property"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;

		float minFOV = (D3DX_PI / 180) * 1;
		float maxFOV = (D3DX_PI / 180) * 180;

		float minRelative = 0;
		float maxRelative = 5;

		ImGui::SliderFloat("fieldOfView", &fieldOfView, minFOV, maxFOV);						//視野角
		ImGui::SliderFloat("nearZ", &nearZ, INIT_NEAR_Z, INIT_FAR_Z);							//視認近距離
		ImGui::SliderFloat("farZ", &farZ, INIT_FAR_Z, INIT_FAR_Z*10);							//視認遠距離

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);						//位置
		ImGui::SliderFloat3("gazePosition", gazePosition, limitBottom, limitTop);				//注視位置
		ImGui::SliderFloat4("relativeQuaternion", relativeQuaternion, minRelative, maxRelative);//相対位置
		ImGui::SliderFloat("relativeDistance", &relativeDistance, 0.01f, 10.0f);				//注視点間距離
		ImGui::SliderFloat3("upVector", upVector, -1, 1);										//上方ベクトル
		ImGui::SliderFloat4("posture", posture, limitBottom, limitTop);							//姿勢クォータニオン

		ImGui::Checkbox("onGaze", &onGaze);														//注視フラグ

	}
#endif // _DEBUG
}

//===================================================================================================================================
//【正射影行列を更新する】
//===================================================================================================================================
void Camera::updateOrtho()
{
	//正射影行列を求める
	float nearH = nearZ * tanf(fieldOfView*0.5f)*2.0f;
	float nearW = nearH * (float)windowWidth / (float)windowHeight;
	D3DXMatrixOrthoLH(&ortho, nearW, nearH, nearZ, farZ);
}

//===================================================================================================================================
//【setter】
//===================================================================================================================================
//ウィンドウサイズによるアスペクト比の設定
void Camera::setAspect(DWORD _windowWidth, DWORD _windowHeight)
{
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;
	aspect = (FLOAT)windowWidth / (FLOAT)windowHeight;
}
//視野角の設定
void Camera::setFieldOfView(float value)
{
	fieldOfView = value;
}
//プロジェクション行列とビュー行列の設定
HRESULT Camera::setViewProjection()
{
	D3DXMatrixLookAtLH(&view, &position, &gazePosition, &upVector);
	// プロジェクション
	D3DXMatrixPerspectiveFovLH(&projection, fieldOfView, aspect, nearZ, farZ);
	return S_OK;
}
void Camera::setNearZ(float value) { nearZ = value; }
void Camera::setFarZ(float value) { farZ = value; }
void Camera::GetViewMatrix(D3DXMATRIX* viewOut, D3DXMATRIX* world)
{
	D3DXMATRIX CameraW;
	//カメラの姿勢をワールド変換
	D3DXMatrixMultiply(&CameraW, &view, world);
	//ビュー変換行列作成
	D3DXMatrixLookAtLH(
		viewOut,
		&D3DXVECTOR3(CameraW._11, CameraW._13, CameraW._13),
		&D3DXVECTOR3(CameraW._31, CameraW._32, CameraW._33),
		&D3DXVECTOR3(CameraW._21, CameraW._23, CameraW._23)
	);
}

//===================================================================================================================================
//【getter】
//===================================================================================================================================
D3DXVECTOR3 Camera::getAxisZ()	{
	D3DXVECTOR3 direction;
	Base::between2VectorDirection(&direction, position, gazePosition);
	return direction;}
D3DXVECTOR3 Camera::getAxisY()	{return D3DXVECTOR3(world._21, world._22, world._23);}
D3DXVECTOR3 Camera::getHorizontalAxis()
{
	D3DXVECTOR3 axis;
	D3DXVec3Cross(&axis,&D3DXVECTOR3(0,1,0),&D3DXVECTOR3(relativeQuaternion.x, relativeQuaternion.y, relativeQuaternion.z));
	if (isnan(axis.x)||isnan(axis.y)||isnan(axis.z))return D3DXVECTOR3(1,0,0);	
	return axis;
}
