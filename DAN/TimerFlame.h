//==================================
//タイマーフレームクラス処理[TimerFlame.h]
//染矢　晃介
//==================================
#pragma once

//==================================
//インクルード
//==================================
#include "BasicUI.h"

//==================================
//名前空間
//==================================
namespace timerFlameNS
{
	const int WIDTH_TIMERFLAME = (1920);	//タイマーフレームの横幅
	const int HEIGHT_TIMERFLAME = (169);	//タイマーフレームの縦幅
	const D3DXVECTOR3 POSITION_TIMERFLAME = D3DXVECTOR3(960, 84, 0);	//タイマーフレームの表示位置(仮)
	const D3DXVECTOR3 ROTATION_TIMERFLAME = D3DXVECTOR3(0, 0, 0);		//回転
	const D3DCOLOR COLOR_TIMERFLAME = D3DCOLOR_RGBA(255, 255, 255, 255);//色
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