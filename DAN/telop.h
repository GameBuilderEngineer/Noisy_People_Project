//-----------------------------------------------------------------------------
// テロップクラス [Telop.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/17
//-----------------------------------------------------------------------------
#pragma once
#include "Telop.h"
#include "Sprite.h"
#include "TextureLoader.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace telopNS
{
	// テロップタイプ
	enum TYPE
	{
		TELOP_TYPE0,
		TELOP_TYPE1,
		TELOP_TYPE2,
		TELOP_TYPE3,
		TELOP_TYPE4,
		TELOP_TYPE5,
		TELOP_TYPE6,
		MAX_TELOP
	};

	const int WIDTH = 800;
	const int HEIGHT = 568;
	const D3DXVECTOR3 POSITION = D3DXVECTOR3((float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
//クラス定義
//=============================================================================
class Telop
{
private:
	Sprite *telop[telopNS::MAX_TELOP];

public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render();
};
