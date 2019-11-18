//======================================
//P2HPゲージクラス[HPguageP2.h]
//染矢晃介
//======================================
#pragma once

//======================================
//インクルード
//======================================
#include "BasicUI.h"
#include "VirtualController.h"

//======================================
//名前空間
//======================================
namespace hpGuageP2NS
{
	const int WIDTH_HP_GUAGE = (500 * WINDOW_WIDTH / 1920);	//タイマーフレームの横幅
	const int HEIGHT_HP_GUAGE = (30 * WINDOW_HEIGHT / 1080);	//タイマーフレームの縦幅
	const D3DXVECTOR3 POSITION_HP_GUAGE = D3DXVECTOR3(1818 * WINDOW_WIDTH / 1920, 35 * WINDOW_HEIGHT / 1080, 0);	//タイマーフレームの表示位置(仮)

}

//======================================
//クラス定義
//======================================
class HPguageP2 :public BasicUI
{
public://メンバー変数
	Sprite * hpGuage;
	int      hp;//HP保存用
	int		 width;
	//UV座標管理用
	D3DXVECTOR2 uvCoord1;
	D3DXVECTOR2 uvCoord2;
	D3DXVECTOR2 uvCoord3;
	D3DXVECTOR2 uvCoord4;

private:
public://メンバー関数
	HPguageP2();
	~HPguageP2();
	void initialize();
	void render();
	void update(int hp);
	void changeGage(int hp);
};
