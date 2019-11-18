//==================================
//タイマーフレームクラス処理[TimerFlame.h]
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
namespace timerFlameNS
{
	const int WIDTH_TIMERFLAME = (1920 * WINDOW_WIDTH / 1920);	//タイマーフレームの横幅
	const int HEIGHT_TIMERFLAME = (169 * WINDOW_HEIGHT / 1080);	//タイマーフレームの縦幅
	const D3DXVECTOR3 POSITION_TIMERFLAME = D3DXVECTOR3(960 * WINDOW_WIDTH / 1920, 84 * WINDOW_HEIGHT / 1080, 0);	//タイマーフレームの表示位置(仮)
}

//==================================
//クラス定義
//==================================
class TimerFlame :public BasicUI
{
public://メンバー変数
	Sprite * timerFlame;	//スプライト

public://メンバー関数
	TimerFlame();			//コンストラクタ
	~TimerFlame();			//デストラクタ
	void initialize();		//初期化
	void render();			//描画

};