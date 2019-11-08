//===================================================================================================================================
//【TutorialUI.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/11/08
// [更新日]2019/11/08
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "TutorialUI.h"
#include "player.h"

//===================================================================================================================================
// 【using宣言】
//===================================================================================================================================
using namespace tutorialUINS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
TutorialUI::TutorialUI()
{
	RECT recDisplay;
	HWND hDeskWnd;
	hDeskWnd = GetDesktopWindow();
	GetClientRect(hDeskWnd, &recDisplay);

	tutorialSpriteBG = new Sprite *[gameMasterNS::PLAYER_NUM];
	tutorialSpriteArray = new Sprite *[gameMasterNS::PLAYER_NUM];

	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//状態
		step[i] = TUTORIAL_STEP::TUTORIAL_STEP_2;

		//矢印の位置
		for (int j = 0; j < tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_MAX; j++)
		{
			arrayPos[i][j] =
				(arrayStPos + (D3DXVECTOR3(recDisplay.right / 2, 0.0f, 0.0f)*i))
				+ (arrayInterval * j);
		}

		tutorialSpriteBG[i] = new Sprite;
		tutorialSpriteArray[i] = new Sprite;

		tutorialSpriteBG[i]->initialize(
			*textureNS::reference(textureNS::TUTORIAL_UI_0),//テクスチャ
			SpriteNS::TOP_LEFT,		//中心
			BG_WIDTH,				//横幅
			BG_HEIGHT,				//縦幅
			D3DXVECTOR3((recDisplay.right / 2)*i, 0, 0), 	//表示位置
			rot,						//回転
			col);					//色

		tutorialSpriteArray[i]->initialize(
			*textureNS::reference(textureNS::TUTORIAL_UI_1),//テクスチャ
			SpriteNS::TOP_LEFT,		//中心
			ARRAY_WIDTH,				//横幅
			ARRAY_HIGHT,				//縦幅
			arrayPos[i][step[i]], 	//表示位置
			rot,						//回転
			col);					//色
	}
}

//===================================================================================================================================
//【進捗の設定】
//===================================================================================================================================
void TutorialUI::setStep(int playerID, int inStep)
{
	step[playerID] = inStep;
	tutorialSpriteArray[playerID]->setPosition(arrayPos[playerID][step[playerID]]);
	tutorialSpriteArray[playerID]->setVertex();
}

//===================================================================================================================================
//【2D描画】
//===================================================================================================================================
void TutorialUI::render(void)
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		tutorialSpriteBG[i]->render();
		tutorialSpriteArray[i]->render();
	}
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
TutorialUI::~TutorialUI()
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		SAFE_DELETE(tutorialSpriteBG[i]);
		SAFE_DELETE(tutorialSpriteArray[i]);
	}
	SAFE_DELETE_ARRAY(tutorialSpriteBG);
	SAFE_DELETE_ARRAY(tutorialSpriteArray);
}
