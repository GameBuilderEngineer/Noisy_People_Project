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
#include "HPguage.h"
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
	enum STATE {
		NONE,
		NORMAL,
		VISION,
		SKY_VISION,
		DIGITAL_SHIFT,
		DEATH,
		STATE_NUM
	};
}

//==================================
//クラス定義
//==================================
class Player1UI:public GameMaster
{
public://メンバー変数
	ButtonUI * buttonUiP1;
	ButtonUI * buttonUiP2;
	HPguage  * hpGuage;
	HPguageEmp *hpGuageEmp;
	CharaIcon * charaIcon;
	ElectGuageEmp* electGuageEmp;
	ElectGuage* electGuage;
	Green* green;
	Mark* mark;
	GreenRate* greenRate;
	Player *player;			//P1の情報
	bool	skyVisionFlag;	//スカイビジョンフラグ
	bool	shotFlag;		//ショットフラグ
	bool	shiftFlag;		//シフトフラグ
	bool	visionFlag;		//ビィジョンフラグ
	bool	jumpFlag;		//ジャンプフラグ
	int		hp;				//hp保存
	int		power;			//電力保存
	int     state;			//プレイヤーの状態
private:
public://メンバー関数
	Player1UI();
	~Player1UI();
	void initialize(Player *player);
	void render();
	void update(float neRate);
	void getFlag();

private:

};