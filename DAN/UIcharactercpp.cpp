//============================
//[UIcharacter.cpp]
//染矢晃介
//============================

//============================
//インクルード
//============================
#include "UIcharacter.h"

//============================
//【using宣言】
//============================
using namespace uiCharacterNS;

//============================
//グローバル変数
//============================

//============================
//初期化
//============================
void UIcharacter::initialize(int playerType)
{
	//Spriteの作成とposに初期位置を代入
	for (int i = 0; i < UICHARACTER_MAX; i++)
	{
		uiCharacter[i] = new Sprite;
		assignPos(i,playerType);
	}

	//リザルトロゴの初期化
	uiCharacter[RESULT]->initialize(
		*textureNS::reference(textureNS::RESULT)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_RESULT								//横幅
		, HEIGHT_RESULT								//縦幅
		, POSITION_RESULT							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//プレイヤーの初期化
	uiCharacter[PLAYER1]->initialize(
		*textureNS::reference(textureNS::RESULT_PLAYER)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_PLAYER								//横幅
		, HEIGHT_PLAYER								//縦幅
		, POSITION_PLAYER							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	uiCharacter[PLAYER2]->initialize(
		*textureNS::reference(textureNS::RESULT_PLAYER)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_PLAYER								//横幅
		, HEIGHT_PLAYER								//縦幅
		, POSITION_PLAYER							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//緑化(英語)の初期化
	uiCharacter[GREENIG]->initialize(
		*textureNS::reference(textureNS::RESULT_GREENIG)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_GREENIG								//横幅
		, HEIGHT_GREENIG							//縦幅
		, POSITION_GREENIG_01						//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//緑化
	uiCharacter[RYOKUKA]->initialize(
		*textureNS::reference(textureNS::RESULT_RYOKUKA)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_RYOKUKA								//横幅
		, HEIGHT_RYOKUKA							//縦幅
		, POSITION_RYOKUKA_01						//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//パーセントの初期化
	uiCharacter[PERSENT]->initialize(
		*textureNS::reference(textureNS::RESULT_PERSENT)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_PERSENT								//横幅
		, HEIGHT_PERSENT							//縦幅
		, POSITION_PERSENT_01						//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//撃退(英語)の初期化
	uiCharacter[DEFEAT]->initialize(
		*textureNS::reference(textureNS::RESULT_DEFEAT)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_DEFEAT								//横幅
		, HEIGHT_DEFEAT								//縦幅
		, POSITION_DEFEAT_01						//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//撃退
	uiCharacter[GEKITAI]->initialize(
		*textureNS::reference(textureNS::RESULT_GEKITAI)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_GEKITAI								//横幅
		, HEIGHT_GEKITAI							//縦幅
		, POSITION_GEKITAI_01						//表示位置
		, ROTATION									//回転
		, COLOR);									//色
}

//============================
//描画
//============================
void UIcharacter::render(int resultPhase)
{
	
	switch (resultPhase)
	{
	case PHASE_01:
		uiCharacter[RESULT]->render();
		break;
	case PHASE_02:
		uiCharacter[RESULT]->render();
		break;
	case PHASE_03:
		uiCharacter[RESULT]->render();
		uiCharacter[PLAYER1]->render();
		uiCharacter[PLAYER2]->render();
		uiCharacter[GREENIG]->render();
		uiCharacter[RYOKUKA]->render();
		uiCharacter[PERSENT]->render();
		uiCharacter[DEFEAT]->render();
		uiCharacter[GEKITAI]->render();
		break;
	case PHASE_04:
		uiCharacter[RESULT]->render();
		uiCharacter[PLAYER1]->render();
		uiCharacter[PLAYER2]->render();
		uiCharacter[GREENIG]->render();
		uiCharacter[RYOKUKA]->render();
		uiCharacter[PERSENT]->render();
		uiCharacter[DEFEAT]->render();
		uiCharacter[GEKITAI]->render();
		break;
	case PHASE_05:
		uiCharacter[RESULT]->render();
		uiCharacter[PLAYER1]->render();
		uiCharacter[PLAYER2]->render();
		uiCharacter[GREENIG]->render();
		uiCharacter[RYOKUKA]->render();
		uiCharacter[PERSENT]->render();
		uiCharacter[DEFEAT]->render();
		uiCharacter[GEKITAI]->render();
	default:
		break;
	}
}

//============================
//更新
//============================
void UIcharacter::update(int resultPhase)
{
	//プレイヤーのuv座標の設定
	setUV();

	switch (resultPhase)
	{
	case PHASE_01:

		if (pos[RESULT].x > END_POS_RESULT)
		{
			slideLeft(RESULT);
		}

		break;
	case PHASE_02:
		break;
	case PHASE_03:

		if (pos[PLAYER1].x > END_POS_PLAYER_01)
		{
			slideLeft(PLAYER1);
		}

		if (pos[PLAYER2].x > END_POS_PLAYER_02)
		{
			slideLeft(PLAYER2);
		}

		if (pos[GREENIG].y > END_POS_GREENING)
		{
			slideUp(GREENIG);
		}

		if (pos[RYOKUKA].y > END_POS_RYOKUKA)
		{
			slideUp(RYOKUKA);
		}

		if (pos[PERSENT].y > END_POS_PERSENT)
		{
			slideUp(PERSENT);
		}

		if (pos[DEFEAT].y > END_POS_DEFEAT)
		{
			slideUp(DEFEAT);
		}

		if (pos[GEKITAI].y > END_POS_GEKITAI)
		{
			slideUp(GEKITAI);
		}

		break;
	case PHASE_04:
		break;
	default:
		break;
	}
}

//=====================================
//終了処理
//=====================================
void UIcharacter::uninitialize()
{
	//Spriteの削除
	for (int i = 0; i < UICHARACTER_MAX; i++)
	{
		delete uiCharacter[i];
	}
}

//======================================
//右にスライドイン処理関数
//第一引数：スライドインさせるUIの種類
//======================================
void UIcharacter::slideRight(int uiType)
{
	//右へのスライドイン
	pos[uiType].x += SLIDE_SPEED;		
	uiCharacter[uiType]->setPosition(pos[uiType]);
	uiCharacter[uiType]->setVertex();
}

//======================================
//左にスライドイン処理関数
//第一引数：スライドインさせるUIの種類
//======================================
void UIcharacter::slideLeft(int uiType)
{
	//左へのスライドイン
	pos[uiType].x -= SLIDE_SPEED;
	uiCharacter[uiType]->setPosition(pos[uiType]);
	uiCharacter[uiType]->setVertex();
}

//======================================
//上にスライドイン処理関数
//第一引数：スライドインさせるUIの種類
//======================================
void UIcharacter::slideUp(int uiType)
{
	//上へのスライドイン
	pos[uiType].y -= SLIDE_SPEED;
	uiCharacter[uiType]->setPosition(pos[uiType]);
	uiCharacter[uiType]->setVertex();
}

//========================================
//変数posに各UIの表示位置を代入する関数
//第一引数:UIの種類
//第二引数:プレイヤーの種類
//========================================
void UIcharacter::assignPos(int uiType,int playerType)
{
	//posにそれぞれの座標を代入
	switch (uiType)
	{
	case RESULT:
		pos[RESULT] = POSITION_RESULT;
		break;
	case PLAYER1:
		if (playerType == 0)
		{
			pos[PLAYER1] = POSITION_PLAYER;
			break;
		}
		else
		{
			pos[PLAYER1] = POSITION_PLAYER;
			break;
		}
	case PLAYER2:
		if (playerType == 0)
		{
			pos[PLAYER2] = POSITION_PLAYER;
			break;
		}
		else
		{
			pos[PLAYER2] = POSITION_PLAYER;
			break;
		}
	case GREENIG:
		if (playerType == 0)
		{
			pos[GREENIG] = POSITION_GREENIG_01;
			break;
		}
		else
		{
			pos[GREENIG] = POSITION_GREENIG_02;
			break;
		}
	case RYOKUKA:
		if (playerType == 0)
		{
			pos[RYOKUKA] = POSITION_RYOKUKA_01;
			break;
		}
		else
		{
			pos[RYOKUKA] = POSITION_RYOKUKA_02;
			break;
		}
	case PERSENT:
		if (playerType == 0)
		{
			pos[PERSENT] = POSITION_PERSENT_01;
			break;
		}
		else
		{
			pos[PERSENT] = POSITION_PERSENT_02;
			break;
		}
	case DEFEAT:
		if (playerType == 0)
		{
			pos[DEFEAT] = POSITION_DEFEAT_01;
			break;
		}
		else
		{
			pos[DEFEAT] = POSITION_DEFEAT_02;
			break;
		}
	case GEKITAI:
		if (playerType == 0)
		{
			pos[GEKITAI] = POSITION_GEKITAI_01;
			break;
		}
		else
		{
			pos[GEKITAI] = POSITION_GEKITAI_02;
			break;
		}
	default:
		break;
	}
}

//================================
//プレイヤーのuv座標の設定
//================================
void UIcharacter::setUV()
{
	uiCharacter[PLAYER1]->setUVCoord(PLAYER1_UV_VERTEX01, PLAYER1_UV_VERTEX02, PLAYER1_UV_VERTEX03, PLAYER1_UV_VERTEX04);
	uiCharacter[PLAYER2]->setUVCoord(PLAYER2_UV_VERTEX01, PLAYER2_UV_VERTEX02, PLAYER2_UV_VERTEX03, PLAYER2_UV_VERTEX04);
}