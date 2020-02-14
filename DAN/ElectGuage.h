//==================================
//電力ゲージクラス[ElectGuage.h]
//染矢晃介
//==================================
#pragma once

//========================================================================================================
//要素
//・タンクがある
//・ゲージがマックスになるとタンクが1増加
//========================================================================================================

//===================================
//インクルード
//===================================
#include "BasicUI.h"

//===================================
//名前空間
//===================================
namespace electGuageNS
{
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 0,255);	//電力ゲージの初期色
	const D3DCOLOR LED_COLOR = D3DCOLOR_RGBA(255, 0, 0, 255);//シフト不可時の色
	const int WIDTH_EN_GUAGE = (512 * WINDOW_WIDTH / 1920);	//電力ゲージの横幅
	const int HEIGHT_EN_GUAGE = (32 * WINDOW_HEIGHT / 1080);//電力ゲージの縦幅
	const D3DXVECTOR3 POSITION_EN_GUAGE = D3DXVECTOR3(112 * WINDOW_WIDTH / 1920, 75 * WINDOW_HEIGHT / 1080, 0);	//電力ゲージの表示位置(仮)
	const D3DXVECTOR3 POSITION_EN_GUAGE_P2 = D3DXVECTOR3(1805 * WINDOW_WIDTH / 1920, 75 * WINDOW_HEIGHT / 1080, 0);	//電力ゲージの表示位置(仮)
}

//===================================
//クラス定義
//===================================
class ElectGuage :public BasicUI
{
public://メンバー変数
	Sprite * electGuage;
	int width;
private:
public://メンバー関数
	ElectGuage();
	~ElectGuage();
	void initialize(int playerType);
	void render();
	void update(int power);
	void changeGuage(int power);//ゲージの変化処理
private:

};
	