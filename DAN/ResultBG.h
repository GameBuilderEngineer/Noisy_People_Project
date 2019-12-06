//========================================
//リザルトBGクラス[ResultBG.h]
//染矢晃介
//========================================
#pragma once

//========================================
//インクルード
//========================================
#include "BasicUI.h"

//========================================
//名前空間
//========================================
namespace resultBGNS
{
	const int WIDTH_BG = WINDOW_WIDTH;//横幅
	const int HEIGHT_BG = WINDOW_HEIGHT;//縦幅
	const D3DXVECTOR3 BG_POSITION = D3DXVECTOR3(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0);

}

//========================================
//クラス定義
//========================================
class ResultBG :public BasicUI
{
public://メンバー変数
	Sprite * resultBG;		//スプライト
private:
public://メンバー関数
	ResultBG();				//コンストラクタ
	~ResultBG();			//デストラクタ
	void initialize();		//初期化
	void render();			//描画
	void update();			//更新
private:
};