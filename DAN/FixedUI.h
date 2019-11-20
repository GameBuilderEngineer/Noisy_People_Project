//=================================
// 固定されているUI[FixedUI]
// 染矢晃介
//=================================
#pragma once

//=================================
// インクルード
//=================================
#include "Timer.h"
#include "TimerFrame.h"
#include "ButtonUI.h"
#include "ButtonFrame.h"
#include "StatusFrame.h"
#include "Guage.h"
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
	ButtonUI   * buttonP2;		//ボタンUI
	ButtonFrame *buttonFrameP1;//ボタンのボックス
	ButtonFrame *buttonFrameP2;//ボタンのボックス
	StatusFrame *statusFrameP1;//HPとかのフレーム
	StatusFrame *statusFrameP2;//HPとかのフレーム
	Guage		*guage;		   //ゲージ
	
public://メンバー関数
	FixedUI();
	~FixedUI();
	void initialize();
	void render();
	void update(float gametime);
	void uninitialize();
private:
};