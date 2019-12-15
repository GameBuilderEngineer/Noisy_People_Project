//===================================================================================================================================
//【Game.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/11/05
//===================================================================================================================================
#pragma once

// チートプレゼン有効の場合はコメントアウトをはずす
//#define CHEAT_PRESENTATION

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

#include "TelopManager.h"
#include "Player1UI.h"
#include "Player2UI.h"
//#include "Text.h"


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
	StaticMeshRenderer*				maleRenderer;		//男プレイヤーレンダラー
	StaticMeshRenderer*				femaleRenderer;		//女プレイヤーレンダラー

	Object*							testField;			//フィールド
	StaticMeshRenderer*				testFieldRenderer;	//フィールドレンダラー

	Sky*							sky;				//スカイドーム
	Ocean*							ocean;				//海面

	StaticMeshRenderer*		MoveP;
	MOVEP*					MoveP1;

	//ビルボード
	//TestEffect*						testEffect;			//インスタンシングビルボードテスト
	TestPlane*						samplePlane;		//ディスプレイ用プレーンサンプル
	Advertisement*					ad;					//開発中広告

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

	//再生パラメータ
	PLAY_PARAMETERS playParameters[4];
	
public:
	Game();
	~Game();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	void render3D(Camera currentCamera);
	void renderUI();
	void test();
	//void tree4Reregister(Object* tmp);//オブジェクトの分木空間への再登録処理
	void tree8Reregister(Object* tmp);//オブジェクトの分木空間への再登録処理

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};


//===================================================================================================================================
//【デバッグ】
//===================================================================================================================================
#ifdef _DEBUG



#endif// _DEBUG
