//=========================================
// PLAYER文字クラス
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
namespace player_CharaNS
{
	//プレイヤー
	const int WIDTH_PLAYER = (345 * WINDOW_WIDTH / 1920);			//幅
	const int HEIGHT_PLAYER = (90 * WINDOW_HEIGHT / 1080);			//高さ
	const D3DXVECTOR3 POSITION_PLAYER = D3DXVECTOR3(3800, 250 * WINDOW_HEIGHT / 1080, 0);//初期位置
	const float END_POS_PLAYER_01 = (345.0f* WINDOW_WIDTH / 1920);//プレイヤー１の最終位置のx座標
	const float END_POS_PLAYER_02 = (1605 * WINDOW_WIDTH / 1920);	 //プレイヤー２の最終位置のx座標

	//uv座標
	const D3DXVECTOR2 PLAYER1_UV_VERTEX01 = D3DXVECTOR2(0.0, 0.0);
	const D3DXVECTOR2 PLAYER1_UV_VERTEX02 = D3DXVECTOR2(1.0, 0.0);
	const D3DXVECTOR2 PLAYER1_UV_VERTEX03 = D3DXVECTOR2(0.0, 0.5);
	const D3DXVECTOR2 PLAYER1_UV_VERTEX04 = D3DXVECTOR2(1.0, 0.5);

	const D3DXVECTOR2 PLAYER2_UV_VERTEX01 = D3DXVECTOR2(0.0, 0.5);
	const D3DXVECTOR2 PLAYER2_UV_VERTEX02 = D3DXVECTOR2(1.0, 0.5);
	const D3DXVECTOR2 PLAYER2_UV_VERTEX03 = D3DXVECTOR2(0.0, 1.0);
	const D3DXVECTOR2 PLAYER2_UV_VERTEX04 = D3DXVECTOR2(1.0, 1.0);
}

//=========================================
//クラス定義
//=========================================
class Player_Chara : public BasicUI
{
public://メンバー変数
	Sprite * player;
private:
public:
	Player_Chara();		//コンストラクタ
	~Player_Chara();	//デストラクタ
	void initialize(int playerType);
	void render();
	void update(int playerType);
private:
};