//===================================================================================================================================
//yTutorialUI.hz
// [์ฌา]HALGP12A332 16 ไ๏ F
// [์ฌ๚]2019/11/08
// [XV๚]2019/11/08
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//yCN[hz
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"
#include "GameMaster.h"
#include "TextureLoader.h"

//===================================================================================================================================
//y}N่`z
//===================================================================================================================================
#define ARRAY_TEX_DIVIDE_X	(4.0f)
#define ARRAY_TEX_DIVIDE_Y	(2.0f)

//===================================================================================================================================
//yผO๓ิz
//===================================================================================================================================
namespace tutorialUINS
{
	//่
	const int BG_WIDTH = 415;									//ก
	const int BG_HEIGHT = 81;									//c
	const int ARRAY_WIDTH = 42;									//ก
	const int ARRAY_HIGHT = 62;									//c

	const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//๑]
	const D3DCOLOR col = D3DCOLOR_RGBA(255, 255, 255, 255);		//F
	const D3DXVECTOR3 arrayStPos = D3DXVECTOR3(140.0f, 30.0f, 0.0f);
	const D3DXVECTOR3 arrayInterval = D3DXVECTOR3(51.0f, 0.0f, 0.0f);

	enum TUTORIAL_STEP
	{
		TUTORIAL_STEP_1,
		TUTORIAL_STEP_2,
		TUTORIAL_STEP_3,
		TUTORIAL_STEP_4,
		TUTORIAL_STEP_5,
		TUTORIAL_STEP_END
	};
}

//===================================================================================================================================
//y`[gAUINXz
//===================================================================================================================================
class TutorialUI
{
public:
	TutorialUI();
	~TutorialUI();

	void setStep(int playerID, int inStep);
	int  getStep(int playerID) { return step[playerID]; }
	void render(void);

private:
	int step[gameMasterNS::PLAYER_NUM];


	Sprite **tutorialSpriteBG;
	Sprite ***tutorialSpriteArray;

	int cnt;

	D3DXVECTOR3 arrayPos[gameMasterNS::PLAYER_NUM][tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_END];
};