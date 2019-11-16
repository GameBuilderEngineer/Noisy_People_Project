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

//==================================
//名前空間
//==================================
namespace playerUiNS
{

}

//==================================
//クラス定義
//==================================
class PlayerUI
{
public://メンバー変数
	ButtonUI * buttonUiP1;
	ButtonUI * buttonUiP2;
	HPguage  * hpGuage;
	CharaIcon * charaIcon;
	Player *player;			//P1の情報
	bool	skyVisionFlag;	//スカイビジョンフラグ
	bool	shotFlag;		//ショットフラグ
	bool	shiftFlag;		//シフトフラグ
	bool	visionFlag;		//ビィジョンフラグ
	int		hp;				//hp保存
private:
public://メンバー関数
	PlayerUI();
	~PlayerUI();
	void initialize(Player *player);
	void render();
	void update();
	
private:

};