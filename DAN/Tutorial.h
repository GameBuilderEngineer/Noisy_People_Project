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
#include <time.h>
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
	const D3DXVECTOR3 PLAYER_P1_POSITION = D3DXVECTOR3(545, 20, -25);
	const D3DXVECTOR3 PLAYER_P2_POSITION = D3DXVECTOR3(-505, 20, -25);

	//エネミー初期位置
	const D3DXVECTOR3 ENEMY_POSTITION[2] = { D3DXVECTOR3(525, 0, -25),D3DXVECTOR3(-525, 0, -25) };

	// plane
	const D3DXVECTOR3 PLANE_POS_5_5[2] = { D3DXVECTOR3(525, 35, 25),D3DXVECTOR3(-525, 35, 25) };
	const D3DXVECTOR3 PLANE_POS_FIN[2] = { D3DXVECTOR3(525, 35, 25),D3DXVECTOR3(-525, 35, 25) };
	const D3DXVECTOR3 FIN_POS[2] = { D3DXVECTOR3(525, 25, 25),D3DXVECTOR3(-525, 25, 25) };

	//ツリー初期位置
	const D3DXVECTOR3 FIRST_TREE_POSTITION[2] = { D3DXVECTOR3(510, 0, -45),D3DXVECTOR3(-540, 0, -45) };
	const D3DXVECTOR3 MID_TREE_POSTITION[2] = { D3DXVECTOR3(530, 30, 10),D3DXVECTOR3(-520, 30, 10) };

	//カメラ相対位置
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(1.5f, 0.0f, 0.0f, 0.0f);

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

	//進捗
	float tutorialTimer;								// チュートリアル時間
	int planeStep[gameMasterNS::PLAYER_NUM];
	int step[gameMasterNS::PLAYER_NUM];
	int timeCnt[gameMasterNS::PLAYER_NUM];
	bool shift[gameMasterNS::PLAYER_NUM];
	bool play[gameMasterNS::PLAYER_NUM];

	//System
	int								nowRenderingWindow;	//現在の描画ウィンドウ識別子
	Linear8TreeManager<Object>*		linear8TreeManager;	//線形８分木管理クラス
	ObjectTree<Object>*				objectTreeArray;	//オブジェクトツリー
	DWORD							collisionNum;		//衝突判定回数
	CollisionList<Object>*			collisionList;		//衝突判定リスト

	//3DObject
	Player*							player;				//プレイヤー
	Object*							testField;			//フィールド
	StaticMeshRenderer*				testFieldRenderer;	//フィールドレンダラー
	Sky*							sky;				//スカイドーム
	StaticMeshRenderer*				MoveP;
	MOVEP*							MoveP1;
	EnemyManager*					enemyManager;		// エネミーマネージャー
	TreeManager*					treeManager;		// ツリーマネージャー
	NavigationMesh*					naviMesh;			// ナビゲーションメッシュ
	Ray 							ray[2];				// レイ
	bool							clear55flag[2];

	//UI
	TutorialUI *tutorialUI;
	Reticle*						reticle;			//レティクル
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
