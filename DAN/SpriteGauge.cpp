//-----------------------------------------------------------------------------
// クラス [.h]　実験用
// Author : 新里
// 作成開始日 : 2019/10/17  
//-----------------------------------------------------------------------------
#include "SpriteGauge.h"



//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace spriteGaugeNS;

SpriteGauge::SpriteGauge()
{
	gauge = new Sprite;
}


SpriteGauge::~SpriteGauge()
{
	SAFE_DELETE(gauge);
}

//=============================================================================
// 初期化処理
//=============================================================================
void SpriteGauge::initialize()
{

//	gauge->initialize( "Splash.png" ,
//		SpriteNS::CENTER, WIDTH, HEIGHT, POSITION,
//		ROTATION, COLOR);
	gauge->initialize(
		*textureNS::reference(textureNS::SAMPLE_TREE),
		SpriteNS::CENTER,
		WIDTH, 
		HEIGHT, 
		POSITION,
		ROTATION,
		COLOR
	);

}


//=============================================================================
// レンダー処理
//=============================================================================
void SpriteGauge::render()
{
	gauge->render();
}