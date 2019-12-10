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
namespace greenig_MojiNS
{
	//緑化
	const int WIDTH_RYOKUKA = (512 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_RYOKUKA = (128 * WINDOW_HEIGHT / 1080);		//高さ
	const D3DXVECTOR3 POSITION_RYOKUKA_01 = D3DXVECTOR3(340 * WINDOW_WIDTH / 1920, 4500, 0);//プレイヤー１用初期位置
	const D3DXVECTOR3 POSITION_RYOKUKA_02 = D3DXVECTOR3(1290 * WINDOW_WIDTH / 1920, 4500, 0);//プレイヤー２用初期位置
	const float END_POS_RYOKUKA = (425.0f * WINDOW_HEIGHT / 1080);	//最終位置のy座標

}

//=========================================
//クラス定義
//=========================================
class Greening_Moji : public BasicUI
{
public://メンバー変数
	Sprite * greening;
private:
public:
	Greening_Moji();		//コンストラクタ
	~Greening_Moji();	//デストラクタ
	void initialize(int playerType);
	void render();
	void update(int playerType);
private:
};
