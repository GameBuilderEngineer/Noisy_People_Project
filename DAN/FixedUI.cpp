//================================
// 固定されたUI[FixedUI.cpp]
// 染矢晃介
//================================

//================================
//インクルード
//================================
#include "FixedUI.h"

//================================
// using宣言
//================================
using namespace fixedUiNS;

//================================
// コンストラクタ
//================================
FixedUI::FixedUI()
{
	timer = new Timer;
	timerFlame = new TimerFlame;
	statusFrameP1 = new StatusFrame;
	statusFrameP2 = new StatusFrame;
}

//================================
// デストラクタ
//================================
FixedUI::~FixedUI()
{
	delete timer;
	delete timerFlame;
	delete statusFrameP1;
	delete statusFrameP2;
}

//================================
// 初期化
//================================
void FixedUI::initialize()
{
	timer->initialize();
	timerFlame->initialize();
	statusFrameP1->initialize(basicUiNS::P1);
	statusFrameP2->initialize(basicUiNS::P2);
}

//================================
// 描画
//================================
void FixedUI::render()
{
	timerFlame->render();
	timer->render();
	statusFrameP1->render();
	statusFrameP2->render();
}

//================================
// 更新
//================================
void FixedUI::update(float gametime)
{
	timer->update(gametime);
}

//================================
// 終了
//================================
void FixedUI::uninitialize()
{
	
}

