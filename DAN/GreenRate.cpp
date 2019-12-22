//====================================
//緑化率処理[GreenRate.cpp]
//染矢晃介
//====================================

//====================================
//インクルード
//====================================
#include "GreenRate.h"


//====================================
//	[using宣言]
//====================================
using namespace greenRateNS;

//====================================
//コンストラクタ
//====================================
GreenRate::GreenRate()
{
	//Spriteの作成
	for (int i = 0; i < DIGIT_GREEN_RATE; i++)
	{
		timer[i] = new Sprite;
	}
}

//====================================
//デストラクタ
//====================================
GreenRate::~GreenRate()
{
	for (int i = 0; i < DIGIT_GREEN_RATE; i++)
	{
		delete timer[i];
	}
}

//====================================
//初期化
//====================================
void GreenRate::initialize(int playerType)
{
	switch (playerType)
	{
	case basicUiNS::P1:
		//初期表示位置の代入
		BasicUI::assingPosition(POSITION_GREEN_RATE);

		//初期サイズの代入
		BasicUI::assingSize(WIDTH_GREEN_RATE, HEIGHT_GREEN_RATE);

		keepRate = 0;

		//テクスチャの初期化
		for (int i = 0; i < DIGIT_GREEN_RATE; i++)
		{
			BasicUI::initialize(timer[i], textureNS::reference(textureNS::UI_TIMER_NUMBER));
		}
	break;
	case basicUiNS::P2:
		//初期表示位置の代入
		BasicUI::assingPosition(POSITION_GREEN_RATE_P2);

		//初期サイズの代入
		BasicUI::assingSize(WIDTH_GREEN_RATE, HEIGHT_GREEN_RATE);

		keepRate = 0;

		//テクスチャの初期化
		for (int i = 0; i < DIGIT_GREEN_RATE; i++)
		{
			BasicUI::initialize(timer[i], textureNS::reference(textureNS::UI_TIMER_NUMBER));
		}
	
	}
}

//====================================
//描画処理
//====================================
void GreenRate::render(int playerType,int state,float time)
{

	float gametime = time;
	setUV((int)keepRate);
	//ビジョン、スカイビジョン中のみ描画
	if (gametime > 60)//残り時間が60以上の時のみ表示
	{
		switch (state)
		{
		case 2:
			if (playerType == basicUiNS::P1)
			{
				for (int i = 0; i < DIGIT_GREEN_RATE; i++)
				{
					position.x = -WIDTH_GREEN_RATE * i + POSITION_GREEN_RATE.x;
					timer[i]->setPosition(position);
					timer[i]->setVertex();
					timer[i]->render();
				}
			}
			else if (playerType == basicUiNS::P2)
			{
				for (int i = 0; i < DIGIT_GREEN_RATE; i++)
				{
					position.x = -WIDTH_GREEN_RATE * i + POSITION_GREEN_RATE_P2.x;
					timer[i]->setPosition(position);
					timer[i]->setVertex();
					timer[i]->render();
				}
			}
			break;

		case 3:
			if (playerType == basicUiNS::P1)
			{
				for (int i = 0; i < DIGIT_GREEN_RATE; i++)
				{
					position.x = -WIDTH_GREEN_RATE * i + POSITION_GREEN_RATE.x;
					timer[i]->setPosition(position);
					timer[i]->setVertex();
					timer[i]->render();
				}
			}
			else if (playerType == basicUiNS::P2)
			{
				for (int i = 0; i < DIGIT_GREEN_RATE; i++)
				{
					position.x = -WIDTH_GREEN_RATE * i + POSITION_GREEN_RATE_P2.x;
					timer[i]->setPosition(position);
					timer[i]->setVertex();
					timer[i]->render();
				}
			}
			break;
		}
	}
	
}

//====================================
//更新処理
//====================================
void GreenRate::update(float gametime)
{
	//タイマーの初期値
	keepRate = gametime;
}

//====================================
//終了処理
//====================================
void GreenRate::uninitialize()
{

}

//====================================
// uv座標の設定
//====================================
void GreenRate::setUV(int time)
{
	for (int i = 0; i < DIGIT_GREEN_RATE; i++)
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


