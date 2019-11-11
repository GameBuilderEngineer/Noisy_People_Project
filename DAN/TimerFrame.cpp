//==================================
//タイマーフレーム処理[TimerFlame.cpp]
//染矢　晃介
//==================================

//==================================
//インクルード
//==================================
#include "TimerFrame.h"

//==================================
// using宣言
//==================================
using namespace timerFlameNS;

//==================================
//コンストラクタ
//==================================
TimerFlame::TimerFlame()
{
	//スプライトの作成
	timerFlame = new Sprite;
}

//==================================
//デストラクタ
//==================================
TimerFlame::~TimerFlame()
{
	delete timerFlame;
}

//==================================
//初期化
//==================================
void TimerFlame::initialize()
{
	//初期表示位置の代入
	BasicUI::assingPosition(POSITION_TIMERFLAME);
	//初期サイズの代入
	BasicUI::assingSize(WIDTH_TIMERFLAME, HEIGHT_TIMERFLAME);
	//テクスチャの初期化
	BasicUI::initialize(timerFlame, textureNS::reference(textureNS::UI_TIMERFLAME));
}

//=================================
//描画
//=================================
void TimerFlame::render()
{
	timerFlame->render();
}
