//-----------------------------------------------------------------------------
// パーツアニメーション基底クラス[PartsAnimation.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/12
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"
#include "Object.h"


//=============================================================================
//クラス定義
//=============================================================================
class PartsAnimation: public Base
{
public:
	float animationTime;	// アニメーション時間
	int keyFrame;			// キーフレーム
	int numKeyFrame;		// キーフレームの数
	DWORD flag;				// 自身のアニメーションフラグ
	bool wasPlayedToEnd;	// 最後まで再生された

	// コンストラクタ
	PartsAnimation(DWORD _flag);
	// デストラクタ
	~PartsAnimation();
	// 初期化
	void initialize();
	// 更新
	virtual void update(D3DXVECTOR3* rot, Object** parts, float t) = 0;
	// アニメーション時間とキーフレームの更新
	void updateTimeAndKeyFrame(float t);
	// 回転の更新
	D3DXVECTOR3 updateRotation(const D3DXVECTOR3* rotation);
	// 現在のキーフレームを取得
	int getKeyFrame();
	// キーフレームの数を取得
	int getNumKeyFrame();
};
