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
	for (int i = 0; i < TYPE_MAX; i++)
	{
		position.x = -HEIGHT_BUTTON * i + POSITION_BUTTON.x;
		button[i]->setPosition(position);
		button[i]->setVertex();
		button[i]->render();
	}
}

//==================================
//更新
//==================================
void UiButton::update()
{

}