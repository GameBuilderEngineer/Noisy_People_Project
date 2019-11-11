//==================================
//ボタンUI処理[UIButtoon.cpp]
//染矢　晃介
//==================================

//==================================
//インクルード
//==================================
#include "UIButton.h"

//==================================
// using宣言
//==================================
using namespace uiButtonNS;

//==================================
//コンストラクタ
//==================================
UiButton::UiButton()
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		button[i] = new Sprite;
	}
}

//==================================
//デストラクタ
//==================================
UiButton::~UiButton()
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		delete button[i];
	}
}

//==================================
//初期化
//==================================
void UiButton::initialize()
{
	//初期表示位置の初期化
	BasicUI::assingPosition(POSITION_BUTTON);

	//初期サイズの初期化
	BasicUI::assingSize(WIDTH_BUTTON, HEIGHT_BUTTON);

	//初期α値の初期化
	BasicUI::assingColor(COLOR);

	//テクスチャの初期化
	BasicUI::initialize(button[SV], textureNS::reference(textureNS::UI_BUTTON1));
	BasicUI::initialize(button[SHOT], textureNS::reference(textureNS::UI_BUTTON2));
	BasicUI::initialize(button[JUMP], textureNS::reference(textureNS::UI_BUTTON3));
	BasicUI::initialize(button[VISION], textureNS::reference(textureNS::UI_BUTTON4));

}

//==================================
//P1描画
//==================================
void UiButton::renderP1()
{
	//1P用の表示
	for (int i = 0; i < TYPE_MAX; i++)
	{
		position.y = -HEIGHT_BUTTON * i + POSITION_BUTTON.y;
		button[i]->setPosition(position);
		button[i]->setVertex();
		button[i]->render();
	}
}

//==================================
//P2描画
//==================================
void UiButton::renderP2()
{
	//2P用の表示
	for (int i = 0; i < TYPE_MAX; i++)
	{
		position.x = POSITION_BUTTON2.x;
		position.y = -HEIGHT_BUTTON * i + POSITION_BUTTON.y;
		button[i]->setPosition(position);
		button[i]->setVertex();
		button[i]->render();
	}
}

//==================================
//更新
//引数:ボタンが使用可能かどうか
//	  :どこのボタンかを指定
//==================================
void UiButton::update(bool flag,int buttontype)
{
	changeAlpha(flag);
	button[buttontype]->setAlphaAllVertex(alphaValue);
	button[buttontype]->render();
}

//==================================
//α値の変更
//引数:ボタンが使用可能かどうか
//==================================
void  UiButton::changeAlpha(bool flag)
{
	if (flag == true)//使用可能の場合は可視化
	{
		BasicUI::increaseAlpha(ALPHA_MAX);
	}
	else//使用不可の場合は不可視
	{
		BasicUI::increaseAlpha(ALPHA_MINIMAM);
	}
}