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
#include "ImguiManager.h"

namespace bulletNS{
	const float		SPEED			= 100.0f;	//弾速
	const float		INTERVAL_TIME	= 0.25f;	//インターバル時間
	const float		RELOAD_TIME		= 0.5f;		//リロード時間
	const int		MAGAZINE_NUM	= 8;		//弾数
	const float		EXIST_TIME		= 30.0f;	//存在時間
	const int		DIGITAL_POWER	= 20;		//デジタルパワー
	const float		LAUNCH_FACT_TIME = 0.333f;	//発射事実残存時間
	const float		LOST_DISTANCE	= 400.0f;

	//銃口エフェクト
	class Muzzle :public effekseerNS::Instance
	{
	public:
		D3DXMATRIX* syncMatrix;
		Muzzle(D3DXMATRIX* syncMatrix) {
			this->syncMatrix = syncMatrix;
			effectNo = effekseerNS::MUZZLE;
			scale = D3DXVECTOR3(0.1f,0.1f,0.1f);
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
			manager->SetScale(handle, scale.x, scale.y, scale.z);
		};
	};

	//弾本体エフェクト
	class BulletBody :public effekseerNS::Instance
	{
	public:
		D3DXMATRIX* syncMatrix;
		BulletBody(D3DXMATRIX* syncMatrix) {
			this->syncMatrix = syncMatrix;
			effectNo = effekseerNS::DAC_BULLET;
			scale = D3DXVECTOR3(1.0f,1.0f,1.0f);
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

	class BulletBody2 :public effekseerNS::Instance
	{
	public:
		D3DXMATRIX* syncMatrix;
		BulletBody2(D3DXMATRIX* syncMatrix) {
			this->syncMatrix = syncMatrix;
			effectNo = effekseerNS::POW_BULLET;
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
	effekseerNS::Instance* effect;			//弾エフェクト

public:
	int playerNo;
public:
//[基本処理]
	Bullet(Ray shootingRay,int playerNo, bool isPowUp);
	~Bullet();
	void update(float frameTime);
	void render();
	bool collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix);
	void destroy();
	//getter
	int	getDigitalPower();
	bool isCollideInitial();
	D3DXVECTOR3	getBulletSpeed();
	//setter
	void setDigitalPower(float value);


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
	float                   powerRate;
	bool					isPowerUp;		// アイテムを取りパワーアップしている
	PLAY_PARAMETERS shotSE;
	PLAY_PARAMETERS reroadSE;
	std::string		currentScene;			//現在のシーン

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
	bool launch(Ray shootingRay,int playerNo);
	//リロード
	void reload(int playerNo);
	//弾削除
	void destroy(Bullet* bullet,int nodeNumber);

//[getter]
	int getRemaining();
	float getReloadTime();
	Bullet* getBullet(int i);
	int getNum();
	bool getIsLaunched();
	float getPowerRate();
//[setter]
	void setPowerRate(float value);
	void setCurrentScene(std::string _scene);

#ifdef _DEBUG
	//void bulletGUI();
#endif
};
