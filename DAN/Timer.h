//====================================
//タイマー処理[Timer.h]
//染矢　晃介
//====================================
#pragma once
//====================================
//インクルード
//====================================
#include "BasicUI.h"
#include "GameMaster.h"
#include "VirtualController.h"

//====================================
//名前空間
//====================================
namespace timerNS
{
	//タイマー
	const int WIDTH_TIMER = (60 * WINDOW_WIDTH / 1920);		//各数字の横幅
	const int HEIGHT_TIMER = (60 * WINDOW_HEIGHT / 1080);		//各数字の縦幅
	const int DIGIT_TIMER = (3);		//タイマーの桁数
	const D3DXVECTOR3 POSITION_TIMER = D3DXVECTOR3(1015 * WINDOW_WIDTH / 1920, 100 * WINDOW_HEIGHT / 1080, 0);	//分タイマーの表示位置(仮)
	const D3DXVECTOR3 ROTATION_TIMER = D3DXVECTOR3(0, 0, 0);		//回転
	const D3DCOLOR COLOR_TIMER = D3DCOLOR_RGBA(255, 255, 255, 255);	//色
	const int      GAME_TIME = 600;									//ゲーム時間
}

//====================================
//クラス定義
//====================================
class Timer:public BasicUI,public GameMaster
{
private://メンバー変数
	Sprite *timer[timerNS::DIGIT_TIMER];
	float		 keepTime;							//時間管理用

public://メンバー関数
	Timer();				//コンストラクタ
	~Timer();				//デストラクタ
	void initialize();		//初期化
	void render();			//描画処理
	void update();			//更新処理
	void uninitialize();	//終了処理
	void setUV(int time);	//uv座標の設定
};
