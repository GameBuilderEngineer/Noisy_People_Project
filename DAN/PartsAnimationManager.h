//-----------------------------------------------------------------------------
// パーツアニメーションマネージャ [PartsAnimationManager.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/12
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "PartsAnimation.h"

//=============================================================================
//クラス定義
//=============================================================================
class PartsAnimationManager: public Base
{
protected:
	int partsMax;
	DWORD flagState;
	Object* parent;
	Object** parts;

public:
	PartsAnimationManager(int _partsMax, Object* _parent, Object** _parts);
	~PartsAnimationManager();
	// 初期化
	void initialize();
	// 更新
	virtual void update(float frameTime) = 0;
	// パーツのワールドマトリクスを算出
	void culcPartsWorldMatrix();
	// アニメーション起動
	void activate(DWORD flag);
	// アニメーション停止
	void inactivate(DWORD flag);
};
