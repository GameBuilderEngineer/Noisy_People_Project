//=========================================
// RESULT文字クラス
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
namespace result_CharaNS
{
	//リザルトロゴ
	const int WIDTH_RESULT = (330 * WINDOW_WIDTH / 1920);			//幅
	const int HEIGHT_RESULT = (170 * WINDOW_HEIGHT / 1080);		//高さ
	const D3DXVECTOR3 POSITION_RESULT = D3DXVECTOR3(2100, 70 * WINDOW_HEIGHT / 1080, 0);//初期位置
	const float	END_POS_RESULT = (215.0f * WINDOW_WIDTH / 1920);	//最終位置のx座標
}

//=========================================
//クラス定義
//=========================================
class Result_Chara : public BasicUI
{
public://メンバー変数
	Sprite * result;
private:
public:
	Result_Chara();		//コンストラクタ
	~Result_Chara();	//デストラクタ
	void initialize();
	void render();
	void update(float flametime);
private:
};