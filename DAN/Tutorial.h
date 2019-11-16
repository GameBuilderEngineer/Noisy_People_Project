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
#include "tutorialPlane.h"
#include "Sky.h"
#include "Timer.h"
#include "movep.h"
#include "TutorialUI.h"

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
	const D3DXVECTOR3 PLAYER_P1_POSITION = D3DXVECTOR3(-20, 100, -45);
	const D3DXVECTOR3 PLAYER_P2_POSITION = D3DXVECTOR3(20, 100, -45);

	//エネミー初期位置
	const D3DXVECTOR3 ENEMY_POSTITION = D3DXVECTOR3(0, 20, 0);

	//カメラ相対位置
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(0.0f, 5.0f, 5.0f, 0.0f);

	//カメラ相対注視位置
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0.0, 2.0f, 0);
}

//===================================================================================================================================
//【チュートリアルシーンクラス】
//===================================================================================================================================
class Tutorial : public AbstractScene
{
private:
	//チュートリアル2D
	TutorialTex tutorialTex;

	Linear8TreeManager<Object>*		linear8TreeManager;	//線形８分木管理クラス
	ObjectTree<Object>*				objectTreeArray;		//オブジェクトツリー
	DWORD							collisionNum;		//衝突判定回数
	CollisionList<Object>*			collisionList;		//衝突判定リスト

	Player*							player;				//プレイヤー
	StaticMeshRenderer*				maleRenderer;		//男プレイヤーレンダラー
	StaticMeshRenderer*				femaleRenderer;		//女プレイヤーレンダラー
	Object*							testField;			//フィールド
	StaticMeshRenderer*				testFieldRenderer;	//フィールドレンダラー

	Sky*								sky;					//スカイドーム

	StaticMeshRenderer*				MoveP;
	MOVEP*							MoveP1;

	EnemyManager*					enemyManager;		// エネミーマネージャー

	//UI
	TutorialUI *tutorialUI;

	//ディスプレイプレーン
	TutorialPlane **plane;

	//タイマー
	Timer *timer;

	//test(sai[del])
	enemyNS::EnemyData* enemyData;

	//進捗
	int planeStep[gameMasterNS::PLAYER_NUM];
	int step[gameMasterNS::PLAYER_NUM];

public:
	Tutorial();
	~Tutorial();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;
	void render3D(Camera currentCamera, int playerID);
	void renderUI();

	void tree8Reregister(Object* tmp);

#ifdef _DEBUG
	int playerSelect;
	virtual void createGUI() override;
#endif
};
