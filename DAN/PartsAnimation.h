//-----------------------------------------------------------------------------
// パーツアニメーション基底クラス[PartsAnimationManager.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/12
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"


//=============================================================================
//クラス定義
//=============================================================================
class PartsAnimation: public Base
{
private:
	float animationTime;	// アニメーション時間
	int keyFrame;			// キーフレーム
	int numKeyFrame;		// キーフレームの数

public:
	PartsAnimation();
	~PartsAnimation();
	void initialize();
	void update(float t);
};
