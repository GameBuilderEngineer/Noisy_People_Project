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
	electGuageEmp = new ElectGuageEmp;
	electGuage = new ElectGuage;
	mark = new Mark;
	green = new Green;
	greenRate = new GreenRate;
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
	delete electGuageEmp;
	delete electGuage;
	delete mark;
	delete green;
	delete greenRate;
}

//==================================
//初期化
//==================================
void Player2UI::initialize(Player *player)
{
	this->player = player;
	buttonUi->initialize();
	hpGuage->initialize();
	charaIcon->initialize(basicUiNS::P2);
	hpGuageEmp->initialize(basicUiNS::P2);
	electGuageEmp->initialize(basicUiNS::P2);
	electGuage->initialize(basicUiNS::P2);
	mark->initialize(basicUiNS::P2);
	green->initialize(basicUiNS::P2);
	greenRate->initialize(basicUiNS::P2);
}

//==================================
//描画
//==================================
void Player2UI::render()
{
	state = player->getState();
	buttonUi->renderP2(state);
	hpGuageEmp->render();
	hpGuage->render();
	electGuageEmp->render();
	electGuage->render();
	charaIcon->render();
	mark->render();
	green->render(state);
	greenRate->render(basicUiNS::P2,state);
}

//==================================
//更新
//==================================
void Player2UI::update(float neRate)
{

	//各フラグの獲得
	getFlag();

	//P2用
	buttonUi->update(shotFlag, 0);
	buttonUi->update(jumpFlag, 1);
	buttonUi->update(visionFlag, 2);
	buttonUi->update(skyVisionFlag, 3);
	buttonUi->update(shiftFlag, 4);

	//HPゲージ処理
	hpGuage->update(hp);

	//電力ゲージ処理
	electGuage->update(power);

	//緑化率
	greenRate->update(neRate);
}

//==================================
//各フラグの獲得
//==================================
void Player2UI::getFlag()
{
	//各フラグの獲得
	skyVisionFlag = player->canDoSkyVision();
	shiftFlag = player->canShift();
	shotFlag = player->canShot();
	visionFlag = player->canDoVision();
	jumpFlag = player->canJump();
	hp = player->getHp();
	power = player->getPower();
}