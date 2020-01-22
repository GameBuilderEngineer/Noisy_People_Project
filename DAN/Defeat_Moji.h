//=========================================
// 撃退数文字クラス
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
namespace defeat_MojiNS
{
	//撃退
	const int WIDTH_GEKITAI = (255 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_GEKITAI = (86 * WINDOW_HEIGHT / 1080);		//高さ
	const D3DXVECTOR3 POSITION_GEKITAI_01 = D3DXVECTOR3(200 * WINDOW_WIDTH / 1920, 6500, 0);//プレイヤー１用初期位置
	const D3DXVECTOR3 POSITION_GEKITAI_02 = D3DXVECTOR3(1150 * WINDOW_WIDTH / 1920, 6500, 0);//プレイヤー２用初期位置
	const float END_POS_GEKITAI = (595.0f* WINDOW_HEIGHT / 1080);	//最終位置のy座標

}

//=========================================
//クラス定義
//=========================================
class Defeat_Moji : public BasicUI
{
public://メンバー変数
	Sprite * defeat;
private:
public:
	Defeat_Moji();		//コンストラクタ
	~Defeat_Moji();	//デストラクタ
	void initialize(int playerType);
	void render();
	void update(int playerType, float flametime);
private:
};

