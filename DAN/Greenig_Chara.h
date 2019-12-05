//=========================================
// 緑化本数文字クラス
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
namespace greenig_CharaNS
{
	//緑化本数(英語)
	const int WIDTH_GREENIG = (512 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_GREENIG = (64 * WINDOW_HEIGHT / 1080);		//高さ
	const D3DXVECTOR3 POSITION_GREENIG_01 = D3DXVECTOR3(280 * WINDOW_WIDTH / 1920, 3500, 0);//プレイヤー１用初期位置
	const D3DXVECTOR3 POSITION_GREENIG_02 = D3DXVECTOR3(1235 * WINDOW_WIDTH / 1920, 3500, 0);//プレイヤー２用初期位置
	const float END_POS_GREENING = (353.0f* WINDOW_HEIGHT / 1080);//最終位置のy座標

}

//=========================================
//クラス定義
//=========================================
class Greening_Chara : public BasicUI
{
public://メンバー変数
	Sprite * greening;
private:
public:
	Greening_Chara();		//コンストラクタ
	~Greening_Chara();	//デストラクタ
	void initialize(int playerType);
	void render();
	void update(int playerType);
private:
};
