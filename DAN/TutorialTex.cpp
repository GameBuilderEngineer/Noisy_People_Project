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
	tutorial3_1 = new Sprite;
	tutorial3_2 = new Sprite;
	tutorialStandby1 = new Sprite;
	tutorialStandby2 = new Sprite;

	nextPage = TUTORIAL_2D_SCENE01;//最初のチュートリアルページで初期化

	ZeroMemory(renderFlag, sizeof(bool) * 4);

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

	//3ページ目-1
	tutorial3_1->initialize(
		*textureNS::reference(textureNS::TUTORIAL3)//テクスチャ
		, SpriteNS::CENTER						//中心
		, WIDTH	/ 2								//横幅
		, HEIGHT								//縦幅
		, POSITION_1P								//表示位置
		, ROTATION								//回転
		, COLOR);								//色
	
	//3ページ目-2
	tutorial3_2->initialize(
		*textureNS::reference(textureNS::TUTORIAL3)//テクスチャ
		, SpriteNS::CENTER						//中心
		, WIDTH / 2								//横幅
		, HEIGHT								//縦幅
		, POSITION_2P							//表示位置
		, ROTATION								//回転
		, COLOR);								//色

	//スタンバイ1
	tutorialStandby1->initialize(
		*textureNS::reference(textureNS::TUTORIAL_STANDBY_1)//テクスチャ
		, SpriteNS::CENTER						//中心
		, WIDTH	/ 2								//横幅
		, HEIGHT								//縦幅
		, POSITION_1P							//表示位置
		, ROTATION								//回転
		, COLOR);								//色

	//スタンバイ2
	tutorialStandby2->initialize(
		*textureNS::reference(textureNS::TUTORIAL_STANDBY_2)//テクスチャ
		, SpriteNS::CENTER						//中心
		, WIDTH	/ 2								//横幅
		, HEIGHT								//縦幅
		, POSITION_2P							//表示位置
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
	// 半面テクスチャ
	if (renderFlag[0]) { tutorial3_1->render(); }
	if (renderFlag[1]) { tutorial3_2->render(); }
	if (renderFlag[2]) { tutorialStandby1->render(); }
	if (renderFlag[3]) { tutorialStandby2->render(); }
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

//============================
//型画面テクスチャの取得
//============================
Sprite* TutorialTex::getHalfSlide(int playerNo, int number)
{
	switch(playerNo)
	{
	case 0:
		if (number == 0) { return tutorial3_1; }
		if (number == 1) { return tutorialStandby1; }
		break;

	case 1:
		if (number == 0) { return tutorial3_2; }
		if (number == 1) { return tutorialStandby2; }
		break;
	}
}

void TutorialTex::setRender(int index, bool setting)
{
	renderFlag[index] = setting;
}