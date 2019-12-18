//============================
//[TitleUI.cpp]
//染矢晃介
//============================

//============================
//インクルード
//============================
#include "TitleUI.h"
#include "Sound.h"
#include "BasicUI.h"

//============================
//【using宣言】
//============================
using namespace titleUiNS;


//============================
//グローバル変数
//============================

//============================
//初期化
//============================
void TitleUI::initialize()
{
	//UIの総数分の初期化
	for (int i = 0; i < UI_MAX; i++)
	{
		UI[i] = new Sprite;
	}
	
	//ロゴの初期化
	UI[UI_LOGO]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_LOGO								//横幅
		, HEIGHT_LOGO								//縦幅
		, POSITION_LOGO								//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//選択肢の初期化
	UI[UI_MENU]->initialize(
		*textureNS::reference(textureNS::TITLE_MENU)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_MENU								//横幅
		, HEIGHT_MENU								//縦幅
		, POSITION_MENU								//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//説明の初期化
	UI[UI_INFO_TUTO]->initialize(
		*textureNS::reference(textureNS::TITLE_INFO_TUTO)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_MENU_INFO							//横幅
		, HEIGHT_MENU_INFO							//縦幅
		, POSITION_MENU_INFO						//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	UI[UI_INFO_GAME]->initialize(
		*textureNS::reference(textureNS::TITLE_INFO_GAME)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_MENU_INFO							//横幅
		, HEIGHT_MENU_INFO							//縦幅
		, POSITION_MENU_INFO						//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	UI[UI_INFO_CREDIT]->initialize(
		*textureNS::reference(textureNS::TITLE_INFO_CREDIT)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_MENU_INFO							//横幅
		, HEIGHT_MENU_INFO							//縦幅
		, POSITION_MENU_INFO						//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	UI[UI_INFO_EXIT]->initialize(
		*textureNS::reference(textureNS::TITLE_INFO_EXIT)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_MENU_INFO							//横幅
		, HEIGHT_MENU_INFO							//縦幅
		, POSITION_MENU_INFO						//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//制作者情報の初期化
	UI[UI_COPYRIGHT]->initialize(
		*textureNS::reference(textureNS::TITLE_COPYRIGHT)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_COPYRIGHT							//横幅
		, HEIGHT_COPYRIGHT							//縦幅
		, POSITION_COPYRIGHT						//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//バーの初期化
	UI[UI_BAR]->initialize(
		*textureNS::reference(textureNS::TITLE_BAR)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_BAR									//横幅
		, HEIGHT_BAR								//縦幅
		, POSITION_BAR_TUTO							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//プレスエニーボタン
	UI[UI_PRESS]->initialize(
		*textureNS::reference(textureNS::TITLE_PRESS)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_PRESS									//横幅
		, HEIGHT_PRESS								//縦幅
		, POSITION_PRESS							//表示位置
		, ROTATION									//回転
		, COLOR);									//色

	//選択状態の初期化
	selectState = TUTORIAL;

	//α値の初期化
	alpha = 0.0f;

	//タイトル状態の初期化
	titleState = 0;

	//タイトル時間
	titleTime = 0.0f;
}

//============================
//描画
//============================
void TitleUI::render()
{
	if (titleState == 0)
	{
		UI[UI_PRESS]->render();
	}
	else
	{
		if (titleTime >FLOAT_LOGO_TIME)
		{
			UI[UI_LOGO]->render();
		}
		UI[UI_BAR]->render();
		UI[UI_MENU]->render();
		switch (selectState)
		{
		case TUTORIAL:
			UI[UI_INFO_TUTO]->render();
			break;

		case GAME:
			UI[UI_INFO_GAME]->render();
			break;

		case CREDIT:
			UI[UI_INFO_CREDIT]->render();
			break;

		case EXIT:
			UI[UI_INFO_EXIT]->render();
			break;

		}
		UI[UI_COPYRIGHT]->render();
	}
}

//============================
//更新
//============================
void TitleUI::update(Input *input,float flametime)
{
	if (input->wasKeyPressed(VK_RETURN) ||
		input->wasKeyPressed(VK_SPACE) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::SPECIAL_MAIN)
		)
	{
		titleState=1;
	}

	if (titleState == 1)
	{
		titleTime += flametime;
		if (alpha < 255 && titleTime>FLOAT_LOGO_TIME)
		{
			alpha += ALPHA_SPEED;
			UI[UI_LOGO]->setAlphaAllVertex(alpha);
		}
		else if(alpha>=255)
		{
			UI[UI_LOGO]->setAlphaAllVertex(255);
		}
	
		ringSE(input);

		if (selectState > EXIT)
		{
			selectState = TUTORIAL;
		}

		else if (selectState < TUTORIAL)
		{
			selectState = EXIT;
		}
		moveBar();
	}
}

//============================
//終了処理
//============================
void TitleUI::uninitialize()
{
	//UIをすべて終了
	for (int i = 0; i < UI_MAX; i++)
	{
		delete UI[i];
	}
	
}

//===========================
//選択肢状態の取得関数
//戻り値：selectState
//===========================
int TitleUI::getSelectState()
{
	return selectState;
}

//===========================
//バーの移動用関数
//===========================
void TitleUI::moveBar()
{
	switch (selectState)
	{
	case TUTORIAL:
		UI[UI_BAR]->setPosition(POSITION_BAR_TUTO);
		break;

	case GAME:
		UI[UI_BAR]->setPosition(POSITION_BAR_GAME);
		break;

	case CREDIT:
		UI[UI_BAR]->setPosition(POSITION_BAR_CREDIT);
		break;

	case EXIT:
		UI[UI_BAR]->setPosition(POSITION_BAR_EXIT);
		break;

	}

	UI[UI_BAR]->setVertex();
}

//===============================
//音を鳴らす処理
//===============================
void TitleUI::ringSE(Input *input)
{
	if (input->wasKeyPressed(VK_DOWN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::DOWN) ||
		(input->getController()[inputNS::DINPUT_1P]->checkConnect() && input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().y>0))
	{
		//ボタンが押される度にSEのを鳴らす処理
		PLAY_PARAMETERS playParameters = { 0 };
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,SE_LIST::SE_Cursor, false,NULL,false,NULL };//SEの1曲目の設定
		SoundInterface::SE->playSound(&playParameters);
		selectState++;
	}

	else if (input->wasKeyPressed(VK_UP) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::UP) ||
		(input->getController()[inputNS::DINPUT_1P]->checkConnect() && input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().y<0))
	{
		//ボタンが押される度にSEのを鳴らす処理
		PLAY_PARAMETERS playParameters = { 0 };
		//FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::HighPassFilter, 0.85f, 1.5f };//フィルターの設定
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,SE_LIST::SE_Cursor, false,NULL,false,NULL };//SEの1曲目の設定
		SoundInterface::SE->playSound(&playParameters);
		selectState--;
	}
}

