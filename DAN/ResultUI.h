//============================
// [ResultUI.h]
// 染矢　晃介
//============================

//============================
//インクルード
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"


//============================
//名前空間 定数
//============================
namespace resultUiNS
{
	//UIの種類
	enum RESULTUI_TYPE
	{
		RESULT,
		LINE,
		FLAME,
		CHARA_A,
		CHARA_B,
		PLAYER,
		GREENIG,
		RYOKUKA,
		PERSENT,
		NUMBER,
		DEFEAT,
		GEKITAI,
		EXCELLENT,
		GREAT,
		CLEAR,
		FAIL,
		RESULTUI_MAX,	//UIの総数
	};

	//リザルトのフェイズ
	enum RESULT_PHASE
	{
		PHASE_01,		//リザルトロゴのスライドイン
		PHASE_02,		//島表示
		PHASE_03,		//結果表示(数字など)
		PHASE_04,		//タイトルに戻るフェイズ
		PHASE_MAX,		//フェイズの総数
	};

	//リザルトロゴ
	const int WIDTH_RESULT = (330);			//幅
	const int HEIGHT_RESULT = (170);		//高さ
	const D3DXVECTOR3 POSITION_RESULT = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//フレーム
	const int WIDTH_FLAME = (860);			//幅
	const int HEIGHT_FLAME = (755);			//高さ
	const D3DXVECTOR3 POSITION_FLAME = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//ライン
	const int WIDTH_LINE = (1920);			//幅
	const int HEIGHT_LINE = (15);			//高さ
	const D3DXVECTOR3 POSITION_LINE = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//キャラA
	const int WIDTH_CHARA_A = (478);		//幅
	const int HEIGHT_CHARA_A = (624);		//高さ
	const D3DXVECTOR3 POSITION_CHARA_A = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//キャラB
	const int WIDTH_CHARA_B = (478);		//幅
	const int HEIGHT_CHARA_B = (624);		//高さ
	const D3DXVECTOR3 POSITION_CHARA_B = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//プレイヤー
	const int WIDTH_PLAYER = (345);			//幅
	const int HEIGHT_PLAYER = (180);		//高さ
	const D3DXVECTOR3 POSITION_PLAYER = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//緑化(英語)
	const int WIDTH_GREENIG = (334);		//幅
	const int HEIGHT_GREENIG = (53);		//高さ
	const D3DXVECTOR3 POSITION_GREENIG = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//緑化
	const int WIDTH_RYOKUKA = (256);		//幅
	const int HEIGHT_RYOKUKA = (86);		//高さ
	const D3DXVECTOR3 POSITION_RYOKUKA = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//パーセント
	const int WIDTH_PERSENT = (115);		//幅
	const int HEIGHT_PERSENT = (124);		//高さ
	const D3DXVECTOR3 POSITION_PERSENT = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//数字
	const int WIDTH_NUMBER = (115);			//幅
	const int HEIGHT_NUMBER = (124);		//高さ
	const D3DXVECTOR3 POSITION_NUMBER = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//撃退(英語)
	const int WIDTH_DEFEAT = (390);			//幅
	const int HEIGHT_DEFEAT = (53);			//高さ
	const D3DXVECTOR3 POSITION_DEFEAT = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//撃退
	const int WIDTH_GEKITAI = (255);			//幅
	const int HEIGHT_GEKITAI = (86);			//高さ
	const D3DXVECTOR3 POSITION_GEKITAI = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//エクセレント
	const int WIDTH_EXCELLENT = (740);		//幅
	const int HEIGHT_EXCELLENT = (331);		//高さ
	const D3DXVECTOR3 POSITION_EXCELLENT = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//グレイト
	const int WIDTH_GREAT = (740);			//幅
	const int HEIGHT_GREAT = (331);			//高さ
	const D3DXVECTOR3 POSITION_GREAT = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//クリア
	const int WIDTH_CLEAR = (740);			//幅
	const int HEIGHT_CLEAR = (331);			//高さ
	const D3DXVECTOR3 POSITION_CLEAR = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//失敗
	const int WIDTH_FAIL = (740);			//幅
	const int HEIGHT_FAIL = (331);			//高さ
	const D3DXVECTOR3 POSITION_FAIL = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//位置

	//共通
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0, 0, 0);			//回転
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//色

}
//============================
//クラス定義
//============================
class ResultUI
{
public: //メンバー変数
	Sprite * resultUI[resultUiNS::RESULTUI_MAX];	//スプライト
	int resultPhase;								//フェイズ管理用

public: //メンバー関数
	void initialize();			//初期化
	void render();				//描画
	void update(Input *input);	//更新
	void uninitialize();		//終了
	void renderPhase();

};

