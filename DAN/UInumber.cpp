//============================
//[UInumber.cpp]
//染矢晃介
//============================

//============================
//インクルード
//============================
#include "UInumber.h"

//============================
//【using宣言】
//============================
using namespace uiNumberNS;

//============================
//グローバル変数
//============================

//============================
//初期化
//第一引数：どこの数字なのかを指定
//============================
void UInumber::initialize(int numType)
{
	//どこの数字かを参照し初期位置の設定
	switch (numType)
	{
	case P1_GREENIG:
		numberPos = P1_GREENIG_POS;
		break;

	case P2_GREENIG:
		numberPos = P2_GREENIG_POS;
		break;

	case P1_DEFEAT:
		numberPos = P1_DEFEAT_POS;
		break;

	case P2_DEFEAT:
		numberPos = P2_DEFEAT_POS;
		break;

	default:
		break;

	}
	
	//桁数分のスプライトの作成と初期化
	for (int i = 0; i < DIGIT_NUMBER; i++)
	{
		number[i] = new Sprite;
		number[i]->initialize(
			*textureNS::reference(textureNS::NUMBER)	//テクスチャ
			, SpriteNS::CENTER							//中心
			, WIDTH_NUMBER								//横幅
			, HEIGHT_NUMBER								//縦幅
			, numberPos									//表示位置
			, ROTATION_TEX								//回転
			, COLOR_TEX);								//色
	}

	//uv座標の代入
	assingUV();

	//表示座標の代入
	assingPos();

	//表示する数字の初期化
	displayScore = 0;
}


//============================
//描画
//第一引数:スコアをもらってsetする
//============================
void UInumber::render()
{
	setNumber((int)displayScore);

	for (int i = 0; i < DIGIT_NUMBER; i++)
	{
		pos[i].x = -WIDTH_NUMBER * i + numberPos.x;
		number[i]->setPosition(pos[i]);
		number[i]->setVertex();
		number[i]->render();
	}
}

//============================
//更新
//============================
void UInumber::update(int score)
{
	countUp(score);
}


//=============================
//終了処理
//=============================
void UInumber::uninitialize()
{
	for (int i = 0; i < DIGIT_NUMBER; i++)
	{
		delete number[i];
	}
}


//============================
//uv座標の代入関数
//============================
void UInumber::assingUV()
{
	uvCoord01 = D3DXVECTOR2(0.0, 0.0);
	uvCoord02 = D3DXVECTOR2(1.0, 0.0);
	uvCoord03 = D3DXVECTOR2(0.0, 1.0);
	uvCoord04 = D3DXVECTOR2(1.0, 1.0);
}

//============================
//ポジションの代入関数
//============================
void UInumber::assingPos()
{
	for (int i = 0; i < DIGIT_NUMBER; i++)
	{
		pos[i] = numberPos;
	}
}

//============================
//スコアの数字を設定
//============================
void UInumber::setNumber(int score)
{
	num = score;

	for (int i = 0; i < DIGIT_NUMBER; i++)
	{
		float x = (float)(num % 10);
		uvCoord01.x = 0.1*x;
		uvCoord02.x = 0.1 *(x + 1);
		uvCoord03.x = 0.1 *x;
		uvCoord04.x = 0.1 *(x + 1);
		num /= 10;
		number[i]->setUVCoord(uvCoord01, uvCoord02, uvCoord03, uvCoord04);
	}
}

//==============================
//カウントアップ関数
//==============================
void UInumber::countUp(int score)
{
	// 最終的に表示したいスコアから表示中のスコアをひいて差分を出す
	if (displayScore < score)
	{
		//保存用変数に表示しているスコアと最終的に表示したいスコアの差分を入れる
		keepScore = (score)-(displayScore);
		//表示しているスコアに差分をカウントアップにかけたいもので割ったものを足す
		displayScore += (keepScore / COUNT_TIME);
		//小数点切り上げのため
		displayScore += 0.9;
	}
	else
	{
		displayScore = score;
	}
}