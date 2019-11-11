//==================================
//ボタンフレーム処理[ButtonFlame.cpp]
//染矢　晃介
//==================================

//==================================
//インクルード
//==================================
#include "ButtonFrame.h"

//==================================
// using宣言
//==================================
using namespace buttonFrameNS;

//==================================
//コンストラクタ
//==================================
ButtonFrame::ButtonFrame()
{
	//スプライトの作成
	buttonFrame = new Sprite;
}

//==================================
//デストラクタ
//==================================
ButtonFrame::~ButtonFrame()
{
	delete buttonFrame;
}

//==================================
//初期化
//引数:プレイヤータイプ
//==================================
void ButtonFrame::initialize(int type)
{
	//初期表示位置の代入
	if (type == basicUiNS::P1)
	{
		BasicUI::assingPosition(POSITION_BUTTONFLAME01);
	}
	else 
	{
		BasicUI::assingPosition(POSITION_BUTTONFLAME02);
	}
	//初期サイズの代入
	BasicUI::assingSize(WIDTH_BUTTONFLAME, HEIGHT_BUTTONFLAME);
	//テクスチャの初期化
	BasicUI::initialize(buttonFrame, textureNS::reference(textureNS::UI_BUTTON_BOX));
}

//=================================
//描画
//=================================
void ButtonFrame::render()
{
	buttonFrame->render();
}
