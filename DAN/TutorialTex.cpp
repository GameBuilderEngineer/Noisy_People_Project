//============================
//[TutorialTex.cpp]
//染矢晃介
//============================

//============================
//インクルード
//============================
#include "TutorialTex.h"
#include "Input.h"

//============================
//【using宣言】
//============================
using namespace tutorialTex;

//============================
//グローバル変数
//============================

//============================
//初期化
//============================
void TutorialTex::initialize()
{
	tutorial1 = new Sprite;
	tutorial2 = new Sprite;

	nextPage = TUTORIAL_2D_SCENE01;//最初のチュートリアルページで初期化

	//1ページ目
	tutorial1->initialize(
		*textureNS::reference(textureNS::TUTORIAL1)//テクスチャ
		, SpriteNS::CENTER						//中心
		, WIDTH									//横幅
		, HEIGHT									//縦幅
		, POSITION								//表示位置
		, ROTATION								//回転
		, COLOR);								//色

	//2ページ目
	tutorial2->initialize(
		*textureNS::reference(textureNS::TUTORIAL2)//テクスチャ
		, SpriteNS::CENTER						//中心
		, WIDTH									//横幅
		, HEIGHT								//縦幅
		, POSITION								//表示位置
		, ROTATION								//回転
		, COLOR);								//色
}

//============================
//描画
//nextPageを参照し描画
//============================
void TutorialTex::render()
{
	switch (nextPage)
	{
	case TUTORIAL_2D_SCENE01:
		tutorial1->render();
		break;

	case TUTORIAL_2D_SCENE02:
		tutorial2->render();
		break;
	}
}

//============================
//更新
//============================
void TutorialTex::update()
{
	
}

//============================
//終了処理
//============================
void TutorialTex::uninitialize()
{
	delete  tutorial1;
	delete  tutorial2;
}
