//=================================
// 固定されているUI[FixedUI]
// 染矢晃介
//=================================
#pragma once
//=================================
// インクルード
//=================================
#include "Timer.h"
#include "TimerFlame.h"
#include "UIButton.h"

//=================================
// 名前空間
//=================================
namespace fixedUiNS
{
	
}

//=================================
//クラス定義
//=================================
class FixedUI
{
public://メンバー変数
private:
	Timer * timer;				//タイマー
	TimerFlame * timerFlame;	//タイマーフレーム
	UiButton   * buttonP1;		//ボタンUI
	UiButton   * buttonP2;		//ボタンUI
	bool	     buttonFlag;    //ボタンon off用フラグ
public://メンバー関数
	FixedUI();
	~FixedUI();
	void initialize();
	void render();
	void update();
	void uninitialize();
private:
};