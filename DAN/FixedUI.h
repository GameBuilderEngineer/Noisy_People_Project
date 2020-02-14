//=================================
// 固定されているUI[FixedUI.h]
// 染矢晃介
//=================================
#pragma once

//=================================
// インクルード
//=================================
#include "Timer.h"
#include "TimerFrame.h"
#include "StatusFrame.h"
#include "CharaIcon.h"

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
	StatusFrame *statusFrameP1;//HPとかのフレーム
	StatusFrame *statusFrameP2;//HPとかのフレーム
	
public://メンバー関数
	FixedUI();
	~FixedUI();
	void initialize();
	void render();
	void update(float gametime);
	void uninitialize();
	Timer* getTimer() { return timer; }
	TimerFlame* getTimerFlame() { return timerFlame; }
private:
};