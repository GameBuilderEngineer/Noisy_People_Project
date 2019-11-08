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
	const int WIDTH_BUTTON = (1920);	//ボタンUIの横幅
	const int HEIGHT_BUTTON = (169);	//ボタンUIの縦幅
	const D3DXVECTOR3 POSITION_BUTTON = D3DXVECTOR3(960, 84, 0);	//ボタンUIの表示位置(仮)
}

//==================================
//クラス定義
//==================================
class UiButton :public BasicUI
{
public://メンバー変数
	Sprite * button[uiButtonNS::TYPE_MAX];	//スプライト

public://メンバー関数
	UiButton();			//コンストラクタ
	~UiButton();			//デストラクタ
	void initialize();		//初期化
	void render();			//描画

}; 
