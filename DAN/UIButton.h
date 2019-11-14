//==================================
//ボタンUI処理[TimerFlame.h]
//染矢　晃介
//==================================
#pragma once

//==================================
//インクルード
//==================================
#include "BasicUI.h"

//==================================
//名前空間
//==================================
namespace uiButtonNS
{
	enum BUTTON_UI_TYPE
	{
		SV,
		SHOT,
		JUMP,
		VISION,
		TYPE_MAX,
	};
	const int WIDTH_BUTTON = (140);	//ボタンUIの横幅
	const int HEIGHT_BUTTON = (40);	//ボタンUIの縦幅
	const D3DXVECTOR3 POSITION_BUTTON = D3DXVECTOR3(100, 900, 0);	//ボタンUIの表示位置(仮)
	const D3DXVECTOR3 POSITION_BUTTON2 = D3DXVECTOR3(1800, 900, 0);	//ボタンUIの表示位置(仮)
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 100);
	const float ALPHA_MAX = 255.0f;
	const float ALPHA_MINIMAM = 100.0f;
}

//==================================
//クラス定義
//==================================
class UiButton :public BasicUI
{
public://メンバー変数
	Sprite * button[uiButtonNS::VISION];	//スプライト
	float	alpha;							//α値

public://メンバー関数
	UiButton();					//コンストラクタ
	~UiButton();				//デストラクタ
	void initialize();			//初期化
	void renderP1();			//P1描画
	void renderP2();			//P2描画
	void update(bool flag, int buttontype);	//更新
	void  changeAlpha(bool flag, int buttontype);//α値の変更
	

}; 
