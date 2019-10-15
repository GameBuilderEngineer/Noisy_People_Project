//===================================================================================================================================
//【Title.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"
#include "InstancingBillboard.h"
#include "Sound.h"
#include "SoundBase.h"

//#include "SceneEffect.h"

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
	//Player player[titleNS::PLAYER_TYPE::PLAYER_TYPE_MAX];	//	プレイヤー
	//SceneEffect sceneEffect;								//	シーンエフェクト
	//UITitle uiTitle;										//	タイトルUI
	//InstancingBillboard plane;							//	インスタンシングビルボード
	InstancingBillboard plane;

	//再生パラメータ
	PLAY_PARAMETERS playParameters[3];

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

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};