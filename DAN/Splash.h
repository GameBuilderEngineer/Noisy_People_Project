//===================================================================================================================================
//【Splash.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"
//#include "Splash2D.h"
#include "Sprite.h"
#include "SplashSprite.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace splashNS
{
	//スプラッシュスプライト情報の定義
	const int WIDTH = 800;
	const int HEIGHT = 568;
	const D3DXVECTOR3 POSITION = D3DXVECTOR3((float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 0);

	//シーン時間
	const float SCENE_TIME = 10.0f;
}

//===================================================================================================================================
//【スプラッシュシーンクラス】
//===================================================================================================================================
class Splash :public AbstractScene
{
private:
	// スプラッシュSprite
	SplashSprite* splashSprite;

public:
	Splash();
	~Splash();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;
	void renderUI();

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};

