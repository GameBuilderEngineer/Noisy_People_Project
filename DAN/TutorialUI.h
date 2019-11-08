//===================================================================================================================================
//ÅyTutorialUI.hÅz
// [çÏê¨é“]HALìåãûGP12A332 16 ‰Ô óFçÑ
// [çÏê¨ì˙]2019/11/08
// [çXêVì˙]2019/11/08
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//ÅyÉCÉìÉNÉãÅ[ÉhÅz
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"
#include "GameMaster.h"
#include "TextureLoader.h"

//===================================================================================================================================
//ÅyñºëOãÛä‘Åz
//===================================================================================================================================
namespace tutorialUINS
{
	//íËêî
	const int BG_WIDTH = 415;									//â°ïù
	const int BG_HEIGHT = 81;									//ècïù
	const int ARRAY_WIDTH = 42;									//â°ïù
	const int ARRAY_HIGHT = 62;									//ècïù

	const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//âÒì]
	const D3DCOLOR col = D3DCOLOR_RGBA(255, 255, 255, 255);		//êF
	const D3DXVECTOR3 arrayStPos = D3DXVECTOR3((BG_WIDTH / 2) + 140.0f, 30.0f, 0.0f);
	const D3DXVECTOR3 arrayInterval = D3DXVECTOR3(51.0f, 0.0f, 0.0f);

	enum TUTORIAL_STEP
	{
		TUTORIAL_STEP_1,
		TUTORIAL_STEP_2,
		TUTORIAL_STEP_3,
		TUTORIAL_STEP_4,
		TUTORIAL_STEP_5,
		TUTORIAL_STEP_MAX
	};
}

//===================================================================================================================================
//ÅyÉ`ÉÖÅ[ÉgÉäÉAÉãUIÉNÉâÉXÅz
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
	Sprite **tutorialSpriteArray;

	D3DXVECTOR3 arrayPos[gameMasterNS::PLAYER_NUM][tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_MAX];
};