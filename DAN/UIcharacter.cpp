//============================
//[UIcharacter.cpp]
//染矢晃介
//============================

//============================
//インクルード
//============================
#include "UIcharacter.h"

//============================
//【using宣言】
//============================
using namespace uiCharacterNS;

//============================
//コンストラクタ
//============================
UIcharacter::UIcharacter()
{
	result = new Result_Chara;
	player = new Player_Chara;
	greenChara = new Greening_Chara;
	greenMoji = new Greening_Moji;
	defeatChara = new Defeat_Chara;
	defeatMoji = new Defeat_Moji;
	greening = new WholeGreening;
	persent = new Persent;
}

//============================
//デストラクタ
//============================
UIcharacter::~UIcharacter()
{
	delete result;
	delete player;
	delete greenChara;
	delete greenMoji;
	delete defeatChara;
	delete defeatMoji;
	delete greening;
	delete persent;
}

//============================
//初期化
//============================
void UIcharacter::initialize(int playerType)
{
	result->initialize();
	player->initialize(playerType);
	greenChara->initialize(playerType);
	greenMoji->initialize(playerType);
	defeatChara->initialize(playerType);
	defeatMoji->initialize(playerType);
	greening->initialize();
	persent->initialize();
}

//============================
//描画
//リザルトフェイズを参照し描画
//============================
void UIcharacter::render(int resultPhase)
{
	switch (resultPhase)
	{
	case PHASE_01:
		result->render();
		break;
	case PHASE_02:
		result->render();
		greening->render();
		persent->render();
		break;
	case PHASE_03:
		result->render();
		greening->render();
		persent->render();
		player->render();
		greenChara->render();
		greenMoji->render();
		defeatChara->render();
		defeatMoji->render();
		break;
	case PHASE_04:
		result->render();
		greening->render();
		persent->render();
		player->render();
		greenChara->render();
		greenMoji->render();
		defeatChara->render();
		defeatMoji->render();
		break;
	case PHASE_05:
		result->render();
		greening->render();
		persent->render();
		player->render();
		greenChara->render();
		greenMoji->render();
		defeatChara->render();
		defeatMoji->render();
		break;
	default:
		break;

	}
	
}

//============================
//更新
//ここ気持ち悪い絶対他にやりようあるけど実装優先
//============================
void UIcharacter::update(int resultPhase,int playerType,float flametime)
{
	switch (resultPhase)
	{
	case PHASE_01:
		result->update(flametime);
		break;
	case PHASE_02:
		greening->update();
		break;
	case PHASE_03:
		greening->update();
		player->update(playerType, flametime);
		greenChara->update(playerType, flametime);
		greenMoji->update(playerType, flametime);
		defeatChara->update(playerType, flametime);
		defeatMoji->update(playerType, flametime);
		break;
	case PHASE_04:
		greening->update();
		player->update(playerType, flametime);
		greenChara->update(playerType, flametime);
		greenMoji->update(playerType, flametime);
		defeatChara->update(playerType, flametime);
		defeatMoji->update(playerType, flametime);
		break;
	case PHASE_05:
		greening->update();
		player->update(playerType, flametime);
		greenChara->update(playerType, flametime);
		greenMoji->update(playerType, flametime);
		defeatChara->update(playerType, flametime);
		defeatMoji->update(playerType, flametime);
		break;
	default:
		break;
	}
}

