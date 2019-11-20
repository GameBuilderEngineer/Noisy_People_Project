//=====================================
//基本ゲージクラス[BasicGuage.h]
//染矢晃介
//=====================================
#pragma once

//=====================================
//インクルード
//=====================================
#include "HPguage.h"
#include "HPguageEmp.h"
#include "Player.h"
#include "ElectGuageeEmp.h"

//=====================================
//名前空間
//=====================================
namespace guageNS
{

}

//=====================================
//クラス定義
//=====================================
class Guage:public Player
{
public://メンバー変数
	ElectGuageEmp *electGuageEmp;
	
private:
public://メンバー関数
	Guage();
	~Guage();
	void initialize();
	void render();
	void update();

};