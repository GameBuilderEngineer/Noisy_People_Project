//==================================
//ボタンUI処理[UIButtoon.cpp]
//染矢　晃介
//==================================

//==================================
//インクルード
//==================================
#include "ButtonUI.h"

//==================================
// using宣言
//==================================
using namespace uiButtonNS;

//==================================
//コンストラクタ
//==================================
ButtonUI::ButtonUI()
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		button[i] = new Sprite;
	}
}

//==================================
//デストラクタ
//==================================
ButtonUI::~ButtonUI()
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		delete button[i];
	}
}

//==================================
//初期化
//==================================
void ButtonUI::initialize()
{
	//初期表示位置の初期化
	BasicUI::assingPosition(POSITION_BUTTON);

	//初期サイズの初期化
	BasicUI::assingSize(WIDTH_BUTTON, HEIGHT_BUTTON);

	//初期α値の初期化
	BasicUI::assingColor(COLOR);

	//テクスチャの初期化
	BasicUI::initialize(button[SHOT], textureNS::reference(textureNS::UI_BUTTON_SHOT));
	BasicUI::initialize(button[JUMP], textureNS::reference(textureNS::UI_BUTTON_JUMP));
	BasicUI::initialize(button[VS], textureNS::reference(textureNS::UI_BUTTON_VISION));
	BasicUI::initialize(button[SV], textureNS::reference(textureNS::UI_BUTTON_SV));
	BasicUI::initialize(button[SHIFT], textureNS::reference(textureNS::UI_BUTTON_SHIFT));

}

//==================================
//P1描画
//==================================
void ButtonUI::renderP1(int state)
{
	changeRenderP1(state);
}

//==================================
//P2描画
//==================================
void ButtonUI::renderP2(int state)
{
	changeRenderP2(state);
}

//==================================
//更新
//引数:ボタンが使用可能かどうか
//	  :どこのボタンかを指定
//==================================
void ButtonUI::update(bool flag,int buttontype)
{
	changeAlpha(flag,buttontype);
	/*if (buttontype == SHIFT)
	{
		changeRender(flag);
	}*/
}

//==================================
//α値の変更
//引数:ボタンが使用可能かどうか
//==================================
void  ButtonUI::changeAlpha(bool flag,int buttontype)
{
	if (flag == true)//使用可能の場合は可視化
	{
		BasicUI::changeAlpha(button[buttontype],ALPHA_MAX);
	}
	else//使用不可の場合は不可視
	{
		BasicUI::changeAlpha(button[buttontype],ALPHA_MINIMAM);
	}
}

//===================================
//表示ボタンの変更
//===================================
void ButtonUI::changeRenderP1(int state)
{
	switch (state)
	{
	case STATE::VISION:
		button[SHIFT]->render();
		break;

	case STATE::SKY_VISION:
		button[SHIFT]->render();
		break;

	default:
		for (int i = 0; i < SHIFT; i++)
		{
			position.y = HEIGHT_BUTTON * i + POSITION_BUTTON.y;
			button[i]->setPosition(position);
			button[i]->setVertex();
			button[i]->render();
		}
		break;
	}
}

//===================================
//表示ボタンの変更
//===================================
void ButtonUI::changeRenderP2(int state)
{
	switch (state)
	{
	case STATE::VISION:
		position.x = POSITION_BUTTON2.x;
		button[SHIFT]->setPosition(position);
		button[SHIFT]->setVertex();
		button[SHIFT]->render();
		break;

	case STATE::SKY_VISION:
		position.x = POSITION_BUTTON2.x;
		button[SHIFT]->setPosition(position);
		button[SHIFT]->setVertex();
		button[SHIFT]->render();
		break;

	default:
		for (int i = 0; i < SHIFT; i++)
		{
			position.x = POSITION_BUTTON2.x;
			position.y = HEIGHT_BUTTON * i + POSITION_BUTTON.y;
			button[i]->setPosition(position);
			button[i]->setVertex();
			button[i]->render();
		}
		break;
	}
}