//===================================================================================================================================
//【Title.h】
// [作成者]HAL東京GP12A332 25 染矢 晃介
// [作成日]2019/09/20
// [更新日]2019/10/17
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"
#include "TestEffect.h"
#include "Sound.h"
#include "SoundBase.h"

//#include "SceneEffect.h"
#include "TitleUI.h"

#include "Object.h"
#include "StaticMeshRenderer.h"

#include "WaveBall.h"
#include "Sky.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace titleNS
{
	enum TRANSITION_SCENE_LIST
	{
		GAME,
		CREDIT,
		OPERATION,
		TUTORIAL,
		NUM_TRANSITION_SCENE
	};
	
	enum PHOTOGRAPH_LIST
	{
		CAMERA0,
		CAMERA1,
		CAMERA2,
		CAMERA3,
		MAX_CAMERA
	};

	const float TITLE_CAMERA_SPEED = 1.0f;

}

//===================================================================================================================================
//【タイトルシーンクラス】
//===================================================================================================================================
class Title : public AbstractScene
{
private:
	TestEffect* testEffect;
	WaveBall *waveBall;

	//フィールド
	Object*					titleField;
	StaticMeshRenderer*		titleFieldRenderer;	
	//スカイドーム
	Sky* sky;
	//ターゲットオブジェクト
	Object* target;

	TitleUI titleUI;										//タイトルUI
	int selectStateMemory;


	int stateCamera;
	float frameDegree = 1.0f / 10.0f;		//自動速度
	float inputDegree = 2.0f;				//入力速度
	
	float moveTime;				//終点までの時間
	float moveTimer;			//移動タイマー
	D3DXVECTOR3 startPos;		//ラープ始点
	
	//カメラの軸取得変数
	D3DXVECTOR3 cameraAxisZ;
	D3DXVECTOR3 fixedAxisZ;
	D3DXVECTOR3 cameraAxisX;
	D3DXVECTOR3 cameraAxisY;
	D3DXVECTOR3 fixedAxisX;
	

public:
	Title(void);
	~Title(void);
	virtual void initialize() override;
	virtual void uninitialize(void) override;
	virtual void update(float _frameTime) override;
	void updateInput(void);
	virtual void render() override;
	void render3D(Camera _currentCamera);
	void render2D();
	virtual void collisions(void) override;
	virtual void AI(void) override;

	float tmpVolume = 0;
#ifdef _DEBUG
	virtual void createGUI() override;

#endif
};