//===================================================================================================================================
//【Bullet.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/05
// [更新日]2019/11/11
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Object.h"
#include "StaticMeshRenderer.h"
#include "LinkedList.h"
#include "Ray.h"

namespace bulletNS{
	const float		SPEED			= 100.0f;	//弾速
	const float		INTERVAL_TIME	= 0.25f;	//インターバル時間
	const float		RELOAD_TIME		= 0.5f;		//リロード時間
	const int		MAGAZINE_NUM	= 8;		//弾数
	const float		EXIST_TIME		= 1.0f;		//存在時間
}

//===================================================================================================================================
//【バレットクラス】
//===================================================================================================================================
class Bullet :	public Object
{
private:
	D3DXVECTOR3		launchPosition;			//発射位置
	Ray				ballisticRay;			//弾道レイ
	D3DXVECTOR3		speed;					//速度
	D3DXVECTOR3		endPoint;				//終着点
	D3DXVECTOR3		initialCollide;			//初期衝突地点
	D3DXVECTOR3		collidePosition;		//衝突位置

public:
//[基本処理]
	Bullet(Ray shootingRay);
	~Bullet();
	void update(float frameTime);
	void render();
	void collide();

};

//===================================================================================================================================
//【バレットマネージャクラス】
//===================================================================================================================================
class BulletManager :public Base
{
private:
	LinkedList<Bullet*>*	bulletList;		//バレットリスト
	StaticMeshRenderer*		renderer;		//レンダラー
	int						remaining;		//残弾数
	float					intervalTimer;	//次の発射までのインターバル時間
	float					reloadTimer;	//リロード時間
	bool					reloading;		//リロード中
public:

//[基本処理]
	//コンストラクタ
	BulletManager();
	//デストラクタ
	~BulletManager();
	//更新
	void update(float frameTime);
	//描画
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

//[アクション]
	//発射
	void launch(Ray shootingRay);
	//リロード
	void reload();
	
//[getter]
	int getRemaining();
	float getReloadTime();
	Bullet* getBullet(int i);
	int getNum();
};
