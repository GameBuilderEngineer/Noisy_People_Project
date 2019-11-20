//==================================
//キャラアイコンクラス[CharaIcon.h]
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
namespace charaIconNS
{
	const int WIDTH_ICON = (123 * WINDOW_WIDTH / 1920);	//キャラアイコンの横幅
	const int HEIGHT_ICON = (123 * WINDOW_HEIGHT / 1080);	//キャラアイコンの縦幅
	const D3DXVECTOR3 POSITION_ICON = D3DXVECTOR3(85 * WINDOW_WIDTH / 1920, 96 * WINDOW_HEIGHT / 1080, 0);	//キャラアイコンの表示位置(仮)
	const D3DXVECTOR3 POSITION_ICON_P2 = D3DXVECTOR3(1840 * WINDOW_WIDTH / 1920, 96 * WINDOW_HEIGHT / 1080, 0);	//キャラアイコンの表示位置(仮)
}

//==================================
//クラス定義
//==================================
class CharaIcon :public BasicUI
{
public://メンバー変数
	Sprite * charaIcon;	//スプライト

public://メンバー関数
	CharaIcon();			//コンストラクタ
	~CharaIcon();			//デストラクタ
	void initialize(int playerType);//初期化
	void render();			//描画

};
