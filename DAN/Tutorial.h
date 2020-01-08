//===================================================================================================================================
//【Tutorial.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"
#include "Player.h"
#include "TutorialTex.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "tutorialPlane.h"
#include "Sky.h"
#include "Timer.h"
#include "movep.h"
#include "movep1.h"
#include "TutorialUI.h"
#include "UtilityFunction.h"
#include "DamageUI.h"
#include "Player1UI.h"
#include "Player2UI.h"
#include "Reticle.h"
#include "FixedUI.h"



//===================================================================================================================================
//【マクロ定義】
//===================================================================================================================================
#define NEXT_MAX	(4)	// チュートリアルマックス値

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace tutorialNS
{
	// プレイヤー人数
	enum {
		PLAYER1,
		PLAYER2,
		NUM_PLAYER,
	};

	//プレイヤー初期位置
	const D3DXVECTOR3 PLAYER_P1_POSITION = D3DXVECTOR3(0, 20, -45);
	const D3DXVECTOR3 PLAYER_P2_POSITION = D3DXVECTOR3(-1000, 20, -45);

	//エネミー初期位置
	const D3DXVECTOR3 ENEMY_POSTITION = D3DXVECTOR3(0, 0, 0);

	//ツリー初期位置
	const D3DXVECTOR3 FIRST_TREE_POSTITION = D3DXVECTOR3(-20, 0, -50);

	//カメラ相対位置
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(0.0f, 0.0f, -1.5f, 0.0f);

	//カメラ相対注視位置
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===================================================================================================================================
//【チュートリアルシーンクラス】
//===================================================================================================================================
class Tutorial : public AbstractScene
{
private:

	//チュートリアル2D
	TutorialTex tutorialTex;

	//ディスプレイプレーン
	TutorialPlane **plane;

	//データ
	enemyNS::EnemyData* enemyData;

	//進捗
	float tutorialTimer;								// チュートリアル時間
	int planeStep[gameMasterNS::PLAYER_NUM];
	int step[gameMasterNS::PLAYER_NUM];


	//System
	int								nowRenderingWindow;	//現在の描画ウィンドウ識別子
	Linear8TreeManager<Object>*		linear8TreeManager;	//線形８分木管理クラス
	ObjectTree<Object>*				objectTreeArray;	//オブジェクトツリー
	DWORD							collisionNum;		//衝突判定回数
	CollisionList<Object>*			collisionList;		//衝突判定リスト

	//3DObject
	Player*							player;				//プレイヤー
	Object**						testField;			//フィールド
	StaticMeshRenderer*				testFieldRenderer;	//フィールドレンダラー
	Sky*							sky;				//スカイドーム
	StaticMeshRenderer*				MoveP;
	MOVEP*							MoveP1;
	EnemyManager*					enemyManager;		// エネミーマネージャー
	TreeManager*					treeManager;		// ツリーマネージャー
	NavigationMesh*					naviMesh;			// ナビゲーションメッシュ

	//UI
	TutorialUI *tutorialUI;
	Reticle*						reticle;			//レティクル
	FixedUI*						fixedUI;			//固定されたUI
	Player1UI*						player1UI;			//プレイヤー周りのUI
	Player2UI*						player2UI;			//プレイヤー２周りのUI
	MarkerRenderer*					markerRenderer;		//マーカー
	DamageUI*						damageUI;			//ダメージUI

public:
	Tutorial();
	~Tutorial();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;
	void render3D(Camera* currentCamera);
	void renderUI();

	void tree8Reregister(Object* tmp);

#ifdef _DEBUG
	int playerSelect;
	virtual void createGUI() override;
#endif
};
