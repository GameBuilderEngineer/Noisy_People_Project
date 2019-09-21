//===================================================================================================================================
//【Game.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include <vector>
#include "AbstractScene.h"

//#include "GameMaster.h"
//#include "PointSprite.h"
//#include "InstancingBillboard.h"

//#include "Object.h"
//#include "Player.h"
//#include "Text.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace gameNS
{
	//プレイヤー数
	enum {
		PLAYER1,
		PLAYER2,
		NUM_PLAYER,
	};

	//プレイヤー初期位置
	const D3DXVECTOR3 PLAYER_POSITION[NUM_PLAYER] =
	{
		D3DXVECTOR3(0,100,0),
		D3DXVECTOR3(0,-100,0)
	};


	//カメラ相対位置
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION[NUM_PLAYER] =
	{
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f),
#ifdef _DEBUG
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f)
#else
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f)
#endif // _DEBUG
	};

	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0,10,0);

	//const int NUM_SAMPLE = 1000;

}

//===================================================================================================================================
//【ゲームシーンクラス】
//===================================================================================================================================
class Game : public AbstractScene
{
private:

	//Text text;					//Sample
	//Text text2;					//Sample
	//PointSprite pointSprite;		//Sample
	//InstancingBillboard plane;	//Sample
	//Object testObject;			//Sample
	//Object testCube;				//Sample

	//プレイヤー
	//Player *player[gameNS::NUM_PLAYER];

public:
	Game();
	~Game();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	//void render3D(Camera currentCamera);
	void renderUI();

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};