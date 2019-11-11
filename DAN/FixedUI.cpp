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
	buttonP1 = new UiButton;
	buttonP2 = new UiButton;
	buttonFrameP1 = new ButtonFrame;
	buttonFrameP2 = new ButtonFrame;
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
	delete buttonP1;
	delete buttonP2;
	delete buttonFrameP1;
	delete buttonFrameP2;
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
	buttonP1->initialize();
	buttonP2->initialize();
	buttonFrameP1->initialize(basicUiNS::P1);
	buttonFrameP2->initialize(basicUiNS::P2);
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
	buttonFrameP1->render();
	buttonFrameP2->render();
	buttonP1->renderP1();
	buttonP2->renderP2();
	statusFrameP1->render();
	statusFrameP2->render();
}

//================================
// 更新
//================================
void FixedUI::update()
{
	//buttonFlag = Getflag()//こんな関数が用意される？
	timer->update();
	//マジックナンバー部分は変数を用意予定
	buttonP1->update(0,1);
	buttonP2->update(1,1);
}

//================================
// 終了
//================================
void FixedUI::uninitialize()
{
	
}

