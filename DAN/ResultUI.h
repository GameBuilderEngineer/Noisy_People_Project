//============================
// [ResultUI.h]
// 染矢　晃介
//============================
#pragma once
//============================
//インクルード
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"
#include "UIcharacter.h"
#include "UItex.h"
#include "UIrank.h"
#include "UInumber.h"
#include "ResultBG.h"
#include "Player.h"

//============================
//名前空間 定数
//============================
namespace resultUiNS
{
	
	//リザルトのフェイズ
	enum RESULT_PHASE
	{
		PHASE_01,		//リザルトロゴのスライドイン
		PHASE_02,		//島表示
		PHASE_03,		//文字表示
		PHASE_04,		//数字のカウントアップ
		PHASE_05,		//ランクの表示
		PHASE_MAX,		//フェイズの総数
	};

	//プレイヤーの種類
	enum PLAYER_TYPE
	{
		PLAYER_01,
		PLAYER_02,
		PLAYER_MAX,
	};

}
//============================
//クラス定義
//============================
class ResultUI
{
public: //メンバー変数
	UIcharacter* uiCharacter01;		//プレイヤー１用文字のUI
	UIcharacter* uiCharacter02;		//プレイヤー2用文字のUI
	UItexture	uiTexture;			//テクスチャのUI
	UIrank*     uiRank;			//プレイヤー１のランクのUI
	UIrank      uiRank02;			//プレイヤー2のランクのUI
	UInumber	uiNumber[uiNumberNS::NUMBER_TYPE_MAX];//数字
	ResultBG*   resultBG;			//リザルト背景
	Player*		player;				//プレイヤーの情報
	int			score01;			//プレイヤー１のスコア保存用
	int			score02;			//プレイヤー２のスコア保存用
	int			greenigPersent;		//全体緑化率
	int         defeat01;			//撃破数
	int         defeat02;			//撃破数
	int			greeningNum01;		//緑化本数
	int			greeningNum02;		//緑化本数
	int			resultPhase;		//フェイズ管理用
	float		time;				//フェイズ移行する時間
	int			rank01;				//プレイヤー１のランク管理用
	int         rank02;				//プレイヤー2のランク管理用
	bool		playedBGM;			//BGM再生用フラグ
public: //メンバー関数
	ResultUI();						//コンストラクタ
	~ResultUI();					//デストラクタ
	void initialize(/*Player *player*/);//初期化
	void render();					//描画
	void update(float flameTime);	//更新
	void uninitialize();			//終了
	void decidionBGM();
	int decisionRank(int greening, int greenNum, int defeat);
	
};

