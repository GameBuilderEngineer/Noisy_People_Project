//===================================================================================================================================
//【Camera.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/11/13
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
	ZeroMemory(this, sizeof(Camera));
	D3DXQuaternionIdentity(&relativeQuaternion);
	posture = D3DXQUATERNION(0, 0, 0, 1);
	D3DXMatrixRotationQuaternion(&world, &posture);
	nearZ = INIT_NEAR_Z;
	farZ = INIT_FAR_Z;
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
		ImGui::SliderFloat3("upVector", upVector, -1, 1);										//上方ベクトル
		ImGui::SliderFloat4("posture", posture, limitBottom, limitTop);							//姿勢クォータニオン

		ImGui::Checkbox("onGaze", &onGaze);														//注視フラグ

	}
#endif // _DEBUG
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

//===================================================================================================================================
//【getter】
//===================================================================================================================================
D3DXVECTOR3 Camera::getAxisZ()	{return D3DXVECTOR3(world._31, world._32, world._33);}
D3DXVECTOR3 Camera::getAxisY()	{return D3DXVECTOR3(world._21, world._22, world._23);}
D3DXVECTOR3 Camera::getHorizontalAxis()
{
	D3DXVECTOR3 axis;
	D3DXVec3Cross(&axis,&D3DXVECTOR3(0,1,0),&D3DXVECTOR3(relativeQuaternion.x, relativeQuaternion.y, relativeQuaternion.z));
	if (isnan(axis.x)||isnan(axis.y)||isnan(axis.z))return D3DXVECTOR3(1,0,0);	
	return axis;
}
