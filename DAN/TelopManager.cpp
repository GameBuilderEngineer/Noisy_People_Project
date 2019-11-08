//-----------------------------------------------------------------------------
// テロップマネージャークラス [TelopManager.cpp]
// Author : 新里　将士
// 作成開始日 : 2019/10/31
//-----------------------------------------------------------------------------
#include "TelopManager.h"

using namespace telopManagerNS;


TelopManager::TelopManager()
{
}


TelopManager::~TelopManager()
{
}

//=============================================================================
// 初期化
//=============================================================================
void TelopManager::initialize()
{

	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i] = new Telop;
	}

	//テロップバーの初期化
	telop[TELOP_INFO_BAR]->initialize(
		*textureNS::reference(textureNS::UI_INFO_BAR),
		SpriteNS::CENTER,
		telopNS::WIDTH_BAR,
		telopNS::HEIGHT_BAR,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ0の初期化
	telop[TELOP_TYPE0]->initialize(
		*textureNS::reference(textureNS::UI_INFO_10),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ1の初期化
	telop[TELOP_TYPE1]->initialize(
		*textureNS::reference(textureNS::UI_INFO_30),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ2の初期化
	telop[TELOP_TYPE2]->initialize(
		*textureNS::reference(textureNS::UI_INFO_50),
		SpriteNS::TOP_LEFT,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ3の初期化
	telop[TELOP_TYPE3]->initialize(
		*textureNS::reference(textureNS::UI_INFO_ASSULT),
		SpriteNS::TOP_LEFT,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ4の初期化
	telop[TELOP_TYPE4]->initialize(
		*textureNS::reference(textureNS::UI_INFO_LIMIT1),
		SpriteNS::TOP_LEFT,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ5の初期化
	telop[TELOP_TYPE5]->initialize(
		*textureNS::reference(textureNS::UI_INFO_LIMIT2),
		SpriteNS::TOP_LEFT,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ6の初期化
	telop[TELOP_TYPE6]->initialize(
		*textureNS::reference(textureNS::UI_INFO_MISSION),
		SpriteNS::TOP_LEFT,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);


}

//=============================================================================
// 終了処理
//=============================================================================
void TelopManager::uninitialize()
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		SAFE_DELETE(telop[i]);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void TelopManager::update(float _frameTime)
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i]->update(_frameTime);
	}
	
}

//=============================================================================
// 描画処理
//=============================================================================
void TelopManager::render()
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i]->render();
	}
}


//=============================================================================
// 再生処理
//=============================================================================
void TelopManager::play(int type)
{
	telop[type]->playTelop();
}