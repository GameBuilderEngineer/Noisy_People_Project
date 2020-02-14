//==================================
//マーカークラス[Mark.h]
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
namespace markNS
{
	const int WIDTH_MARK = (512 * WINDOW_WIDTH / 1920);	//キャラアイコンの横幅
	const int HEIGHT_MARK = (32 * WINDOW_HEIGHT / 1080);	//キャラアイコンの縦幅
	const D3DXVECTOR3 POSITION_MARK = D3DXVECTOR3(348 * WINDOW_WIDTH / 1920, 85 * WINDOW_HEIGHT / 1080, 0);	//キャラアイコンの表示位置(仮)
	const D3DXVECTOR3 POSITION_MARK_P2 = D3DXVECTOR3(1800 * WINDOW_WIDTH / 1920, 85 * WINDOW_HEIGHT / 1080, 0);	//キャラアイコンの表示位置(仮)
}

//==================================
//クラス定義
//==================================
class Mark :public BasicUI
{
public://メンバー変数
	Sprite * mark;	//スプライト

public://メンバー関数
	Mark();			//コンストラクタ
	~Mark();		//デストラクタ
	void initialize(int playerType);//初期化
	void render();			//描画

};

