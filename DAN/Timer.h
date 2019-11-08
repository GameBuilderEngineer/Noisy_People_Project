//====================================
//タイマー処理[Timer.h]
//染矢　晃介
//====================================
#pragma once

//====================================
//インクルード
//====================================
#include "Sprite.h"
#include "TextureLoader.h"

//====================================
//名前空間
//====================================
namespace timerNS
{
	//タイマー
	const int WIDTH_TIMER = (100);		//各数字の横幅
	const int HEIGHT_TIMER = (100);		//各数字の縦幅
	const int DIGIT_TIMER = (4);		//タイマーの桁数
	const D3DXVECTOR3 POSITION_TIMER = D3DXVECTOR3(1000, 100, 0);	//タイマーの表示位置(仮)
	const D3DXVECTOR3 ROTATION_TIMER = D3DXVECTOR3(0, 0, 0);		//回転
	const D3DCOLOR COLOR_TIMER = D3DCOLOR_RGBA(255, 255, 255, 255);	//色

}
//====================================
//クラス定義
//====================================
class Timer
{
private://メンバー変数
	Sprite *timer[timerNS::DIGIT_TIMER];
	D3DXVECTOR3 pos[timerNS::DIGIT_TIMER];	//表示位置管理用
	float		 keepTime;						//時間管理用
	//uv座標用の変数
	D3DXVECTOR2 uvCoord01;
	D3DXVECTOR2 uvCoord02;
	D3DXVECTOR2 uvCoord03;
	D3DXVECTOR2 uvCoord04;
	

public://メンバー関数
	Timer();				//コンストラクタ
	~Timer();				//デストラクタ
	void initialize();		//初期化
	void render();			//描画処理
	void update();			//更新処理
	void uninitialize();	//終了処理
	void setUV(int time);	//uv座標の設定
	void assingUV();		//uv座標の代入

};
