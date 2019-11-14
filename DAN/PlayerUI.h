//==================================
//プレイヤーのUIクラス[PlayerUI.h]
//染矢晃介
//==================================
#pragma once

//==================================
//インクルード
//==================================
#include "Player.h"
#include "ButtonUI.h"

//==================================
//名前空間
//==================================
namespace playerUiNS
{

}

//==================================
//クラス定義
//==================================
class PlayerUI :public Player
{
public://メンバー変数
	ButtonUI * buttonUiP1;
	ButtonUI * buttonUiP2;
	bool	skyVisionFlag;
	bool	shotFlag;
	bool	jumpFlag;
	bool	visionFlag;
private:
public://メンバー関数
	PlayerUI();
	~PlayerUI();
	void initialize();
	void render();
	void update();
	
private:

};