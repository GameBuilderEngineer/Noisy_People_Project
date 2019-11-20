//===============================
//プレイヤーUIの処理[PlayerUI.cpp]
//染矢晃介
//===============================

//================================
//インクルード
//================================
#include "Player2UI.h"

//================================
//コンストラクタ
//================================
Player2UI::Player2UI()
{
	buttonUi = new ButtonUI;
	hpGuage = new HPguageP2;
	charaIcon = new CharaIcon;
	hpGuageEmp = new HPguageEmp;
}

//=================================
//デストラクタ
//=================================
Player2UI::~Player2UI()
{
	delete buttonUi;
	delete hpGuage;
	delete charaIcon;
	delete hpGuageEmp;
}

//==================================
//初期化
//==================================
void Player2UI::initialize(Player *player)
{
	this->player = player;
	buttonUi->initialize();
	hpGuage->initialize();
	charaIcon->initialize();
	hpGuageEmp->initialize(basicUiNS::P2);
}

//==================================
//描画
//==================================
void Player2UI::render()
{
	buttonUi->renderP1();
	hpGuageEmp->render();
	hpGuage->render();
	charaIcon->render();
}

//==================================
//更新
//==================================
void Player2UI::update()
{
	//各フラグの獲得
	skyVisionFlag = player->canDoSkyVision();
	shiftFlag = player->canShift();
	shotFlag = player->canShot();
	visionFlag = player->canDoVision();
	hp = player->getHp();

	//P2用
	buttonUi->update(skyVisionFlag, 3);
	buttonUi->update(shiftFlag, 2);
	buttonUi->update(shotFlag, 1);
	buttonUi->update(visionFlag, 0);

	hpGuage->update(hp);
}
