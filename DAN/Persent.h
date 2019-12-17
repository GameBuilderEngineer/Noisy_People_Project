//========================================
//パーセントクラス[ResultBG.h]
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
namespace persentNS
{
	const int WIDTH_PERSENT = (115*WINDOW_WIDTH/1920);//横幅
	const int HEIGHT_PERSENT = (124*WINDOW_HEIGHT/1080);//縦幅
	const D3DXVECTOR3 PERSNT_POSITION = D3DXVECTOR3(1250 * WINDOW_WIDTH / 1920, 220 *WINDOW_HEIGHT / 1080, 0);
}

//========================================
//クラス定義
//========================================
class Persent :public BasicUI
{
public://メンバー変数
	Sprite * persent;		//スプライト
private:
public://メンバー関数
	Persent();				//コンストラクタ
	~Persent();				//デストラクタ
	void initialize();		//初期化
	void render();			//描画
	void update();			//更新
private:
};
