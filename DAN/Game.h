//===================================================================================================================================
//【Game.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/11/05
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include <vector>
#include "AbstractScene.h"
#include "Player.h"
#include "StaticMeshRenderer.h"
#include "Stone.h"
#include "DeadTree.h"
#include "TreeTypeA.h"
#include "TreeTypeB.h"
#include "TestEffect.h"
#include "TestPlane.h"
#include "EnemyManager.h"
#include "AIDirector.h"
#include "NavigationMesh.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Wind.h"
#include "MapObjectManager.h"
#include "Telop.h"
#include "Sky.h"
#include "SpriteGauge.h"
#include "Battery.h"
#include "Timer.h"
#include "Reticle.h"
#include "Ocean.h"
#include "FixedUI.h"
#include "Advertisement.h"
#include "Sound.h"
#include "SoundBase.h"
#include "LinearTreeCell.h"
#include "movep.h"
#include "movep1.h"
#include "Equipment.h"
#include "TelopManager.h"
#include "Player1UI.h"
#include "Player2UI.h"
//#include "Text.h"
#include "networkClient.h"
#include "CountUI.h"
#include "Announcement.h"
#include "Marker.h"
#include "DamageUI.h"
#include "Title.h"
#include "tmpOcean.h"
#include "PowerUp.h"
#if 1
#define CHEAT_PREZEN
#endif

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace gameNS
{
	//プレイヤー初期位置
	const D3DXVECTOR3 PLAYER_POSITION =	D3DXVECTOR3(0,100,0);

	//カメラ相対位置
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(0.0f,0.0f,-1.5f,0.0f);

	//カメラ相対注視位置
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0.0f,0.0f,0.0f);

	enum OPENING_CAMERA_LIST
	{
		CAMERA0,
		CAMERA1,
		CAMERA2,
		CAMERA3,
		CAMERA4,
		CAMERA5,
		CAMERA6,
		CAMERA7,
		CAMERA8,
		CAMERA9,
		MAX_CAMERA
	};
	
	enum ITEM_TYPE
	{
		BATTERY,
		EXAMPLE,
		POWER_UP,
		ITEM_TYPE_MAX
	};

}

//===================================================================================================================================
//【ゲームシーンクラス】
//===================================================================================================================================
class Game : public AbstractScene
{
private:
	int								nowRenderingWindow;	//現在の描画ウィンドウ識別子

	//衝突判定
	//Linear4TreeManager<Object>*	linear4TreeManager;	//線形４分木管理クラス
	Linear8TreeManager<Object>*		linear8TreeManager;	//線形８分木管理クラス
	ObjectTree<Object>*				objectTreeArray;	//オブジェクトツリー
	DWORD							collisionNum;		//衝突判定回数
	CollisionList<Object>*			collisionList;		//衝突判定リスト

	//3Dオブジェクト
	Player*							player;				//プレイヤー

	Object*							testField;			//フィールド
	StaticMeshRenderer*				testFieldRenderer;	//フィールドレンダラー
	Object*							faceField;			//装飾フィールド 
	StaticMeshRenderer*				faceFieldRenderer;	//装飾フィールドレンダラー

	Sky*							sky;				//スカイドーム
	Ocean*							ocean;				//海面
	//TmpOcean*						tmpOcean;			//仮海面

	StaticMeshRenderer*				MoveP;
	MOVEP*							MoveP1;

	//ビルボード
	//TestEffect*						testEffect;		//インスタンシングビルボードテスト
	TestPlane*						samplePlane;		//ディスプレイ用プレーンサンプル

	EnemyManager*					enemyManager;		// エネミーマネージャー
	TreeManager*					treeManager;		// ツリーマネージャー
	ItemManager*					itemManager;		// アイテムマネージャー
	WindManager*					windManager;		// 風マネージャー
	MapObjectManager*				mapObjectManager;	// マップオブジェクトマネージャー
	AIDirector*						aiDirector;			// AI
	NavigationMesh*					naviMesh;			// ナビゲーションメッシュ

	//UI
	SpriteGauge*					spriteGauge;		//Sprite実験
	Reticle*						reticle;			//レティクル
	TelopManager*					telopManager;		//テロップマネジャー
	FixedUI*						fixedUI;			//固定されたUI
	Player1UI*						player1UI;			//プレイヤー周りのUI
	Player2UI*						player2UI;			//プレイヤー２周りのUI
	CountUI*						countUI;			//カウントUI
	Announcement*					announcement;		//アナウンス
	MarkerRenderer*					markerRenderer;		//マーカー
	DamageUI*						damageUI;			//ダメージUI
	//再生パラメータ
	PLAY_PARAMETERS					playParameters[6];


	//Network
	NETWORK_CLIENT*					networkClient;
	
	
	//オープニングカメラ
	Camera* cameraOP;
	//GameOPカメラターゲットオブジェクト
	Object* target;

	//BOSSカメラ
	Camera* cameraBoss;

	int count;

	float moveTime;				//終点までの時間
	float moveTimer;			//移動タイマー
	float displayTime;
	float displayTimer;
	D3DXVECTOR3 startPos;		//ラープ始点

	//GameOPカメラ操作変数
	float rate;
	float rateY;
	float rateX;
	float degreeY;
	float degreeX;
	float degreeTimer;
	float degreeTime;
	float distance;
	float moveDistance;
	
	float degreeTimerX;
	float degreeTimeX;

	int stateCamera;
	float frameDegree = 1.0f / 5.0f;		//自動速度
	float inputDegree = 2.0f;				//入力速度

	//カメラの軸取得変数
	D3DXVECTOR3 cameraAxisX;			//カメラ回転軸X
	D3DXVECTOR3 cameraAxisY;			//カメラ回転軸Y
	D3DXVECTOR3 cameraAxisZ;			//カメラ回転軸Z
	D3DXVECTOR3 fixedAxisX;				//固定X軸
	D3DXVECTOR3 fixedAxisZ;				//固定Z軸
	D3DXQUATERNION tmpCameraQ;			//カメラの相対位置ベクトルの一時保存
	D3DXVECTOR3 BezierCurveS1;			//S字ベジェ曲線点１
	D3DXVECTOR3 BezierCurveS2;			//S字ベジェ曲線点２
	D3DXVECTOR3 targetDistance;
	D3DXVECTOR3 newRelative;

	D3DXVECTOR3 BezierPoint1;			//S字ベジェ曲線点１
	D3DXVECTOR3 BezierPoint2;			//S字ベジェ曲線点２
	D3DXVECTOR3 BezierPoint3;			//S字ベジェ曲線点２
	D3DXVECTOR3 BezierPoint4;			//S字ベジェ曲線点２
	D3DXVECTOR3 BezierPoint5;			//S字ベジェ曲線点２
	D3DXVECTOR3 BezierPoint6;			//S字ベジェ曲線点２

	D3DXVECTOR3 Curve1;					//S字ベジェ曲線点２
	D3DXVECTOR3 Curve2;					//S字ベジェ曲線点２
	D3DXVECTOR3 Curve3;					//S字ベジェ曲線点２
	D3DXVECTOR3 Curve4;					//S字ベジェ曲線点２
	D3DXVECTOR3 Curve5;					//S字ベジェ曲線点２
	bool measurement = true;

public:
	Game();
	~Game();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	void render3D(Camera* currentCamera);
	void renderUI();
	void test();
	//void tree4Reregister(Object* tmp);//オブジェクトの分木空間への再登録処理
	void tree8Reregister(Object* tmp);//オブジェクトの分木空間への再登録処理

	//更新処理
	void updateOP(); //OPの更新

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};

