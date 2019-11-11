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
#include "UIButton.h"
#include "ButtonFrame.h"
#include "StatusFrame.h"

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
	ButtonFrame *buttonFrameP1;//ボタンのボックス
	ButtonFrame *buttonFrameP2;//ボタンのボックス
	StatusFrame *statusFrameP1;//HPとかのフレーム
	StatusFrame *statusFrameP2;//HPとかのフレーム
	//bool	     buttonFlag;    //ボタンon off用フラグ
	//int		 buttonType;	//ボタンタイプ管理用
public://メンバー関数
	FixedUI();
	~FixedUI();
	void initialize();
	void render();
	void update();
	void uninitialize();
private:
};