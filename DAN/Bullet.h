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
#include "Sound.h"
#include "EffekseerManager.h"

namespace bulletNS{
	const float		SPEED			= 100.0f;	//弾速
	const float		INTERVAL_TIME	= 0.25f;	//インターバル時間
	const float		RELOAD_TIME		= 0.5f;		//リロード時間
	const int		MAGAZINE_NUM	= 8;		//弾数
	const float		EXIST_TIME		= 30.0f;	//存在時間
	const int		DIGITAL_POWER	= 20;		//デジタルパワー
	const float		LAUNCH_FACT_TIME = 0.333f;	//発射事実残存時間


	//銃口エフェクト
	class Muzzle :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		D3DXMATRIX* syncMatrixRotation;
		Muzzle(D3DXVECTOR3* syncPosition, D3DXMATRIX* syncMatrixRotation) {
			this->syncPosition = syncPosition;
			this->syncMatrixRotation = syncMatrixRotation;
			effectNo = effekseerNS::MUZZLE;
			scale = D3DXVECTOR3(0.1f,0.1f,0.1f);
		}
		virtual void update() {
			position = *syncPosition;
			
			D3DXMATRIX R = *syncMatrixRotation;

			rotation.x = atan2f(R._32, R._33);
			rotation.y = asinf(-R._31);
			rotation.z = atan2f(R._21, R._11);

			Instance::update();
		};
	};

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
	int				digitalPower;			//デジタルパワー
public:
//[基本処理]
	Bullet(Ray shootingRay);
	~Bullet();
	void update(float frameTime);
	void render();
	bool collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix);
	void destroy();
	//getter
	int	getDigitalPower();
	bool isCollideInitial();
	D3DXVECTOR3	getBulletSpeed();
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
	bool					isLaunched;		//発射したか
	float					launchFactTime;	//発射事実残存時間（発射した事実をゲーム中に残す時間.更新頻度の低いエネミーセンサにのせるため）

	PLAY_PARAMETERS shotSE;
	PLAY_PARAMETERS reroadSE;

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
	bool launch(Ray shootingRay);
	//リロード
	void reload();
	//弾削除
	void destroy(Bullet* bullet,int nodeNumber);

//[getter]
	int getRemaining();
	float getReloadTime();
	Bullet* getBullet(int i);
	int getNum();
	bool getIsLaunched();
};
