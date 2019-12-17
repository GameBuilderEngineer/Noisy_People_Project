//====================================
//緑化率処理[GreenRate.h]
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
namespace greenRateNS
{
	//タイマー
	const int WIDTH_GREEN_RATE = (30 * WINDOW_WIDTH / 1920);		//各数字の横幅
	const int HEIGHT_GREEN_RATE = (30 * WINDOW_HEIGHT / 1080);		//各数字の縦幅
	const int DIGIT_GREEN_RATE = (3);		//タイマーの桁数
	const D3DXVECTOR3 POSITION_GREEN_RATE = D3DXVECTOR3(340 * WINDOW_WIDTH / 1920, 200 * WINDOW_HEIGHT / 1080, 0);	//分タイマーの表示位置(仮)
	const D3DXVECTOR3 POSITION_GREEN_RATE_P2 = D3DXVECTOR3(1820 * WINDOW_WIDTH / 1920, 200 * WINDOW_HEIGHT / 1080, 0);	//分タイマーの表示位置(仮)
	const D3DXVECTOR3 ROTATION_GREEN_RATE = D3DXVECTOR3(0, 0, 0);		//回転
	const D3DCOLOR COLOR_GREEN_RATE = D3DCOLOR_RGBA(255, 255, 255, 255);	//色
}

//====================================
//クラス定義
//====================================
class GreenRate :public BasicUI, public GameMaster
{
private://メンバー変数
	Sprite * timer[greenRateNS::DIGIT_GREEN_RATE];
	float		 keepRate;//管理用

public://メンバー関数
	GreenRate();//コンストラクタ
	~GreenRate();//デストラクタ
	void initialize(int playerType);//初期化
	void render(int playerType);//描画処理
	void update(float gametime);//更新処理
	void uninitialize();//終了処理
	void setUV(int time);//uv座標の設定
};

