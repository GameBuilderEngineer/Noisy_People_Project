//=====================================
//スタッツフレーム処理[StatusFrame.cpp]
//染矢晃介
//=====================================

//=====================================
//インクルード
//=====================================
#include "StatusFrame.h"

//=====================================
// using宣言
//=====================================
using namespace statusFrameNS;

//=====================================
//コンストラクタ
//=====================================
StatusFrame::StatusFrame()
{
	//スプライトの作成
	statusFrame = new Sprite;
}

//=====================================
//デストラクタ
//=====================================
StatusFrame::~StatusFrame()
{
	delete statusFrame;
}

//=====================================
//初期化
//=====================================
void StatusFrame::initialize(int playerType)
{
	if (playerType == basicUiNS::P1)
	{
		//初期位置の代入
		BasicUI::assingPosition(POSITION_STATUSFLAME01);

		//初期サイズの代入
		BasicUI::assingSize(WIDTH_STATUSFLAME, HEIGHT_STATUSFLAME);

		//テクスチャの指定
		BasicUI::initialize(statusFrame, textureNS::reference(textureNS::UI_STATUSFRAME_P1));
	}
	else if (playerType == basicUiNS::P2)
	{
		//初期位置の代入
		BasicUI::assingPosition(POSITION_STATUSFLAME02);

		//初期サイズの代入
		BasicUI::assingSize(WIDTH_STATUSFLAME, HEIGHT_STATUSFLAME);

		//テクスチャの指定
		BasicUI::initialize(statusFrame, textureNS::reference(textureNS::UI_STATUSFRAME_P2));
	}
}

//=====================================
//描画
//=====================================
void StatusFrame::render()
{
	statusFrame->render();
}

//=====================================
//更新
//=====================================
void StatusFrame::update()
{

}