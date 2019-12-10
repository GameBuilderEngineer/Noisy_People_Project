//=============================================
//[UItex.cpp]
//染矢晃介
//=============================================

//=============================================
//インクルード
//=============================================
#include "UIrank.h"
#include "Sound.h"

//=============================================
//【using宣言】
//=============================================
using namespace uiRankNS;

//=============================================
//コンストラクタ
//=============================================
UIrank::UIrank()
{
	for (int i = 0; i < RANK_MAX; i++)
	{
		rank[basicUiNS::P1][i] = new Sprite;
		rank[basicUiNS::P2][i] = new Sprite;
	}
}

//=============================================
//デストラクタ
//=============================================
UIrank::~UIrank()
{
	for (int i = 0; i < RANK_MAX; i++)
	{
		delete rank[basicUiNS::P1][i];
		delete rank[basicUiNS::P2][i];
	}
}

//============================
//初期化
//第一引数:ランクの種類
//第二引数:1Pか2Pか(0ならP1それ以外ならP2)
//============================
void UIrank::initialize()
{

	//SE再生フラグの初期化
	playedSE = true;

	//サイズを代入
	widthSize = WIDTH_RANK;
	heightSize = HEIGHT_RANK;

	//ランクの種類とプレイヤーに応じて初期化
	rank[basicUiNS::P1][EXCELLENT]->initialize(
				*textureNS::reference(textureNS::RESULT_EXCELLENT)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P1							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色

		
	rank[basicUiNS::P2][EXCELLENT]->initialize(
				*textureNS::reference(textureNS::RESULT_EXCELLENT)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P2							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		
	rank[basicUiNS::P1][GREAT]->initialize(
				*textureNS::reference(textureNS::RESULT_GREAT)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P1							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		
	rank[basicUiNS::P2][GREAT]->initialize(
				*textureNS::reference(textureNS::RESULT_GREAT)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P2							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		
	rank[basicUiNS::P1][CLEARE]->initialize(
				*textureNS::reference(textureNS::RESULT_CLEARE)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P1							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		
	rank[basicUiNS::P2][CLEARE]->initialize(
				*textureNS::reference(textureNS::RESULT_CLEARE)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P2							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		
	rank[basicUiNS::P1][FAILED]->initialize(
				*textureNS::reference(textureNS::RESULT_FAILED)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P1							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		
	rank[basicUiNS::P2][FAILED]->initialize(
				*textureNS::reference(textureNS::RESULT_FAILED)//テクスチャ
				, SpriteNS::CENTER							//中心
				, WIDTH_RANK								//横幅
				, HEIGHT_RANK								//縦幅
				, POSITION_RANK_P2							//表示位置
				, ROTATION_TEX								//回転
				, COLOR_TEX);								//色
		
}

//============================
//描画
//第一引数:ランクの種類
//============================
void UIrank::render(int rankType1,int rankType2)
{
	rank[basicUiNS::P1][rankType1]->render();
	rank[basicUiNS::P2][rankType2]->render();
}

//============================
//更新
//第一引数:ランクの種類
//============================
void UIrank::update(int rankType1, int rankType2)
{
	if (widthSize > END_WIDTH_RANK && heightSize > END_HEIGHT_RANK)
	{
		widthSize -= SHRINK_SPEED;
		heightSize -= SHRINK_SPEED;
		rank[basicUiNS::P1][rankType1]->setSize(widthSize, heightSize);
		rank[basicUiNS::P1][rankType1]->setVertex();
		rank[basicUiNS::P1][rankType1]->render();
		rank[basicUiNS::P2][rankType2]->setSize(widthSize, heightSize);
		rank[basicUiNS::P2][rankType2]->setVertex();
		rank[basicUiNS::P2][rankType2]->render();
	}
	if (widthSize == END_WIDTH_RANK && heightSize == END_HEIGHT_RANK&&playedSE==true)
	{
		PLAY_PARAMETERS playParameters = { 0 };
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Score, false,NULL,false,NULL};
		SoundInterface::SE->playSound(&playParameters);
		playedSE = false;
	}
}
