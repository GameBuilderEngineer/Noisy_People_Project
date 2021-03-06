//-----------------------------------------------------------------------------
// テロップクラス [Telop.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/17
//-----------------------------------------------------------------------------
#include "Telop.h"
#include "UtilityFunction.h"
using namespace telopNS;


//=============================================================================
//【コンストラクタ】
//=============================================================================
Telop::Telop()
{
	//再生パラメーター
	openTelop = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Telop,false,NULL,false,NULL };
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Telop::~Telop()
{
}


//=============================================================================
// 初期化
//=============================================================================
void Telop::initialize(LPDIRECT3DTEXTURE9 _texture, int _pivot,
	int _width, int _height, D3DXVECTOR3 _position, D3DXVECTOR3 _rotation, D3DCOLOR color)
{

	input = getInput();
	telopFlag = false;
	state = END;
	announceTimer = 0.0f;
	telopTimer = 0.0f;
	displayTimer = 0.0f;
	

	sprite = new Sprite;
	sprite->initialize(_texture, _pivot,
		_width,  _height, _position, _rotation, color);
	
	
}


//=============================================================================
// 終了処理
//=============================================================================
void Telop::uninitialize()
{
	SAFE_DELETE(sprite);
}


//=============================================================================
// 更新処理
//=============================================================================
void Telop::update(float _frameTime)
{
	frameTime = _frameTime;				//フレームタイムの保存
	
	rate = 0.0f;
	switch (state)
	{
	case ANNOUNCE:
		announceTimer += _frameTime;
		Telop::telopAnnounce();
		break;

	case OPEN://オープン
		
		telopTimer += _frameTime;		//シーンタイムの更新

		//高さの更新
		if (telopFlag)
		{
			Telop::open();
		}

		if (barFlag)
		{
			Telop::openBar();
		}

		break;

	case DISPLAY:	

		//表示待機　
		displayTimer -= _frameTime;
		Telop::display();
		
		break;

	case CLOSE:		

		//クローズ
		if (telopFlag)
		{
			Telop::close();
		}
		if (barFlag)
		{
			Telop::closeBar();
		}
		break;

	case END:
		//オープンへ遷移
		if (telopFlag)
		{
			state = ANNOUNCE;
			telopTimer = 0.0f;
		}
		if (barFlag)
		{
			state = ANNOUNCE;
			telopTimer = 0.0f;
		}
	}

	//サイズの更新
	if (telopFlag)
	{
		sprite->setSize(WIDTH, heightValue);
	}
	if (barFlag)
	{
		sprite->setSize(WIDTH_BAR, heightValue);
	}
	sprite->setVertex();
}


//=============================================================================
// 描画処理
//=============================================================================
void Telop::render()
{
	if (telopTimer > 0)
	{
		sprite->render();
	}
	
}

//=============================================================================
// アナウンス処理
//=============================================================================
void Telop::telopAnnounce()
{
	rate = announceTimer / telopNS::ANNOUNCE_TIME;
	
	if (announceTimer > telopNS::ANNOUNCE_TIME)
	{
		SoundInterface::SE->playSound(&openTelop);
		announceTimer = 0.0f;
		state = OPEN;
	}

}

//=============================================================================
// オープン処理
//=============================================================================
void Telop::open()
{
	//高さの更新
	rate = telopTimer / telopNS::OPEN_TIME;
	heightValue = (int)((float)telopNS::MAX_HEIGHT * rate);
	
	if (telopTimer > telopNS::OPEN_TIME)
	{
		state = telopNS::DISPLAY;
		displayTimer = telopNS::DISPLAY_TIME;
		telopTimer = telopNS::CLOSE_TIME;
	}
}

void Telop::openBar()
{
	//高さの更新
	rate = telopTimer / telopNS::OPEN_TIME;
	heightValue = (int)((float)telopNS::HEIGHT_BAR * rate);
	if (telopTimer > telopNS::OPEN_TIME)
	{
		state = telopNS::DISPLAY;
		displayTimer = telopNS::DISPLAY_TIME;
		telopTimer = telopNS::CLOSE_TIME;
	}
}

//=============================================================================
// 待機処理
//=============================================================================
void Telop::display()
{
	if (displayTimer < 0)
	{
		state = CLOSE;
		displayTimer = 0.0f;
		closeTimer = 0.0f;
	}
}
//=============================================================================
// クローズ処理
//=============================================================================
void Telop::close()
{
	if (telopTimer > 0)
	{
		telopTimer -= frameTime;
		closeTimer += frameTime;
		//高さの更新
		rate = telopTimer / CLOSE_TIME;
		heightValue = (int)((float)MAX_HEIGHT * rate);
		if (closeTimer >= CLOSE_TIME)
		{
			state = END;
			telopFlag = false;
			*playFlag = false;
		}
	}
}

void Telop::closeBar()
{
	if (telopTimer > 0)
	{
		telopTimer -= frameTime;
		closeTimer += frameTime;
		//高さの更新
		rate = telopTimer / CLOSE_TIME;
		heightValue = (int)((float)HEIGHT_BAR * rate);
		if (closeTimer >= CLOSE_TIME)
		{
			state = END;
			barFlag = false;
			*playFlag = false;
		}
	}
}

//=============================================================================
// テロップ再生処理
//=============================================================================
void Telop::playTelop()
{
	telopFlag = true;
}

void Telop::playTelopBar()
{
	barFlag = true;
}

void Telop::setManagerFlag(bool* managerFlag)
{
	playFlag = managerFlag;
}

