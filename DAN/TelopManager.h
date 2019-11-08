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
		TELOP_TYPE0,
		TELOP_TYPE1,
		TELOP_TYPE2,
		TELOP_TYPE3,
		TELOP_TYPE4,
		TELOP_TYPE5,
		TELOP_TYPE6,
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

