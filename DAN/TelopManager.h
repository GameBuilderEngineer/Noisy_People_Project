//-----------------------------------------------------------------------------
// テロップマネージャークラス [TelopManager.h]
// Author : 新里　将士
// 作成開始日 : 2019/10/31
//-----------------------------------------------------------------------------
#pragma once
#include "Telop.h"
#include "Sprite.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace telopManagerNS
{
	// テロップタイプ
	enum TYPE
	{
		TELOP_INFO_BAR,
		TELOP_TYPE0,	// 緑化率10%
		TELOP_TYPE1,	// 緑化率30%
		TELOP_TYPE2,	// 緑化率50%
		TELOP_TYPE3,	// デジタルツリー襲撃
		TELOP_TYPE4,	// 活動限界～
		TELOP_TYPE5,	// ～感知できなくなってしまった
		TELOP_TYPE6,	// 制限時間以内に島の枯れ木を緑化せよ！
		MAX_TELOP
	};

}

class TelopManager
{
private:
	Telop* telop[telopManagerNS::MAX_TELOP];

	bool playFlag;

public:
	TelopManager();
	~TelopManager();

	void initialize();
	void uninitialize();
	void update(float _frameTime);
	void render();
	void play(int type);
};

