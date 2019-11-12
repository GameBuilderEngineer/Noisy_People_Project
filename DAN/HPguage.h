//======================================
//HPゲージクラス[HPguage.h]
//染矢晃介
//======================================
#pragma once

//======================================
//インクルード
//======================================
#include "BasicUI.h"

//======================================
//名前空間
//======================================
namespace hpGuageNS
{
	const int WIDTH_HP_GUAGE = (500);	//タイマーフレームの横幅
	const int HEIGHT_HP_GUAGE = (30);	//タイマーフレームの縦幅
	const D3DXVECTOR3 POSITION_HP_GUAGE = D3DXVECTOR3(350, 50, 0);	//タイマーフレームの表示位置(仮)
}

//======================================
//クラス定義
//======================================
class HPguage:public BasicUI
{
public://メンバー変数
	Sprite * hpGuage;
private:
public://メンバー関数
	HPguage();
	~HPguage();
	void initialize();
	void render();
	void update();
};