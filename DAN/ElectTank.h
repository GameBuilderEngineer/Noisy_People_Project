//=========================================
//電力タンククラス[ElectTank.h]
//染矢晃介
//=========================================
#pragma once

//=========================================
//インクルード
//=========================================
#include "BasicUI.h"

//=========================================
//名前空間
//=========================================
namespace electTankNS
{
	const int TANK_MAX = 2;
}

//=========================================
//クラス定義
//=========================================
class ElectTank : public BasicUI
{
public://メンバー変数
	Sprite * electTank[electTankNS::TANK_MAX];
private:
public://メンバー関数
	ElectTank();
	~ElectTank();
	void initialize();
	void render();
	void update();
private:
};