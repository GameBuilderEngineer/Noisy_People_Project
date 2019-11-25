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
	const int WIDTH_EN_GUAGE = (24 * WINDOW_WIDTH / 1920);	//電力タンクの横幅
	const int HEIGHT_EN_GUAGE = (24 * WINDOW_HEIGHT / 1080);//電力タンクの縦幅
	const D3DXVECTOR3 TANK_POSTION = D3DXVECTOR3(550.0f * WINDOW_WIDTH / 1920, 90.0f * WINDOW_HEIGHT / 1080, 0.0f);//タンクのポジション
	const D3DXVECTOR3 TANK_POSTION_P2 = D3DXVECTOR3(1340.0f * WINDOW_WIDTH / 1920, 90.0f * WINDOW_HEIGHT / 1080, 0.0f);//タンクのポジション
	const float ALPHA_MAX = 255.0f;
	const float ALPHA_MIN = 0.0f;
}

//=========================================
//クラス定義
//=========================================
class ElectTank : public BasicUI
{
public://メンバー変数
	Sprite * electTank[electTankNS::TANK_MAX];
	D3DXVECTOR3 postion;
private:
public://メンバー関数
	ElectTank();
	~ElectTank();
	void initialize(int playerType);
	void render();
	void update(int power);
	void renderRestriction(int power);//タンクの描画制限関数
private:
};