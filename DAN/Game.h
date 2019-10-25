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
#include "EnemyManager.h"
#include "AIDirector.h"
#include "NavigationMesh.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Telop.h"
#include "Sky.h"

#include "SpriteGauge.h"
#include "Battery.h"

#include "Sound.h"
#include "SoundBase.h"
//#include "GameMaster.h"
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
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0,5.0f,0);

}

//===================================================================================================================================
//【ゲームシーンクラス】
//===================================================================================================================================
class Game : public AbstractScene
{
private:

	Player*					player;				//プレイヤー
	StaticMeshRenderer*		maleRenderer;		//男プレイヤーレンダラー
	StaticMeshRenderer*		femaleRenderer;		//女プレイヤーレンダラー
	Object*					testField;			//フィールド
	StaticMeshRenderer*		testFieldRenderer;	//フィールドレンダラー

	DeadTree*				deadTree;			//枯木
	TreeTypeA*				treeA;				//木Ａ
	TreeTypeB*				treeB;				//木B
	Stone*					stone;				//石
	Sky*					sky;				//スカイドーム

	TestEffect*				testEffect;			//インスタンシングビルボードテスト

	EnemyManager*			enemyManager;		// エネミーマネージャー
	Enemy*					enemy;				// エネミー
	TreeManager*			treeManager;		// ツリーマネージャー
	ItemManager*			itemManager;		// アイテムマネージャー

	Telop*					telop;				// テロップ

	AIDirector*				aiDirector;			// AI
	NavigationMesh*			naviAI;				// naviAI

	//再生パラメータ
	PLAY_PARAMETERS playParameters[4];

	//Sprite実験
	SpriteGauge* spriteGauge;

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

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};


//===================================================================================================================================
//【デバッグ】
//===================================================================================================================================
#ifdef _DEBUG

#if 1	// ここを1でナビメッシュデバッグモード
#define DEBUG_NAVIMESH
#endif


#endif// _DEBUG
