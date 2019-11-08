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
	//初期表示位置の代入
	BasicUI::assingPosition(POSITION_TIMER);

	//初期サイズの代入
	BasicUI::assingSize(WIDTH_TIMER, HEIGHT_TIMER);

	//タイマーの初期値
	keepTime = GAME_TIME;

	//テクスチャの初期化
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		BasicUI::initialize(timer[i], textureNS::reference(textureNS::NUMBER));
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
		position.x = -WIDTH_TIMER * i + POSITION_TIMER.x;
		timer[i]->setPosition(position);
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
		uvCoord01.x = 0.1f *x;
		uvCoord02.x = 0.1f *(x + 1);
		uvCoord03.x = 0.1f *x;
		uvCoord04.x = 0.1f *(x + 1);
		time /= 10;
		timer[i]->setUVCoord(uvCoord01, uvCoord02, uvCoord03, uvCoord04);
	}
}


