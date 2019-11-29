//============================
//[UInumber.h]
// 染矢　晃介
//============================
#pragma once
//============================
//インクルード
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "VirtualController.h"

//============================
//名前空間 定数
//============================
namespace uiNumberNS
{
	enum NUMBER_TYPE
	{
		P1_GREENIG,			//P1用緑化率
		P2_GREENIG,			//P2用緑化率
		P1_DEFEAT,			//P1用撃退数
		P2_DEFEAT,			//P2用撃退数
		NUMBER_TYPE_MAX,
	};

	//数字の表示位置
	const int WIDTH_NUMBER = (100 * WINDOW_WIDTH / 1920);									//初期の幅
	const int HEIGHT_NUMBER = (100 * WINDOW_HEIGHT / 1080);								//初期の高さ
	const int DIGIT_NUMBER = 3;										//桁数
	const D3DXVECTOR3 P1_GREENIG_POS = D3DXVECTOR3(750 * WINDOW_WIDTH / 1920, 415 * WINDOW_HEIGHT / 1080, 0);	//P1用緑化率の表示位置
	const D3DXVECTOR3 P2_GREENIG_POS = D3DXVECTOR3(1710 * WINDOW_WIDTH / 1920, 415 * WINDOW_HEIGHT / 1080, 0);	//P1用緑化率の表示位置
	const D3DXVECTOR3 P1_DEFEAT_POS = D3DXVECTOR3(835 * WINDOW_WIDTH / 1920, 585 * WINDOW_HEIGHT / 1080, 0);		//P1用撃退数の表示位置
	const D3DXVECTOR3 P2_DEFEAT_POS = D3DXVECTOR3(1800 * WINDOW_WIDTH / 1920, 585 * WINDOW_HEIGHT / 1080, 0);	//P1用撃退数の表示位置

	//共通
	const D3DXVECTOR3 ROTATION_TEX = D3DXVECTOR3(0, 0, 0);			//回転
	const D3DCOLOR COLOR_TEX = D3DCOLOR_RGBA(255, 255, 255, 255);	//色
	const int SLIDE_SPEED = 20;										//スライドインスピード
	const float COUNT_TIME = 120.0f;										//カウントアップにかかる時間
}
//============================
//クラス定義
//============================
class UInumber
{
private: //メンバー変数
	Sprite * number[uiNumberNS::DIGIT_NUMBER];			//スプライトの作成
	D3DXVECTOR3    pos[uiNumberNS::DIGIT_NUMBER];		//表示位置管理用
	int      num;			//数字管理用
	float	 time;			//時間管理用
	float		 displayScore;	//表示するスコア
	int		 endScore;		//最終スコア
	int		 keepScore;		//保存用スコア
	int		 scored;
	D3DXVECTOR3 numberPos;	//表示位置
	D3DXVECTOR2 uvCoord01;	//uv座標管理用
	D3DXVECTOR2 uvCoord02;	//uv座標管理用
	D3DXVECTOR2 uvCoord03;	//uv座標管理用
	D3DXVECTOR2 uvCoord04;	//uv座標管理用

public: //メンバー関数
	void initialize(int numType);			//初期化
	void render();							//描画
	void update(int score);					//更新
	void uninitialize();					//終了
	void assingUV();						//uv座標の代入関数
	void assingPos();						//表示座標の代入関数
	void setNumber(int score);				//数字の設定用関数
	void countUp(int score);				//カウントアップ関数
};

