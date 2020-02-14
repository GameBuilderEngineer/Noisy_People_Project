//==================================
//プレイヤーのUIクラス[Player2UI.h]
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
#include "ElectGuageeEmp.h"
#include "ElectGuage.h"
#include "Green.h"
#include "Mark.h"
#include "GreenRate.h"

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
	ElectGuageEmp* electGuageEmp;
	ElectGuage*	electGuage;
	Green* green;
	Mark* mark;
	GreenRate* greenRate;
	Player *player;			//P2の情報
	bool	skyVisionFlag;	//スカイビジョンフラグ
	bool	shotFlag;		//ショットフラグ
	bool	shiftFlag;		//シフトフラグ
	bool	visionFlag;		//ビィジョンフラグ
	bool	jumpFlag;		//ジャンプフラグ
	int		hp;				//hp保存
	int     power;			//電力
	int		state;			//プレイヤーの状態

private:
public://メンバー関数
	Player2UI();
	~Player2UI();
	void initialize(Player *player);
	void render(float time);
	void update(float neRate);
	void getFlag();

private:

};