//-----------------------------------------------------------------------------
// テロップクラス [Telop.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/17
//-----------------------------------------------------------------------------
#include "Telop.h"
#include "UtilityFunction.h"
using namespace telopNS;


//=============================================================================
// 初期化
//=============================================================================
void Telop::initialize()
{
	int sinValue = 0;
	input = getInput();
	telopFlag = false;
	state = CLOSE;
	telopTimer = 0.0f;
	displayTimer = 0.0f;
	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i] = new Sprite;
	}
	
	//テロップ0の初期化
	telop[TELOP_TYPE0]->initialize(
		*textureNS::reference(textureNS::UI_INFO_10),
		SpriteNS::CENTER,
		WIDTH,
		MIN_HEIGHT,
		POSITION,
		ROTATION,
		COLOR
	);

	//テロップ1の初期化
	telop[TELOP_TYPE1]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO),
		SpriteNS::TOP_LEFT,
		WIDTH,
		MIN_HEIGHT,
		POSITION,
		ROTATION,
		COLOR
	);

	//テロップ2の初期化
	/*telop[TELOP_TYPE2]->initialize

	//テロップ3の初期化
	telop[TELOP_TYPE3]->initialize
	
	//テロップ4の初期化
	telop[TELOP_TYPE4]->initialize
	
	//テロップ5の初期化
	telop[TELOP_TYPE5]->initialize*/

	telop[TELOP_INFO_BAR]->initialize(
		*textureNS::reference(textureNS::UI_INFO_BAR),
		SpriteNS::CENTER,
		WIDTH_BAR,
		HEIGHT_BAR,
		POSITION,
		ROTATION,
		COLOR
	);

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
void Telop::update(float _frameTime)
{
	//frameTime = _frameTime;							//フレームタイムの保存
	//float rate = sceneTimer / SCENE_TIME;
	
	float rate = 0.0f;
	switch (state)
	{
	case OPEN://オープン
		telopTimer += _frameTime;						//シーンタイムの更新
		//高さの更新
		 rate = telopTimer / OPEN_TIME;
		heightValue = (int)((float)MAX_HEIGHT * rate);
		if (telopTimer > OPEN_TIME)
		{
			state = DISPLAY;
			displayTimer = DISPLAY_TIME;
			telopTimer = CLOSE_TIME;
		}
		break;
	case DISPLAY:	
		//表示待機
		displayTimer -= _frameTime;
		if (displayTimer < 0)
		{
			state = CLOSE;
			displayTimer = 0.0f;
		}
		break;
	case CLOSE:		
		//クローズ
		if (telopTimer > 0)
		{
			telopTimer -= _frameTime;
			//高さの更新
			rate = telopTimer / CLOSE_TIME;
			heightValue = (int)((float)MAX_HEIGHT * rate);
		}
		//オープンへ遷移
		if (input->wasKeyPressed('L'))
		{
			state = OPEN;
			telopTimer = 0.0f;
		}
		break;
		
	}

	/*if (telopFlag)
	{
		telopFlag = false;
		state = OPEN;
		telopTimer = 0.0f;
	}*/



	//サイズの更新
	telop[TELOP_TYPE0]->setSize(WIDTH, heightValue);
	telop[TELOP_TYPE0]->setVertex();
}


//=============================================================================
// 描画処理
//=============================================================================
void Telop::render()
{
	if (telopTimer>0)
	{
		telop[TELOP_INFO_BAR]->render();
		telop[TELOP_TYPE0]->render();
	}
	//telop[TELOP_TYPE1]->render();

}
