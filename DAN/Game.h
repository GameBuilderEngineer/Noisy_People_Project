//===================================================================================================================================
//【Game.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/10/18
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
#include "Telop.h"
#include "Sky.h"
#include "SpriteGauge.h"
#include "Battery.h"
#include "telop.h"
#include "Timer.h"

#include "Sound.h"
#include "SoundBase.h"
#include "LinearTreeCell.h"
#include "movep.h"
//#include "PointSprite.h"
//#include "Object.h"
//#include "Text.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace gameNS
{
	//プレイヤー初期位置
	const D3DXVECTOR3 PLAYER_POSITION =	D3DXVECTOR3(0,100,0);

	//カメラ相対位置
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(0.0f,5.0f,5.0f,0.0f);

	//カメラ相対注視位置
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0.0,2.0f,0);
}

//===================================================================================================================================
//【ゲームシーンクラス】
//===================================================================================================================================
class Game : public AbstractScene
{
private:

	//Linear4TreeManager<Object>*	linear4TreeManager;	//線形４分木管理クラス
	Linear8TreeManager<Object>*		linear8TreeManager;	//線形８分木管理クラス
	ObjectTree<Object>*				objectTreeArray;	//オブジェクトツリー
	DWORD							collisionNum;		//衝突判定回数
	CollisionList<Object>*			collisionList;		//衝突判定リスト

	Player*							player;				//プレイヤー
	StaticMeshRenderer*				maleRenderer;		//男プレイヤーレンダラー
	StaticMeshRenderer*				femaleRenderer;		//女プレイヤーレンダラー
	Object*							testField;			//フィールド
	StaticMeshRenderer*				testFieldRenderer;	//フィールドレンダラー

	DeadTree*						deadTree;			//枯木
	TreeTypeA*						treeA;				//木Ａ
	TreeTypeB*						treeB;				//木B
	Stone*							stone;				//石
	Sky*							sky;				//スカイドーム

	StaticMeshRenderer*		MoveP;
	MOVEP*					MoveP1;

	TestEffect*						testEffect;			//インスタンシングビルボードテスト
	TestPlane*						samplePlane;		//ディスプレイ用プレーンサンプル

	EnemyManager*					enemyManager;		// エネミーマネージャー
	TreeManager*					treeManager;		// ツリーマネージャー
	ItemManager*					itemManager;		// アイテムマネージャー

	AIDirector*						aiDirector;			// AI
	NavigationMesh*					naviMesh;			// ナビゲーションメッシュ
	Telop*							telop;				// テロップ

	//再生パラメータ
	PLAY_PARAMETERS playParameters[4];

	//Sprite実験
	SpriteGauge*					spriteGauge;
	//タイマー
	Timer *timer;
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
