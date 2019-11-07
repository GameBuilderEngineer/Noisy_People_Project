//==================================
//タイマーフレーム処理[TimerFlame.cpp]
//染矢　晃介
//==================================

//==================================
//インクルード
//==================================
#include "TimerFlame.h"

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
	timerFlame->initialize(
		*textureNS::reference(textureNS::UI_TIMERFLAME)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_TIMERFLAME							//横幅
		, HEIGHT_TIMERFLAME							//縦幅
		, POSITION_TIMERFLAME						//表示位置
		, ROTATION_TIMERFLAME						//回転
		, COLOR_TIMERFLAME);						//色
}

//=================================
//描画
//=================================
void TimerFlame::render()
{
	timerFlame->render();
}