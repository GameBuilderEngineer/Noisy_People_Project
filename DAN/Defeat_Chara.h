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
namespace defeat_CharaNS
{
	//撃退(英語)
	const int WIDTH_DEFEAT = (390 * WINDOW_WIDTH / 1920);			//幅
	const int HEIGHT_DEFEAT = (53 * WINDOW_HEIGHT / 1080);			//高さ
	const D3DXVECTOR3 POSITION_DEFEAT_01 = D3DXVECTOR3(273 * WINDOW_WIDTH / 1920, 5500, 0);//プレイヤー１用初期位置
	const D3DXVECTOR3 POSITION_DEFEAT_02 = D3DXVECTOR3(1218 * WINDOW_WIDTH / 1920, 5500, 0);//プレイヤー２用初期位置
	const float END_POS_DEFEAT = (523.0f * WINDOW_HEIGHT / 1080);	//最終位置のy座標

}

//=========================================
//クラス定義
//=========================================
class Defeat_Chara : public BasicUI
{
public://メンバー変数
	Sprite * defeat;
private:
public:
	Defeat_Chara();		//コンストラクタ
	~Defeat_Chara();	//デストラクタ
	void initialize(int playerType);
	void render();
	void update(int playerType,float flametime);
private:
};
