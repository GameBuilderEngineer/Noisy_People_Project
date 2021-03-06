//-----------------------------------------------------------------------------
// テロップクラス [Telop.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/17
//-----------------------------------------------------------------------------
#pragma once
#include "Telop.h"
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"
#include "UtilityFunction.h"
#include "Sound.h"
#include "SoundBase.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace telopNS
{
	
	enum STATE
	{
		ANNOUNCE,
		OPEN,			//テロップの状態
		DISPLAY,
		CLOSE,
		END,
		STATE_NUM,
	};

	const int WIDTH = 1500;
	const int MIN_HEIGHT = 0;
	const int MAX_HEIGHT = 120;
	const int WIDTH_BAR = 1920;
	const int HEIGHT_BAR = 62;
	const D3DXVECTOR3 POSITION = D3DXVECTOR3((float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 4, 0.0f);
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);
	const float OPEN_TIME = 0.5f;
	const float CLOSE_TIME = 0.3f;
	const float DISPLAY_TIME = 2.0f;
	const float ANNOUNCE_TIME = 1.0f;
}


//=============================================================================
//クラス定義
//=============================================================================
class Telop
{
private:
	Sprite* sprite;
	Input*	input;

	float frameTime;
	float announceTimer;
	float telopTimer;
	float displayTimer;
	int heightValue = 0;
	bool telopFlag;
	bool barFlag;
	int state;
	float rate;
	float closeTimer;
	bool* playFlag;
	 
	//サウンド
	PLAY_PARAMETERS openTelop;

public:
	Telop();
	~Telop();
	void initialize(LPDIRECT3DTEXTURE9 _texture, int _pivot,
		int _width, int _height, D3DXVECTOR3 _position, D3DXVECTOR3 _rotation, D3DCOLOR color);
	void uninitialize();
	void update(float _frameTimer);
	void render();
	void open();
	void openBar();
	void display();
	void close();
	void closeBar();
	void playTelop();
	void playTelopBar();
	void telopAnnounce();
	void setManagerFlag(bool* managerFlag);

};
