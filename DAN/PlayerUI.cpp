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
}

//=================================
//デストラクタ
//=================================
PlayerUI::~PlayerUI()
{
	delete buttonUiP1;
	delete buttonUiP2;
}

//==================================
//初期化
//==================================
void PlayerUI::initialize()
{
	buttonUiP1->initialize();
	buttonUiP2->initialize();
}

//==================================
//描画
//==================================
void PlayerUI::render()
{
	buttonUiP1->renderP1();
	buttonUiP2->renderP2();
}

//==================================
//更新
//==================================
void PlayerUI::update()
{
	//各フラグの獲得
	skyVisionFlag = Player::canDoSkyVision();
	shotFlag = Player::canShot();
	jumpFlag = Player::canJump();
	visionFlag = Player::canDoVision();

	//P1用
	buttonUiP1->update(skyVisionFlag, 3);
	buttonUiP1->update(shotFlag, 2);
	buttonUiP1->update(jumpFlag, 1);
	buttonUiP1->update(visionFlag, 0);

	//P2用
	buttonUiP2->update(skyVisionFlag, 3);
	buttonUiP2->update(shotFlag, 2);
	buttonUiP2->update(jumpFlag, 1);
	buttonUiP2->update(visionFlag, 0);
}