//===================================================================================================================================
//【Object.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/12/05
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
//【スタティック変数】
//===================================================================================================================================
int Object::objectCounter = 0;	//IDの割当に使用

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Object::Object()
{
	id					= objectCounter;									//IDの割当
	objectCounter++;														//オブジェクトカウンターの加算：IDの割当に使用
	treeCell.type		= ObjectType::NONE;									//オブジェクトタイプ：初期値NONE
	treeCell.target		= ObjectType::NONE;									//オブジェクトタイプ：初期値NONE

	position			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//位置
	quaternion			= D3DXQUATERNION(0, 0, 0, 1);						//回転
	scale				= D3DXVECTOR3(1.0f,1.0f,1.0f);						//スケール
	radius				= 5.0f;												//半径
	size				= D3DXVECTOR3(1.0f,1.0f,1.0f);						//サイズの設定
	alpha				= 1.0f;												//α値の設定
	speed				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//速度
	color				= D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	onGravity			= false;											//重力フラグ
	onActive			= true;												//アクティブフラグ

	axisX.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(1, 0, 0));			//x軸
	axisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));			//y軸
	axisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1));			//z軸
	reverseAxisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-1, 0, 0));	//-x軸
	reverseAxisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -1, 0));	//-y軸
	reverseAxisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, -1));	//-z軸
	sphere = new BoundingSphere(&center, radius);
	box					= new DebugBox(size);
#ifdef _DEBUG
	axisX.color			= D3DXCOLOR(255, 0, 0, 255);						//x軸カラー
	axisY.color			= D3DXCOLOR(0, 255, 0, 255);						//y軸カラー
	axisZ.color			= D3DXCOLOR(0, 0, 255, 255);						//z軸カラー
	reverseAxisX.color	= D3DXCOLOR(255, 0, 0, 255);						//-x軸カラー
	reverseAxisY.color	= D3DXCOLOR(0, 255, 0, 255);						//-y軸カラー
	reverseAxisZ.color	= D3DXCOLOR(0, 0, 255, 255);						//-z軸カラー
#endif // _DEBUG

	D3DXMatrixIdentity(&matrixPosition);									//位置行列
	D3DXMatrixIdentity(&matrixRotation);									//回転行列
	D3DXMatrixIdentity(&matrixScale);										//スケール行列
	D3DXMatrixIdentity(&matrixCenter);										//センター行列
	D3DXMatrixIdentity(&matrixWorld);										//ワールド

	existenceTimer = 1.0f;													//存在時間

	treeCell.object = this;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Object::~Object()
{
	treeCell.remove();//リストから外れる
	SAFE_DELETE(box);
	SAFE_DELETE(sphere);
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
	D3DXMatrixIdentity(&matrixWorld);									//正規化
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixScale);		//*スケール行列
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixRotation);		//*回転行列
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixPosition);		//*位置行列

	//ワールド座標から自身の軸レイを更新する
	center = position + D3DXVECTOR3(0.0f,size.y/2,0.0f);
	//センター行列の作成（センター座標→センター行列への変換）
	D3DXMatrixTranslation(&matrixCenter, center.x, center.y, center.z);
	axisX.update(center, D3DXVECTOR3(matrixWorld._11,matrixWorld._12,matrixWorld._13));
	axisY.update(center, D3DXVECTOR3(matrixWorld._21,matrixWorld._22,matrixWorld._23));
	axisZ.update(center, D3DXVECTOR3(matrixWorld._31,matrixWorld._32,matrixWorld._33));
	reverseAxisX.update(center, -D3DXVECTOR3(matrixWorld._11, matrixWorld._12, matrixWorld._13));
	reverseAxisY.update(center, -D3DXVECTOR3(matrixWorld._21, matrixWorld._22, matrixWorld._23));
	reverseAxisZ.update(center, -D3DXVECTOR3(matrixWorld._31, matrixWorld._32, matrixWorld._33));
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
	box->render(matrixCenter);
	sphere->render();
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

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//位置
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
//【getter】
//===================================================================================================================================
D3DXMATRIX*		Object::getMatrixWorld() { return &matrixWorld; }
D3DXVECTOR3*	Object::getPosition() { return &position; };
D3DXQUATERNION	Object::getQuaternion() { return quaternion; };
float			Object::getRadius() { return radius; }
D3DXVECTOR3		Object::getSpeed() { return speed; }
D3DXVECTOR3		Object::getAcceleration() { return acceleration; }
D3DXVECTOR3		Object::getGravity() { return gravity; };
Ray*			Object::getAxisX() { return &axisX; };
Ray*			Object::getAxisY() { return &axisY; };
Ray*			Object::getAxisZ() { return &axisZ; };
Ray*			Object::getReverseAxisX() { return &reverseAxisX; };
Ray*			Object::getReverseAxisY() { return &reverseAxisY; };
Ray*			Object::getReverseAxisZ() { return &reverseAxisZ; };
Ray*			Object::getGravityRay() { return &gravityRay; };
bool			Object::getActive() { return onActive; }
float			Object::getRight() { return position.x + radius; }
float			Object::getLeft() { return position.x - radius; }
float			Object::getTop() { return position.z + radius; }
float			Object::getBottom() { return position.z - radius; }
float			Object::getFront() { return position.z + radius; }
float			Object::getBack() { return position.z - radius; }
D3DXVECTOR3		Object::getMin() { return position - (size / 2); }
D3DXVECTOR3		Object::getMax() { return position + (size / 2); }

//===================================================================================================================================
//【setter】
//===================================================================================================================================
void Object::setSpeed(D3DXVECTOR3 _speed) { speed = _speed; }
void Object::addSpeed(D3DXVECTOR3 add) { speed += add; }
void Object::setPosition(D3DXVECTOR3 _position) { position = _position; }
void Object::setQuaternion(D3DXQUATERNION _quaternion) { quaternion = _quaternion; }
void Object::activation() { onActive = true; }
void Object::inActivation() { onActive = false; }
void Object::setAlpha(float value) { alpha = value; }
void Object::setRadius(float value)
{
	radius = value;
	SAFE_DELETE(sphere);
	sphere = new BoundingSphere(&center,radius);
}
void Object::setSize(D3DXVECTOR3 value) { 
	size = value; 
	SAFE_DELETE(box);
	box = new DebugBox(size);
#ifdef _DEBUG
#endif // _DEBUG
}

//===================================================================================================================================
//【オブジェクトIDのカウンターをリセット】
//===================================================================================================================================
void objectNS::resetCounter()
{
	Object::objectCounter = 0;
}