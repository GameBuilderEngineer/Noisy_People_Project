//===================================================================================================================================
//【Object.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/09/23
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "Object.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace objectNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Object::Object()
{
	ZeroMemory(&position, sizeof(D3DXVECTOR3));												//位置
	quaternion				= D3DXQUATERNION(0, 0, 0, 1);										//回転
	scale						= D3DXVECTOR3(1.0f,1.0f,1.0f);										//スケール
	radius					= 0.0f;																				//半径
	alpha						= 1.0f;																				//α値の設定

	ZeroMemory(&speed, sizeof(D3DXVECTOR3));													//速度

	onGravity				= false;																				//重力フラグ
	onActive				= true;																				//アクティブフラグ

	axisX.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(1, 0, 0));						//x軸
	axisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));						//y軸
	axisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1));						//z軸
	reverseAxisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-1, 0, 0));			//-x軸
	reverseAxisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -1, 0));			//-y軸
	reverseAxisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, -1));			//-z軸
#ifdef _DEBUG
	axisX.color				= D3DXCOLOR(255, 0, 0, 255);											//x軸カラー
	axisY.color				= D3DXCOLOR(0, 255, 0, 255);											//y軸カラー
	axisZ.color				= D3DXCOLOR(0, 0, 255, 255);											//z軸カラー
	reverseAxisX.color	= D3DXCOLOR(255, 0, 0, 255);											//-x軸カラー
	reverseAxisY.color	= D3DXCOLOR(0, 255, 0, 255);											//-y軸カラー
	reverseAxisZ.color	= D3DXCOLOR(0, 0, 255, 255);											//-z軸カラー
#endif // _DEBUG

	ZeroMemory(&matrixPosition, sizeof(D3DXMATRIX));										//位置行列
	D3DXMatrixIdentity(&matrixRotation);																//回転行列
	ZeroMemory(&matrixScale, sizeof(D3DXMATRIX));											//スケール行列
	ZeroMemory(&matrixWorld, sizeof(D3DXMATRIX));											//ワールド行列

	existenceTimer = 1.0f;																					//存在時間

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Object::~Object()
{
	
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
HRESULT Object::initialize(D3DXVECTOR3* _position)
{
	//初期位置
	memcpy(position, _position, sizeof(D3DXVECTOR3));
	activation();
	//事前更新
	update();
	return S_OK;
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Object::update()
{
	if (onActive == false)return;
	//位置行列の作成（位置座標→位置行列への変換）	
	D3DXMatrixTranslation(&matrixPosition, position.x, position.y, position.z);

	//回転行列の作成（クォータニオン→回転行列への変換）
	D3DXMatrixRotationQuaternion(&matrixRotation, &quaternion);
	
	//スケール行列の作成（スケール値→スケール行列への変換）
	D3DXMatrixScaling(&matrixScale, scale.x, scale.y, scale.z);

	//ワールド行列＝スケール行列*回転行列*位置行列
	D3DXMatrixIdentity(&matrixWorld);														//正規化
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixScale);			//*スケール行列
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixRotation);		//*回転行列
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixPosition);		//*位置行列

	//ワールド座標から自身の軸レイを更新する
	axisX.update(position, D3DXVECTOR3(matrixWorld._11,matrixWorld._12,matrixWorld._13));
	axisY.update(position, D3DXVECTOR3(matrixWorld._21,matrixWorld._22,matrixWorld._23));
	axisZ.update(position, D3DXVECTOR3(matrixWorld._31,matrixWorld._32,matrixWorld._33));
	reverseAxisX.update(position, -D3DXVECTOR3(matrixWorld._11, matrixWorld._12, matrixWorld._13));
	reverseAxisY.update(position, -D3DXVECTOR3(matrixWorld._21, matrixWorld._22, matrixWorld._23));
	reverseAxisZ.update(position, -D3DXVECTOR3(matrixWorld._31, matrixWorld._32, matrixWorld._33));
}

//===================================================================================================================================
//【デバッグ用描画】
//===================================================================================================================================
void Object::debugRender()
{
#ifdef _DEBUG
	axisX.render(10.0f);
	axisY.render(10.0f);
	axisZ.render(10.0f);
	reverseAxisX.render(10.0f);
	reverseAxisY.render(10.0f);
	reverseAxisZ.render(10.0f);
#endif // _DEBUG
}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void Object::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("ObjectInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop		= 1000;
		float limitBottom	= -1000;

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);					//位置
		ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//回転
		ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);							//スケール
		ImGui::SliderFloat("radius", &radius, 0, limitTop);										//半径
		ImGui::SliderFloat("alpha", &alpha, 0, 255);												//透過値
		ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);						//速度
		ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//加速度
		ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);					//重力
		
		ImGui::Checkbox("onGravity", &onGravity);												//重力有効化フラグ
		ImGui::Checkbox("onActive", &onActive);												//アクティブ化フラグ
		
	}
#endif // _DEBUG
}

//===================================================================================================================================
//【重力を設定し、加速度へ加算する】
//===================================================================================================================================
void Object::setGravity(D3DXVECTOR3 source, float power)
{
	D3DXVec3Normalize(&gravity, &source);
	//gravity *= min(power, reverseAxisY.distance);
	gravity *= power;
	
	if (onGravity)acceleration += gravity;
}

//===================================================================================================================================
//【setter】
//===================================================================================================================================
void Object::activation()					{ onActive = true;}
void Object::inActivation()				{ onActive = false;}
void Object::setAlpha(float value)	{ alpha = value;}
