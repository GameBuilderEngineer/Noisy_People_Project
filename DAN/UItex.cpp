//============================
//[UItex.cpp]
//染矢晃介
//============================

//============================
//インクルード
//============================
#include "UItex.h"

//============================
//【using宣言】
//============================
using namespace uiTexNS;

//============================
//グローバル変数
//============================

//============================
//初期化
//============================
void UItexture::initialize()
{

	//Spriteの作成とposに初期位置を代入
	for (int i = 0; i < UITEX_MAX; i++)
	{
		uiTexture[i] = new Sprite;
		assignPos(i);
	}

	//ラインの初期化
	uiTexture[LINE]->initialize(
		*textureNS::reference(textureNS::RESULT_LINE)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_LINE								//横幅
		, HEIGHT_LINE								//縦幅
		, POSITION_LINE								//表示位置
		, ROTATION_TEX								//回転
		, COLOR_TEX);								//色

	//フレームの初期化
	uiTexture[FLAME01]->initialize(
		*textureNS::reference(textureNS::RESULT_FLAME)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_FLAME								//横幅
		, HEIGHT_FLAME								//縦幅
		, POSITION_FLAME_01							//表示位置
		, ROTATION_TEX								//回転
		, COLOR_TEX);								//色

	//フレームの初期化
	uiTexture[FLAME02]->initialize(
		*textureNS::reference(textureNS::RESULT_FLAME02)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_FLAME								//横幅
		, HEIGHT_FLAME								//縦幅
		, POSITION_FLAME_02							//表示位置
		, ROTATION_TEX								//回転
		, COLOR_TEX);								//色

	//キャラAの初期化
	uiTexture[CHARA_A]->initialize(
		*textureNS::reference(textureNS::RESULT_CHARA_A)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_CHARA_A								//横幅
		, HEIGHT_CHARA_A							//縦幅
		, POSITION_CHARA_A							//表示位置
		, ROTATION_TEX								//回転
		, COLOR_TEX);								//色

	//キャラBの初期化
	uiTexture[CHARA_B]->initialize(
		*textureNS::reference(textureNS::RESULT_CHARA_B)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_CHARA_B								//横幅
		, HEIGHT_CHARA_B							//縦幅
		, POSITION_CHARA_B							//表示位置
		, ROTATION_TEX								//回転
		, COLOR_TEX);								//色

	//NEXTテクスチャの初期化
	uiTexture[NEXT]->initialize(
		*textureNS::reference(textureNS::RESULT_NEXT)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_NEXT								//横幅
		, HEIGHT_NEXT								//縦幅
		, POSITION_NEXT								//表示位置
		, ROTATION_TEX								//回転
		, COLOR_TEX);								//色

}

//============================
//描画
//============================
void UItexture::render(int resultPhase)
{

	switch (resultPhase)
	{
	case PHASE_01:
		uiTexture[LINE]->render();
		break;
	case PHASE_02:
		uiTexture[LINE]->render();
		break;
	case PHASE_03:
		uiTexture[LINE]->render();
		uiTexture[FLAME01]->render();
		uiTexture[FLAME02]->render();
		uiTexture[CHARA_A]->render();
		uiTexture[CHARA_B]->render();
		break;
	case PHASE_04:
		uiTexture[LINE]->render();
		uiTexture[FLAME01]->render();
		uiTexture[FLAME02]->render();
		uiTexture[CHARA_A]->render();
		uiTexture[CHARA_B]->render();
		break;
	case PHASE_05:
		uiTexture[LINE]->render();
		uiTexture[FLAME01]->render();
		uiTexture[FLAME02]->render();
		uiTexture[CHARA_A]->render();
		uiTexture[CHARA_B]->render();
		uiTexture[NEXT]->render();
		break;
	default:
		break;
	}
}

//============================
//更新
//============================
void UItexture::update(int resultPhase)
{
	switch (resultPhase)
	{
	case PHASE_01:
		if (pos[LINE].x < END_POS_LINE)
		{
			slideRight(LINE);
		}
		break;

	case PHASE_02:
		if (pos[LINE].x < END_POS_LINE)
		{
			slideRight(LINE);
		}
		break;
	case PHASE_03:
		if (pos[LINE].x < END_POS_LINE)
		{
			slideRight(LINE);
		}
		if (pos[FLAME01].y > END_POS_FLAME)
		{
			slideUp(FLAME01);
			slideUp(FLAME02);
		}
		if (pos[CHARA_A].y > END_POS_CHARA_A)
		{
			slideUp(CHARA_A);
			slideUp(CHARA_B);
		}
		break;
	case PHASE_04:
		if (pos[LINE].x < END_POS_LINE)
		{
			slideRight(LINE);
		}
		if (pos[FLAME01].y > END_POS_FLAME)
		{
			slideUp(FLAME01);
			slideUp(FLAME02);
		}
		if (pos[CHARA_A].y > END_POS_CHARA_A)
		{
			slideUp(CHARA_A);
			slideUp(CHARA_B);
		}
		break;
	case PHASE_05:
		if (pos[LINE].x < END_POS_LINE)
		{
			slideRight(LINE);
		}
		if (pos[FLAME01].y > END_POS_FLAME)
		{
			slideUp(FLAME01);
			slideUp(FLAME02);
		}
		if (pos[CHARA_A].y > END_POS_CHARA_A)
		{
			slideUp(CHARA_A);
			slideUp(CHARA_B);
		}
		if (pos[NEXT].x > END_POS_NEXT)
		{
			slideLeft(NEXT);
		}
		break;
	default:
		break;
	}
}

//=====================================
//終了処理
//=====================================
void UItexture::uninitialize()
{
	//Spriteの削除
	for (int i = 0; i < UITEX_MAX; i++)
	{
		delete uiTexture[i];
	}
}

//======================================
//右にスライドイン処理関数
//第一引数：スライドインさせるUIの種類
//======================================
void UItexture::slideRight(int uiType)
{
	//右へのスライドイン
	pos[uiType].x += SLIDE_SPEED_TEX;
	uiTexture[uiType]->setPosition(pos[uiType]);
	uiTexture[uiType]->setVertex();
}

//======================================
//左にスライドイン処理関数
//第一引数：スライドインさせるUIの種類
//======================================
void UItexture::slideLeft(int uiType)
{
	//左へのスライドイン
	pos[uiType].x -= SLIDE_SPEED_TEX;
	uiTexture[uiType]->setPosition(pos[uiType]);
	uiTexture[uiType]->setVertex();
}

//======================================
//上にスライドイン処理関数
//第一引数：スライドインさせるUIの種類
//======================================
void UItexture::slideUp(int uiType)
{
	//上へのスライドイン
	pos[uiType].y -= SLIDE_SPEED_TEX;
	uiTexture[uiType]->setPosition(pos[uiType]);
	uiTexture[uiType]->setVertex();
}

//========================================
//変数posに各UIの表示位置を代入する関数
//========================================
void UItexture::assignPos(int uiType)
{
	//posにそれぞれの座標を代入
	switch (uiType)
	{
	case LINE:
		pos[LINE] = POSITION_LINE;
		break;

	case FLAME01:
		pos[FLAME01] = POSITION_FLAME_01;
		break;
		
	case FLAME02:
		pos[FLAME02] = POSITION_FLAME_02;
		break;

	case CHARA_A:
		pos[CHARA_A] = POSITION_CHARA_A;
		break;

	case CHARA_B:
		pos[CHARA_B] = POSITION_CHARA_B;
		break;

	case NEXT:
		pos[NEXT] = POSITION_NEXT;
		break;

	default:
		break;
	}
}