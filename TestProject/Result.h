//===================================================================================================================================
//【Result.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/23
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"
//#include "Sprite.h"
//#include "UIResult.h"
//#include "SceneEffect.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace resultNS
{
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION =	D3DXQUATERNION(0.0f, 20.0f, -40.0f, 0.0f);
}

//===================================================================================================================================
//【リザルトシーンクラス】
//===================================================================================================================================
class Result :public AbstractScene
{
private:
	//UIResult uiResult;			//	リザルトUI
	//TextManager* textManager;	//	テキストマネージャ
	//SceneEffect sceneEffect;//	シーンエフェクト

public:
	Result(void);
	~Result(void);
	virtual void initialize() override;
	virtual void uninitialize(void) override;
	virtual void update(float _frameTime) override;
	virtual void render() override;
	virtual void collisions(void) override;
	virtual void AI(void) override;

	//void render3D(Camera _currentCamera);
	void renderUI();

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};