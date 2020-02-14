//=========================================
// 全体緑化文字クラス
//染矢晃介
//=========================================
#pragma once

//=========================================
//インクルード
//=========================================
#include "BasicUI.h"

//=========================================
//名前空間
//=========================================
namespace wholeGreenig_CharaNS
{
	//緑化本数(英語)
	const int WIDTH_GREENIG = (256 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_GREENIG = (128 * WINDOW_HEIGHT / 1080);		//高さ
	const D3DXVECTOR3 POSITION_GREENIG = D3DXVECTOR3(750 * WINDOW_WIDTH / 1920, 220 * WINDOW_HEIGHT / 1080, 0);//プレイヤー１用初期位置

}

//=========================================
//クラス定義
//=========================================
class WholeGreening : public BasicUI
{
public://メンバー変数
	Sprite * greening;
private:
public:
	WholeGreening();		//コンストラクタ
	~WholeGreening();	//デストラクタ
	void initialize();
	void render();
	void update();
private:
};

