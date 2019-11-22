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

}

//===================================================================================================================================
//【タイトルシーンクラス】
//===================================================================================================================================
class Title : public AbstractScene
{
private:
	TestEffect* testEffect;
	WaveBall *waveBall;

	//フィールド描画実験
	Object*					titleField;
	StaticMeshRenderer*		titleFieldRenderer;	

	TitleUI titleUI;										//タイトルUI
	int selectStateMemory;

	float degree = 1.0f / 10.0f;
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