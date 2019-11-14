//==================================
//キャラアイコンクラス[CharaIcon.h]
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
namespace charaIconNS
{
	const int WIDTH_ICON = (123);	//キャラアイコンの横幅
	const int HEIGHT_ICON = (123);	//キャラアイコンの縦幅
	const D3DXVECTOR3 POSITION_ICON = D3DXVECTOR3(85, 96, 0);	//キャラアイコンの表示位置(仮)
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
	void initialize();		//初期化
	void render();			//描画

};
