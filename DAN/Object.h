//===================================================================================================================================
//【Object.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/23
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
#include "LinearTreeCell.h"

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

	void resetCounter();
}

//===================================================================================================================================
//【オブジェクトクラス】
//===================================================================================================================================
class Object:public Base
{
public:
	//Data
	//ステータス変数
	static int			objectCounter;			//オブジェクトカウンター：IDの割当に使用
	int					id;						//ID
	D3DXVECTOR3			position;				//位置
	D3DXQUATERNION		quaternion;				//回転
	D3DXVECTOR3			scale;					//スケール
	float				radius;					//衝突半径
	float				alpha;					//透過値

	//移動系変数
	D3DXVECTOR3			speed;					//速度
	D3DXVECTOR3			acceleration;			//加速度
	D3DXVECTOR3			gravity;				//重力

	//各種フラグ
	bool				onGravity;				//重力有効化フラグ
	bool				onActive;				//アクティブ化フラグ

	//方向6軸
	Ray					axisX;					//x軸
	Ray					axisY;					//y軸
	Ray					axisZ;					//z軸
	Ray					reverseAxisX;			//-x軸
	Ray					reverseAxisY;			//-y軸
	Ray					reverseAxisZ;			//-z軸

	//重力Ray
	Ray					gravityRay;				//重力Ray

	//行列（位置・回転・スケール・ワールド）
	D3DXMATRIX			matrixPosition;			//位置行列
	D3DXMATRIX			matrixRotation;			//回転行列
	D3DXMATRIX			matrixScale;			//スケール行列
	D3DXMATRIX			matrixWorld;			//ワールド行列

	//タイマー
	float				existenceTimer;			//存在時間

	//オブジェクトツリーへ所属するためのインターフェースクラス
	ObjectTree<Object>	treeCell;		//木空間

	//Method
	Object();
	virtual ~Object();
	
	//基本関数
	HRESULT initialize(D3DXVECTOR3* _position);
	void update();

	//デバッグ関係関数
	void debugRender();				//デバッグ用ユーティリティレンダー
	virtual void outputGUI();			//ImGUIへObjectの情報を出力

	//getter
	D3DXMATRIX*				getMatrixWorld()								{ return &matrixWorld; }	
	D3DXVECTOR3*			getPosition()									{ return &position; };
	D3DXQUATERNION			getQuaternion()									{ return quaternion; };
	float					getRadius()										{ return radius; }
	D3DXVECTOR3				getSpeed()										{ return speed; }
	D3DXVECTOR3				getAcceleration()								{ return acceleration; }
	D3DXVECTOR3				getGravity()									{ return gravity; };
	Ray*					getAxisX()										{ return &axisX; };
	Ray*					getAxisY()										{ return &axisY; };
	Ray*					getAxisZ()										{ return &axisZ; };
	Ray*					getReverseAxisX()								{ return &reverseAxisX; };
	Ray*					getReverseAxisY()								{ return &reverseAxisY; };
	Ray*					getReverseAxisZ()								{ return &reverseAxisZ; };
	Ray*					getGravityRay()									{ return &gravityRay; };
	bool					getActive()										{ return onActive; }
	float					getRight()										{ return position.x + radius;}
	float					getLeft()										{ return position.x - radius;}
	float					getTop()										{ return position.z + radius;}
	float					getBottom()										{ return position.z - radius;}
	float					getFront()										{ return position.z + radius;}
	float					getBack()										{ return position.z - radius;}
	D3DXVECTOR3				getMin()										{ return position - D3DXVECTOR3(radius,radius,radius);}
	D3DXVECTOR3				getMax()										{ return position + D3DXVECTOR3(radius,radius,radius);}

	//setter
	void					setSpeed(D3DXVECTOR3 _speed)					{ speed = _speed; }
	void					addSpeed(D3DXVECTOR3 add)						{ speed += add; }
	void					setPosition(D3DXVECTOR3 _position)				{ position = _position; }
	void					setQuaternion(D3DXQUATERNION _quaternion)		{ quaternion = _quaternion; }
	void					setAlpha(float value);							//α値の設定
	void					setGravity(D3DXVECTOR3 source, float power);	//重力の設定
	void					activation();									//アクティブ化
	void					inActivation();									//非アクティブ化

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

