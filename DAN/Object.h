//===================================================================================================================================
//【Object.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/09/23
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include <stdio.h>
#include <windows.h>
#include <d3dx9.h>
#include "Ray.h"
#include "StaticMeshLoader.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "Animation.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace objectNS {
	enum FILLMODE
	{
		SOLID		= (int)D3DFILL_SOLID,
		WIREFRAME	= (int)D3DFILL_WIREFRAME,
		POINT		= (int)D3DFILL_POINT,
	};
}

//===================================================================================================================================
//【オブジェクトクラス】
//===================================================================================================================================
class Object:public Base
{
protected:
	//Data
	//ステータス変数
	D3DXVECTOR3 position;				//位置
	D3DXQUATERNION quaternion;	//回転
	D3DXVECTOR3 scale;					//スケール
	float radius;								//衝突半径
	float alpha;								//透過値
	int fillMode;

	//移動系変数
	D3DXVECTOR3 speed;			//速度
	D3DXVECTOR3 acceleration;	//加速度
	D3DXVECTOR3 gravity;		//重力

	//各種フラグ
	bool onGravity;				//重力有効化フラグ
	bool onActive;				//アクティブ化フラグ
	bool onRender;				//描画有効化フラグ
	bool onLighting;			//光源処理フラグ
	bool onTransparent;			//透過フラグ

	//方向6軸
	Ray axisX;					//x軸
	Ray axisY;					//y軸
	Ray axisZ;					//z軸
	Ray reverseAxisX;			//-x軸
	Ray reverseAxisY;			//-y軸
	Ray reverseAxisZ;			//-z軸

	//重力Ray
	Ray gravityRay;

	//行列（位置・回転・ワールド）：スケール追加予定
	D3DXMATRIX matrixPosition;
	D3DXMATRIX matrixRotation;
	D3DXMATRIX matrixWorld;

	//インスタンシング変数
	//描画する数
	int renderNum;
	//インスタンシング描画時の各オブジェクトの位置バッファー
	LPDIRECT3DVERTEXBUFFER9 positionBuffer;
	LPDIRECT3DVERTEXDECLARATION9 declaration;	// 頂点宣言

public:
	//Method
	Object();
	~Object();
	
	//processing
	HRESULT initialize(D3DXVECTOR3* _position);
	void update();

	//デバッグ用ユーティリティレンダー
	void debugRender();

	//ImGUIへObjectの情報を出力
	virtual void outputGUI();	

	//getter
	//LPD3DXMESH* getMesh() { return &staticMesh->mesh; }
	D3DXMATRIX *getMatrixWorld() { return &matrixWorld; }	
	D3DXVECTOR3* getPosition() { return &position; };
	D3DXQUATERNION getQuaternion() { return quaternion; };
	float getRadius() { return radius; }
	D3DXVECTOR3 getSpeed() { return speed; }
	D3DXVECTOR3 getAcceleration() { return acceleration; }
	D3DXVECTOR3 getGravity() { return gravity; };
	Ray* getAxisX() { return &axisX; };
	Ray* getAxisY() { return &axisY; };
	Ray* getAxisZ() { return &axisZ; };
	Ray* getReverseAxisX() { return &reverseAxisX; };
	Ray* getReverseAxisY() { return &reverseAxisY; };
	Ray* getReverseAxisZ() { return &reverseAxisZ; };
	Ray* getGravityRay() { return &gravityRay; };
	bool getActive() { return onActive; }

	//setter
	void setSpeed(D3DXVECTOR3 _speed) { speed = _speed; }
	void addSpeed(D3DXVECTOR3 add) { speed += add; }
	void setGravity(D3DXVECTOR3 source, float power);
	void setPosition(D3DXVECTOR3 _position) { position = _position; }
	void setQuaternion(D3DXQUATERNION _quaternion) { quaternion = _quaternion; }
	void setAlpha(float value);
	void activation();								//アクティブ化
	void inActivation();							//非アクティブ化
	void switchTransparent(bool flag);				//透過の切り替え
	void setRenderFlag(bool frag);					//描画フラグのセット

	//描画数をセット
	//num:描画する数を指定
	//positionBuffer:各オブジェクトの位置を保存した配列のポインタ
	//void setInstancing(LPDIRECT3DDEVICE9 device, int num, D3DXVECTOR3* _positionList);

	//姿勢制御
	void postureControl(D3DXVECTOR3 currentDirection, D3DXVECTOR3 nextDirection,float t)
	{
		Base::postureControl(&quaternion,currentDirection, nextDirection, t);
	}

	//任意軸線型補間回転
	void anyAxisRotationSlerp(D3DXVECTOR3 axis, float radian,float t)
	{
		Base::anyAxisRotationSlerp(&quaternion, axis, radian, t);
	}
	
	//任意軸回転
	void anyAxisRotation(D3DXVECTOR3 axis, float degree)
	{
		Base::anyAxisRotation(&quaternion, axis, degree);
	}
};

