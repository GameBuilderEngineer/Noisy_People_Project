//===============================
//プレイヤーUIの処理[PlayerUI.cpp]
//染矢晃介
//===============================

//================================
//インクルード
//================================
#include "PlayerUI.h"

//================================
//コンストラクタ
//================================
PlayerUI::PlayerUI()
{
	buttonUiP1 = new ButtonUI;
	buttonUiP2 = new ButtonUI;
	hpGuage = new HPguage;
	charaIcon = new CharaIcon;
}

//=================================
//デストラクタ
//=================================
PlayerUI::~PlayerUI()
{
	delete buttonUiP1;
	delete buttonUiP2;
	delete hpGuage;
	delete charaIcon;
}

//==================================
//初期化
//==================================
void PlayerUI::initialize(Player *player)
{
	this->player = player;
	buttonUiP1->initialize();
	buttonUiP2->initialize();
	hpGuage->initialize();
	charaIcon->initialize();
}

//==================================
//描画
//==================================
void PlayerUI::render()
{
	buttonUiP1->renderP1();
	buttonUiP2->renderP2();
	hpGuage->render();
	charaIcon->render();
}

//==================================
//更新
//==================================
void PlayerUI::update()
{
	//各フラグの獲得
	skyVisionFlag = player->canDoSkyVision();
	shotFlag = player->canShot();
	jumpFlag = player->canJump();
	visionFlag = player->canDoVision();
	hp = player->getHp();

	//P1用
	buttonUiP1->update(skyVisionFlag, 3);
	buttonUiP1->update(shotFlag, 2);
	buttonUiP1->update(jumpFlag, 1);
	buttonUiP1->update(visionFlag, 0);

	hpGuage->update(hp);
}
