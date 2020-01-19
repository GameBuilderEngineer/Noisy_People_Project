//==========================================
//電力タンク処理[electTank.cpp]
//染矢晃介
//==========================================

//==========================================
//インクルード
//==========================================
#include "Green.h"

//==========================================
// using宣言
//==========================================
using namespace greenNS;

//==========================================
//コンストラクタ
//==========================================
Green::Green()
{
		green = new Sprite;
}

//==========================================
//デストラクタ
//==========================================
Green::~Green()
{
		delete green;
}

//==========================================
//初期化
//==========================================
void Green::initialize(int playerType)
{
	BasicUI::assingSize(WIDTH_GREEN, HEIGHT_GREEN);
	switch (playerType)
	{
	case basicUiNS::P1:
			BasicUI::assingPosition(GREEN_POSTION);
			BasicUI::initialize(green, textureNS::reference(textureNS::UI_EN_TANK));
		break;
	case basicUiNS::P2:
		BasicUI::assingPosition(GREEN_POSTION_P2);
		BasicUI::initialize(green, textureNS::reference(textureNS::UI_EN_TANK));
		break;
	}
}

//==========================================
//描画処理
//==========================================
void Green::render(int state, float time)
{
	float gametime = time;
	if (gametime > 60)//残り時間が60以上の時のみ表示
	{
		switch (state)
		{
		case 2:
			green->render();
			break;
		case 3:
			green->render();
			break;
		}
	}
	
}
