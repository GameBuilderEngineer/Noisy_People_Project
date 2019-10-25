//-----------------------------------------------------------------------------
// テロップクラス [Telop.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/17
//-----------------------------------------------------------------------------
#include "Telop.h"
using namespace telopNS;


//=============================================================================
// 初期化
//=============================================================================
void Telop::initialize()
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i] = new Sprite;
	}

	telop[TELOP_TYPE0]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO),
		SpriteNS::CENTER,
		WIDTH,
		HEIGHT,
		POSITION,
		ROTATION,
		COLOR
	);
	//telop[TELOP_TYPE1]->initialize
	/*telop[TELOP_TYPE2]->initialize
	telop[TELOP_TYPE3]->initialize
	telop[TELOP_TYPE4]->initialize
	telop[TELOP_TYPE5]->initialize*/

}


//=============================================================================
// 終了処理
//=============================================================================
void Telop::uninitialize()
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		SAFE_DELETE(telop[i]);
	}
}


//=============================================================================
// 更新処理
//=============================================================================
void Telop::update(float frameTime)
{

}


//=============================================================================
// 描画処理
//=============================================================================
void Telop::render()
{
	telop[TELOP_TYPE0]->render();
}
