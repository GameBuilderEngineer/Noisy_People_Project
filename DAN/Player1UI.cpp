//===============================
//プレイヤーUIの処理[PlayerUI.cpp]
//染矢晃介
//===============================

//================================
//インクルード
//================================
#include "Player1UI.h"

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
}

//==================================
//初期化
//==================================
void Player1UI::initialize(Player *player)
{
	this->player = player;
	buttonUiP1->initialize();
	buttonUiP2->initialize();
	hpGuage->initialize();
	charaIcon->initialize(basicUiNS::P1);
	hpGuageEmp->initialize(basicUiNS::P1);
	electGuageEmp->initialize(basicUiNS::P1);
}

//==================================
//描画
//==================================
void Player1UI::render()
{
	buttonUiP1->renderP1();
	buttonUiP2->renderP2();
	hpGuageEmp->render();
	hpGuage->render();
	charaIcon->render();
	electGuageEmp->render();
}

//==================================
//更新
//==================================
void Player1UI::update()
{
	//各フラグの獲得
	skyVisionFlag = player->canDoSkyVision();
	shiftFlag = player->canShift();
	shotFlag = player->canShot();
	visionFlag = player->canDoVision();
	hp = player->getHp();

	//P1用
	buttonUiP1->update(skyVisionFlag, 3);
	buttonUiP1->update(shiftFlag, 2);
	buttonUiP1->update(shotFlag, 1);
	buttonUiP1->update(visionFlag, 0);

	hpGuage->update(hp);
}
