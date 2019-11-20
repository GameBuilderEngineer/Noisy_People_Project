//==================================
//プレイヤーのUIクラス[PlayerUI.h]
//染矢晃介
//==================================
#pragma once

//==================================
//インクルード
//==================================
#include "ButtonUI.h"
#include "Player.h"
#include "HPguageP2.h"
#include "CharaIcon.h"
#include "HPguageEmp.h"

//==================================
//名前空間
//==================================
namespace playerUiNS
{

}

//==================================
//クラス定義
//==================================
class Player2UI
{
public://メンバー変数
	ButtonUI* buttonUi;
	HPguageP2* hpGuage;
	CharaIcon* charaIcon;
	HPguageEmp*  hpGuageEmp;
	Player *player;			//P1の情報
	bool	skyVisionFlag;	//スカイビジョンフラグ
	bool	shotFlag;		//ショットフラグ
	bool	shiftFlag;		//シフトフラグ
	bool	visionFlag;		//ビィジョンフラグ
	int		hp;				//hp保存
private:
public://メンバー関数
	Player2UI();
	~Player2UI();
	void initialize(Player *player);
	void render();
	void update();

private:

};