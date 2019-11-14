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
	buttonP1 = new ButtonUI;
	buttonP2 = new ButtonUI;
	buttonFrameP1 = new ButtonFrame;
	buttonFrameP2 = new ButtonFrame;
	statusFrameP1 = new StatusFrame;
	statusFrameP2 = new StatusFrame;
	guage = new Guage;
	charaIcon = new CharaIcon;
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
	delete guage;
	delete charaIcon;
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
	guage->initialize();
	charaIcon->initialize();
	
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
	guage->render();
	charaIcon->render();
}

//================================
// 更新
//================================
void FixedUI::update()
{
	timer->update();
	guage->update();
}

//================================
// 終了
//================================
void FixedUI::uninitialize()
{
	
}

