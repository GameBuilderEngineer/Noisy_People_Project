//===========================================
//スタッツフレームクラス[StatusFrame.h]
//染矢晃介
//===========================================
#pragma once

//===========================================
//インクルード
//===========================================
#include "BasicUI.h"

//===========================================
//名前空間
//===========================================
namespace statusFrameNS
{
	const int WIDTH_STATUSFLAME = (672);	//スタッツフレームの横幅
	const int HEIGHT_STATUSFLAME = (143);	//スッタツフレームの縦幅
	const D3DXVECTOR3 POSITION_STATUSFLAME01 = D3DXVECTOR3(350, 100, 0);	//スッタツフレームの表示位置(仮)
	const D3DXVECTOR3 POSITION_STATUSFLAME02 = D3DXVECTOR3(1440, 30, 0);//スッタツフレームの表示位置(仮)
}

//===========================================
//クラス定義
//===========================================
class StatusFrame:public BasicUI
{
public://メンバー変数
	Sprite * statusFrame;

private:
public://メンバー関数
	StatusFrame();
	~StatusFrame();
	void initialize();
	void render();
	void update();
};