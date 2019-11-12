//============================
//[UItex.cpp]
//染矢晃介
//============================

//============================
//インクルード
//============================
#include "UIrank.h"
#include "Sound.h"

//============================
//【using宣言】
//============================
using namespace uiRankNS;

//============================
//グローバル変数
//============================

//============================
//初期化
//第一引数:ランクの種類
//第二引数:1Pか2Pか(0ならP1それ以外ならP2)
//============================
void UIrank::initialize(int rankType,int playerType)
{

	//SE再生フラグの初期化
	playedSE = true;
	//スプライトの初期化
	rank[rankType] = new Sprite;

	//サイズを代入
	widthSize = WIDTH_RANK;
	heightSize = HEIGHT_RANK;

	//ランクの種類とプレイヤーに応じて初期化
	switch (rankType)
	{
	case EXCELLENT:
		if (playerType == 0)
		{
			rank[EXCELLENT]->initialize(
				*textureNS::reference(textureNS::RESULT_EXCELLENT)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P1							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色

		}
		else
		{
			rank[EXCELLENT]->initialize(
				*textureNS::reference(textureNS::RESULT_EXCELLENT)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P2							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		}
		break;

	case GREAT:
		if (playerType == 0)
		{
			rank[GREAT]->initialize(
				*textureNS::reference(textureNS::RESULT_GREAT)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P1							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		}
		else
		{
			rank[GREAT]->initialize(
				*textureNS::reference(textureNS::RESULT_GREAT)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P2							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		}
		break;

	case CLEARE:
		if (playerType == 0)
		{
			rank[CLEARE]->initialize(
				*textureNS::reference(textureNS::RESULT_CLEARE)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P1							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		}
		else
		{
			rank[CLEARE]->initialize(
				*textureNS::reference(textureNS::RESULT_CLEARE)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P2							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		}
		break;

	case FAILED:
		if (playerType == 0)
		{
			rank[FAILED]->initialize(
				*textureNS::reference(textureNS::RESULT_FAILED)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P1							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		}
		else
		{
			rank[FAILED]->initialize(
				*textureNS::reference(textureNS::RESULT_FAILED)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P2							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		}
		break;

	default:
		break;
	}
}

//============================
//描画
//第一引数:ランクの種類
//============================
void UIrank::render(int rankType)
{
	rank[rankType]->render();
}

//============================
//更新
//第一引数:ランクの種類
//============================
void UIrank::update(int rankType)
{
	if (widthSize > END_WIDTH_RANK && heightSize > END_HEIGHT_RANK)
	{
		changeSize(rankType);
	}
	if (widthSize == END_WIDTH_RANK && heightSize == END_HEIGHT_RANK&&playedSE==true)
	{
		PLAY_PARAMETERS playParameters = { 0 };
		FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::HighPassFilter, 0.75f, 1.0f };
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Score, false,NULL,false,NULL,true, filterParameters };
		SoundInterface::SE->playSound(&playParameters);
		playedSE = false;
	}
}

//=============================
//終了処理
//第一引数:ランクの種類
//=============================
void UIrank::uninitialize(int rankType)
{
	delete rank[rankType];
}

//============================
//拡大縮小関数
//============================
void UIrank::changeSize(int rankType)
{
	widthSize -= SHRINK_SPEED;
	heightSize -= SHRINK_SPEED;
	rank[rankType]->setSize(widthSize, heightSize);
	rank[rankType]->setVertex();
}