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


	//テクスチャの初期化
	BasicUI::initialize(button[SV], textureNS::reference(textureNS::UI_BUTTON1));
	BasicUI::initialize(button[SHOT], textureNS::reference(textureNS::UI_BUTTON2));
	BasicUI::initialize(button[JUMP], textureNS::reference(textureNS::UI_BUTTON3));
	BasicUI::initialize(button[VISION], textureNS::reference(textureNS::UI_BUTTON4));
}

//==================================
//描画
//==================================
void UiButton::render()
{

}

//==================================
//更新
//==================================
void UiButton::update()
{

}