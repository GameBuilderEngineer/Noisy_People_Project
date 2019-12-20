//-----------------------------------------------------------------------------
// タイガーバレット [TigerBullet.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/10
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "StaticMeshRenderer.h"
#include "LinkedList.h"
#include "Sound.h"
#include "EffekseerManager.h"


// バレットエフェクトクラス
class TigerBulletEffect :public effekseerNS::Instance
{
public:
public:
	D3DXMATRIX* syncMatrix;
	TigerBulletEffect(D3DXMATRIX* syncMatrix) {
		this->syncMatrix = syncMatrix;
		effectNo = effekseerNS::TIGER_BULLET;
		scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	virtual void update() {

		D3DXMATRIX M = *syncMatrix;

		::Effekseer::Manager*	manager = getEffekseerManager(managerNo)->manager;
		::Effekseer::Matrix43 matrix;
		matrix.Value[0][0] = M._11;
		matrix.Value[0][1] = M._12;
		matrix.Value[0][2] = M._13;
		matrix.Value[1][0] = M._21;
		matrix.Value[1][1] = M._22;
		matrix.Value[1][2] = M._23;
		matrix.Value[2][0] = M._31;
		matrix.Value[2][1] = M._32;
		matrix.Value[2][2] = M._33;
		matrix.Value[3][0] = M._41;
		matrix.Value[3][1] = M._42;
		matrix.Value[3][2] = M._43;
		manager->SetMatrix(handle, matrix);
	};
};


//=============================================================================
// バレット
//=============================================================================
class TigerBullet: public Object
{
private:
	D3DXVECTOR3			launchPosition;			//発射位置
	Ray					ballisticRay;			//弾道レイ
	D3DXVECTOR3			bulletSpeed;			//速度
	D3DXVECTOR3			endPoint;				//終着点
	D3DXVECTOR3			initialCollide;			//初期衝突地点
	D3DXVECTOR3			collidePosition;		//衝突位置
	TigerBulletEffect*	tigerBulletEffect;		//バレットエフェクト
	bool isHit;

public:
	const float		SPEED = 25.0f;			//弾速
	const float		EXIST_TIME = 3.0f;		//存在時間

	TigerBullet(Ray shootingRay);
	~TigerBullet();
	void update(float frameTime);
	void render();
	bool collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix);

	void hit();

	// Getter
	bool getIsHit() { return isHit; }
	// Setter
	void setIsHit(bool set) { isHit = set; }

};


//=============================================================================
// バレットマネージャー
//=============================================================================
class TigerBulletManager: public Base
{
private:
	LinkedList<TigerBullet*>	bulletList;		// バレットリスト
	int							remaining;		// 残弾数
	float						intervalTimer;	// 次の発射までのインターバル時間
	bool						isShot;			// 発射したか

public:
	const int MAGAZINE_NUM = 3;
	const float INTERVAL_TIME = 0.5f;
	const float RELOAD_TIME = 2.0f;

	TigerBulletManager();
	~TigerBulletManager();
	// 更新
	void update(float frameTime);
	// 描画
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	// 発射
	bool shoot(Ray shootingRay);
	// 弾の破棄
	void destroy(TigerBullet* bullet, int nodeNumber);
	// バレットリストの取得
	LinkedList<TigerBullet*>* getBulletList();
};