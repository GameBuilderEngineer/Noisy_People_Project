//============================
//[ResultUI.cpp]
//染矢晃介
//============================

//============================
//インクルード
//============================
#include "ResultUI.h"

//============================
//【using宣言】
//============================
using namespace resultUiNS;

//============================
//グローバル変数
//============================

//============================
//初期化
//============================
void ResultUI::initialize()
{
	for (int i = 0; i < RESULTUI_MAX; i++)
	{
		resultUI[i] = new Sprite;
	}

	//フェイズの初期化(第一フェイズで)
	resultPhase = PHASE_01;

	//リザルトロゴの初期化
	resultUI[RESULT]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_RESULT								//横幅
		, HEIGHT_RESULT								//縦幅
		, POSITION_RESULT							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//ラインの初期化
	resultUI[LINE]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_LINE								//横幅
		, HEIGHT_LINE								//縦幅
		, POSITION_LINE								//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//フレームの初期化
	resultUI[FLAME]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_FLAME								//横幅
		, HEIGHT_FLAME								//縦幅
		, POSITION_FLAME							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//キャラAの初期化
	resultUI[CHARA_A]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_CHARA_A								//横幅
		, HEIGHT_CHARA_A							//縦幅
		, POSITION_CHARA_A							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//キャラBの初期化
	resultUI[CHARA_B]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_CHARA_B								//横幅
		, HEIGHT_CHARA_B							//縦幅
		, POSITION_CHARA_B							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//プレイヤーの初期化
	resultUI[PLAYER]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_PLAYER								//横幅
		, HEIGHT_PLAYER								//縦幅
		, POSITION_PLAYER							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//緑化(英語)の初期化
	resultUI[GREENIG]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_GREENIG								//横幅
		, HEIGHT_GREENIG							//縦幅
		, POSITION_GREENIG							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//緑化
	resultUI[RYOKUKA]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_RYOKUKA								//横幅
		, HEIGHT_RYOKUKA							//縦幅
		, POSITION_RYOKUKA							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//パーセントの初期化
	resultUI[PERSENT]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_PERSENT								//横幅
		, HEIGHT_PERSENT							//縦幅
		, POSITION_PERSENT							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//数字の初期化
	resultUI[NUMBER]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_NUMBER								//横幅
		, HEIGHT_NUMBER								//縦幅
		, POSITION_NUMBER							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//撃退(英語)の初期化
	resultUI[DEFEAT]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_DEFEAT								//横幅
		, HEIGHT_DEFEAT								//縦幅
		, POSITION_DEFEAT							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//撃退
	resultUI[GEKITAI]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_GEKITAI								//横幅
		, HEIGHT_GEKITAI							//縦幅
		, POSITION_GEKITAI							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//エクセレントの初期化
	resultUI[EXCELLENT]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_EXCELLENT							//横幅
		, HEIGHT_EXCELLENT							//縦幅
		, POSITION_EXCELLENT						//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//グレイトの初期化
	resultUI[GREAT]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_GREAT								//横幅
		, HEIGHT_GREAT								//縦幅
		, POSITION_GREAT							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//クリアの初期化
	resultUI[CLEAR]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_CLEAR								//横幅
		, HEIGHT_CLEAR								//縦幅
		, POSITION_CLEAR							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//失敗の初期化
	resultUI[FAIL]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_FAIL								//横幅
		, HEIGHT_FAIL								//縦幅
		, POSITION_FAIL								//表示位置
		, ROTATION									//回転
		, COLOR);									//色
}

//============================
//描画
//============================
void ResultUI::render()
{
	switch (resultPhase)
	{
		case PHASE_01:
			resultUI[RESULT]->render();
			resultUI[LINE]->render();
			break;
		case PHASE_02:
			resultUI[RESULT]->render();
			resultUI[LINE]->render();
			break;
		case PHASE_03:
			resultUI[RESULT]->render();
			resultUI[LINE]->render();
			resultUI[FLAME]->render();
			resultUI[CHARA_A]->render();
			resultUI[CHARA_B]->render();
			break;
		case PHASE_04:
			break;
		default:
			break;
	}
}

//============================
//更新
//============================
void ResultUI::update(Input *input)
{
	
}

//============================
//終了処理
//============================
void ResultUI::uninitialize()
{
	for (int i = 0; i < RESULTUI_MAX; i++)
	{
		delete resultUI[i];
	}

}

