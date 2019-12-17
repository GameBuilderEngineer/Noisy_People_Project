//=========================================
//緑化率クラス[Green.h]
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
namespace greenNS
{
	const int WIDTH_GREEN = (512 * WINDOW_WIDTH / 1920);	//電力タンクの横幅
	const int HEIGHT_GREEN = (64 * WINDOW_HEIGHT / 1080);//電力タンクの縦幅
	const D3DXVECTOR3 GREEN_POSTION = D3DXVECTOR3(220.0f * WINDOW_WIDTH / 1920, 190.0f * WINDOW_HEIGHT / 1080, 0.0f);//タンクのポジション
	const D3DXVECTOR3 GREEN_POSTION_P2 = D3DXVECTOR3(1690.0f * WINDOW_WIDTH / 1920, 190.0f * WINDOW_HEIGHT / 1080, 0.0f);//タンクのポジション
	const float ALPHA_MAX = 255.0f;
	const float ALPHA_MIN = 0.0f;
}

//=========================================
//クラス定義
//=========================================
class Green : public BasicUI
{
public://メンバー変数
	Sprite * green;
	
private:
public://メンバー関数
	Green();
	~Green();
	void initialize(int playerType);
	void render(int state);
	
private:
};