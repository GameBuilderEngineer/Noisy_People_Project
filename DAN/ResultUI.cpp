//============================
//[ResultUI.cpp]
//染矢晃介
//============================

//============================
//インクルード
//============================
#include "ResultUI.h"
#include "Sound.h"

//============================
//【using宣言】
//============================
using namespace resultUiNS;

//============================
//コンストラクタ
//============================
ResultUI::ResultUI()
{
	resultBG = new ResultBG;
	uiCharacter01 = new UIcharacter;//プレイヤー1
	uiCharacter02 = new UIcharacter;;//プレイヤー2
}

//============================
//デストラクタ
//============================
ResultUI::~ResultUI()
{
	delete resultBG;
	delete uiCharacter01;
	delete uiCharacter02;
}

//============================
//初期化
//============================
void ResultUI::initialize()
{

	//BGM再生フラグをtrueで初期化
	playedBGM = true;

	//フェイズ移行時間の初期化
	time = 0;

	//フェイズの初期化(第一フェイズで)
	resultPhase = PHASE_01;

	//全体緑化率
	greenigPersent = 0;

	//緑化本数
	greeningNum01 = 0;
	greeningNum02 = 0;

	//撃破数
	defeat01 = 0;
	defeat02 = 0;

	//ランクの確定(仮)
	score01 = greenigPersent + greeningNum01/10 + defeat01;//全体緑化率+緑化本数割る１０+撃破数(仮）
	score02 = greenigPersent + greeningNum02/10 + defeat02;//全体緑化率+緑化本数割る１０+撃破数(仮）

	//**********************************************
	//70以下でFAILED,70以上８０以下CLEARE         //
	//80以上90以下GREAT,90以上でEXCELLENT         //
	//**********************************************
	//プレイヤー１のランクの確定
	if (score01 < 70)
	{
		rank01 = uiRankNS::UIRANK_TYPE::FAILED;
	}
	else if (score01 >= 70 && score01 < 80)
	{
		rank01 = uiRankNS::UIRANK_TYPE::CLEARE;
	}
	else if (score01 >= 70 && score01 < 90)
	{
		rank01 = uiRankNS::UIRANK_TYPE::GREAT;
	}
	else if(score01 >= 90)
	{
		rank01 = uiRankNS::UIRANK_TYPE::EXCELLENT;
	}
	
	//プレイヤー2のランクの確定
	if (score02 < 70)
	{
		rank02 = uiRankNS::UIRANK_TYPE::FAILED;
	}
	else if (score02 >= 70 && score02 < 80)
	{
		rank02 = uiRankNS::UIRANK_TYPE::CLEARE;
	}
	else if (score02 >= 70 && score02 < 90)
	{
		rank02 = uiRankNS::UIRANK_TYPE::GREAT;
	}
	else if (score02 >= 90)
	{
		rank02 = uiRankNS::UIRANK_TYPE::EXCELLENT;
	}

	//文字UIの初期化
	uiCharacter01->initialize(PLAYER_01);//プレイヤー1
	uiCharacter02->initialize(PLAYER_02);//プレイヤー2

	//テクスチャUIの初期化
	uiTexture.initialize();
	
	//ランクUIの初期化
	uiRank01.initialize(rank01, PLAYER_01);	//プレイヤー１
	uiRank02.initialize(rank02, PLAYER_02);	//プレイヤー2

	//数字の種類分だけ初期化
	for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX ; i++)
	{
		uiNumber[i].initialize(i);

	}

	//リザルト背景の初期化
	resultBG->initialize();
}

//============================
//描画
//ここもめっちゃk持ち悪い
//============================
void ResultUI::render()
{
	resultBG->render();					//リザルト背景の描画
	uiTexture.render(resultPhase);		//テクスチャの描画
	uiCharacter01->render(resultPhase);	//プレイヤー１の文字描画
	uiCharacter02->render(resultPhase);	//プレイヤー2の文字描画

	//全体緑化率の描画
	if (resultPhase == PHASE_02)
	{
		uiNumber[uiNumberNS::GREENIG_PERSENT].render();
	}

	if (resultPhase == PHASE_03)
	{
		uiNumber[uiNumberNS::GREENIG_PERSENT].render();
	}
	//数字表示はフェイズ４から描画
	if (resultPhase == PHASE_04)
	{
		//数字の表示
		for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX; i++)
		{
			uiNumber[i].render();
		}
	}
	//ランク表示
	if (resultPhase == PHASE_05)
	{
		uiRank01.render(rank01);		//プレイヤー１のランク描画
		uiRank02.render(rank02);		//プレイヤー２のランク描画
										//数字の表示
		for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX; i++)
		{
			uiNumber[i].render();
			
		}
	}
}

//============================
//更新
//フレームタイムをもたせて一定時間経過でフェイズを更新
//============================
void ResultUI::update(float flameTime)
{
	time += flameTime;

	//フェイズの更新
	if (time > 2.0f)
	{
		resultPhase = PHASE_02;
		uiNumber[uiNumberNS::GREENIG_PERSENT].update(greenigPersent);
	}
	if (time > 5.0f)
	{
		resultPhase= PHASE_03;
	}
	if (time > 10.0f)
	{
		resultPhase = PHASE_04;
		//数字
		int score[uiNumberNS::GREENIG_PERSENT] = { greeningNum01,greeningNum02,defeat01,defeat02 };
		for (int i = 0; i < uiNumberNS::GREENIG_PERSENT; i++)
		{
			uiNumber[i].update(score[i]);
		}
		
	}
	if (time > 12.0f)
	{
		resultPhase = PHASE_05;
		//ランク
		uiRank01.update(rank01);
		uiRank02.update(rank02);
		//再生
		if (playedBGM)
		{
			decidionBGM();
		}
	}
	
	uiCharacter01->update(resultPhase,PLAYER_01);	//プレイヤー１の文字更新
	uiCharacter02->update(resultPhase,PLAYER_02);	//プレイヤー１の文字更新
	uiTexture.update(resultPhase);		//テクスチャの更新
}

//============================
//終了処理
//============================
void ResultUI::uninitialize()
{

	uiTexture.uninitialize();
	uiRank01.uninitialize(rank01);
	uiRank02.uninitialize(rank02);

	//数字の種類分だけ終了処理
	for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX; i++)
	{
		uiNumber[i].uninitialize();
	}
}

//=============================
//鳴らすBGMを決定し鳴らす処理
//=============================
void ResultUI::decidionBGM()
{
	//どちらかが70を超えていたならクリア
	if (score01 >=70 || score02 >= 70)
	{
		PLAY_PARAMETERS playParameters = { 0 };//同時に再生したい数
		//再生する曲の指定サウンドID,ループ,スピードNULLでしない,基本false,基本NULL,フィルターを使うか使わないか
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Clear, true,1.0f,false,NULL};//BGMの設定
		 //再生
		SoundInterface::BGM->playSound(&playParameters);
	}
	//どちらも70以下なら失敗
	else if(score01<70 && score02<70)
	{
		PLAY_PARAMETERS playParameters = { 0 };//同時に再生したい数
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Failed, true,1.0f,false,NULL};//BGMの設定
		SoundInterface::BGM->playSound(&playParameters);//再生
	}
	//1度再生したらフラグをフォルス
	playedBGM = false;
}