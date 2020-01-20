//==================================
//ボタンUI処理[ButtonUI.h]
//染矢　晃介
//==================================
#pragma once

//==================================
//インクルード
//==================================
#include "BasicUI.h"
#include "VirtualController.h"

//==================================
//名前空間
//==================================
namespace uiButtonNS
{
	enum BUTTON_UI_TYPE
	{
		SHOT,
		JUMP,
		VS,
		SV,
		SHIFT,
		TYPE_MAX,
	};

	enum STATE {
		NONE,
		OP,
		ED,
		NORMAL,
		VISION,
		SKY_VISION,
		DIGITAL_SHIFT,
		DEATH,
		STATE_NUM
	};

	const int WIDTH_BUTTON = (80 * WINDOW_WIDTH / 1920);	//ボタンUIの横幅
	const int HEIGHT_BUTTON = (80 * WINDOW_HEIGHT / 1080);	//ボタンUIの縦幅
	const D3DXVECTOR3 POSITION_BUTTON = D3DXVECTOR3(80 * WINDOW_WIDTH / 1920, 700 * WINDOW_HEIGHT / 1080, 0);	//ボタンUIの表示位置(仮)
	const D3DXVECTOR3 POSITION_BUTTON2 = D3DXVECTOR3(1800 * WINDOW_WIDTH / 1920, 700 * WINDOW_HEIGHT / 1080, 0);	//ボタンUIの表示位置(仮)
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);
	const float ALPHA_MAX = 255.0f;
	const float ALPHA_MINIMAM = 100.0f;
}

//==================================
//クラス定義
//==================================
class ButtonUI :public BasicUI
{
public://メンバー変数
	Sprite * button[uiButtonNS::TYPE_MAX];	//スプライト
	float	alpha;							//α値

public://メンバー関数
	ButtonUI();					//コンストラクタ
	~ButtonUI();				//デストラクタ
	void initialize();			//初期化
	void renderP1(int state);	//P1描画
	void renderP2(int state);	//P2描画
	void update(bool flag, int buttontype);	//更新
	void  changeAlpha(bool flag, int buttontype);//α値の変更
	void  changeRenderP1(int state);
	void  changeRenderP2(int state);
}; 
