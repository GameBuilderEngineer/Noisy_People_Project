//==================================
//ボタンフレームクラス処理[ButtonFlame.h]
//染矢　晃介
//==================================
#pragma once

//==================================
//インクルード
//==================================
#include "BasicUI.h"
#include "VirtualController.h"

//==================================
//名前空間
//==================================
namespace buttonFrameNS
{
	const int WIDTH_BUTTONFLAME = (185 * WINDOW_WIDTH / 1920);	//ボタンフレームの横幅
	const int HEIGHT_BUTTONFLAME = (255 * WINDOW_HEIGHT / 1080);	//ボタンフレームの縦幅
	const D3DXVECTOR3 POSITION_BUTTONFLAME01 = D3DXVECTOR3(100 * WINDOW_WIDTH / 1920, 870 * WINDOW_HEIGHT / 1080, 0);	//ボタンフレームの表示位置(仮)
	const D3DXVECTOR3 POSITION_BUTTONFLAME02 = D3DXVECTOR3(1800 * WINDOW_WIDTH / 1920, 870 * WINDOW_HEIGHT / 1080, 0);	//ボタンフレームの表示位置(仮)
}

//==================================
//クラス定義
//==================================
class ButtonFrame :public BasicUI
{
public://メンバー変数
	Sprite * buttonFrame;	//スプライト

public://メンバー関数
	ButtonFrame();			//コンストラクタ
	~ButtonFrame();			//デストラクタ
	void initialize(int type);		//初期化
	void render();			//描画

};
