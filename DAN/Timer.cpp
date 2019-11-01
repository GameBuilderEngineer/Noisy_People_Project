//====================================
//タイマー処理[Timer.cpp]
//染矢晃介
//====================================

//====================================
//インクルード
//====================================
#include "Timer.h"

//====================================
//	[using宣言]
//====================================
using namespace timerNS;

//====================================
//コンストラクタ
//====================================
Timer::Timer()
{
	//Spriteの作成
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		timer[i] = new Sprite;
	}
}

//====================================
//デストラクタ
//====================================
Timer::~Timer()
{
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		delete timer[i];
	}
}

//====================================
//初期化
//====================================
void Timer::initialize()
{
	//uv座標の代入
	assingUV();

	//タイマーの初期値
	keepTime = 400;

	//タイマーの初期化
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		timer[i]->initialize(
			*textureNS::reference(textureNS::NUMBER)//テクスチャ
			, SpriteNS::CENTER							//中心
			, WIDTH_TIMER								//横幅
			, HEIGHT_TIMER								//縦幅
			, POSITION_TIMER							//表示位置
			, ROTATION_TIMER							//回転
			, COLOR_TIMER);								//色
		pos[i] = POSITION_TIMER;						//表示位置の代入
	}
	
}

//====================================
//描画処理
//====================================
void Timer::render()
{

	setUV((int)keepTime);
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		pos[i].x = -WIDTH_TIMER * i + POSITION_TIMER.x;
		timer[i]->setPosition(pos[i]);
		timer[i]->setVertex();
		timer[i]->render();
	}
}

//====================================
//更新処理
//====================================
void Timer::update()
{
	//60フレームで１減少
	keepTime -= 1.0f / 60.0f;
}

//====================================
//終了処理
//====================================
void Timer::uninitialize()
{
	
}

//====================================
// uv座標の設定
//====================================
void Timer::setUV(int time)
{
	
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		float x = (float)(time % 10);
		uvCoord01.x = 0.1*x;
		uvCoord02.x = 0.1 *(x + 1);
		uvCoord03.x = 0.1 *x;
		uvCoord04.x = 0.1 *(x + 1);
		time /= 10;
		timer[i]->setUVCoord(uvCoord01, uvCoord02, uvCoord03, uvCoord04);
	}
}

//===================================
// uv座標の代入
//===================================
void Timer::assingUV()
{
	uvCoord01 = D3DXVECTOR2(0.0, 0.0);
	uvCoord02 = D3DXVECTOR2(1.0, 0.0);
	uvCoord03 = D3DXVECTOR2(0.0, 1.0);
	uvCoord04 = D3DXVECTOR2(1.0, 1.0);
}

