//===============================
//プレイヤーUIの処理[PlayerUI.cpp]
//染矢晃介
//===============================

//================================
//インクルード
//================================
#include "Player1UI.h"

//================================
//using宣言
//================================
using namespace playerUiNS;

//================================
//コンストラクタ
//================================
Player1UI::Player1UI()
{
	buttonUiP1 = new ButtonUI;
	buttonUiP2 = new ButtonUI;
	hpGuage = new HPguage;
	charaIcon = new CharaIcon;
	hpGuageEmp = new HPguageEmp;
	electGuageEmp = new ElectGuageEmp;
	electGuage = new ElectGuage;
	electTank = new ElectTank;
}

//=================================
//デストラクタ
//=================================
Player1UI::~Player1UI()
{
	delete buttonUiP1;
	delete buttonUiP2;
	delete hpGuage;
	delete charaIcon;
	delete hpGuageEmp;
	delete electGuageEmp;
	delete electGuage;
	delete electTank;
}

//==================================
//初期化
//==================================
void Player1UI::initialize(Player *player)
{
	this->player = player;
	buttonUiP1->initialize();
	hpGuage->initialize();
	charaIcon->initialize(basicUiNS::P1);
	hpGuageEmp->initialize(basicUiNS::P1);
	electGuageEmp->initialize(basicUiNS::P1);
	electGuage->initialize(basicUiNS::P1);
	electTank->initialize(basicUiNS::P1);
}

//==================================
//描画
//==================================
void Player1UI::render()
{
	state = player->getState();
	buttonUiP1->renderP1(state);
	hpGuageEmp->render();
	hpGuage->render();
	charaIcon->render();
	electGuageEmp->render();
	electGuage->render();
	electTank->render();
}

//==================================
//更新
//==================================
void Player1UI::update()
{
	//各フラグの獲得
	getFlag();

	//P1用
	buttonUiP1->update(shotFlag, 0);
	buttonUiP1->update(jumpFlag, 1);
	buttonUiP1->update(visionFlag, 2);
	buttonUiP1->update(skyVisionFlag, 3);
	buttonUiP1->update(shiftFlag, 4);

	hpGuage->update(hp);
	electGuage->update(power);
	electTank->update(power,basicUiNS::P1);
}

//==================================
//各フラグの獲得
//==================================
void Player1UI::getFlag()
{
	//各フラグの獲得
	skyVisionFlag = player->canDoSkyVision();
	shiftFlag = player->canShift();
	shotFlag = player->canShot();
	visionFlag = player->canDoVision();
	jumpFlag = player->canJump();
	hp = player->getHp();
	power = player->getPower();
	state = player->getState();
}
