//==========================================
//リザルト文字処理
//染矢晃介
//==========================================

//==========================================
//インクルード
//==========================================
#include "Player_chara.h"

//==========================================
// using宣言
//==========================================
using namespace player_CharaNS;
using namespace basicUiNS;

//==========================================
//コンストラクタ
//==========================================
Player_Chara::Player_Chara()
{
	player = new Sprite;
}

//==========================================
//デストラクタ
//==========================================
Player_Chara::~Player_Chara()
{
	delete player;
}

//==========================================
//初期化
//==========================================
void Player_Chara::initialize(int playerType)
{
	switch (playerType)
	{
	case P1:
		BasicUI::assingSize(WIDTH_PLAYER, HEIGHT_PLAYER);
		BasicUI::assingPosition(POSITION_PLAYER);
		BasicUI::initialize(player, textureNS::reference(textureNS::RESULT_PLAYER));
		break;

	case P2:
		BasicUI::assingSize(WIDTH_PLAYER, HEIGHT_PLAYER);
		BasicUI::assingPosition(POSITION_PLAYER);
		BasicUI::initialize(player, textureNS::reference(textureNS::RESULT_PLAYER));
		break;

	default:
		break;
	}

}

//==========================================
//描画
//==========================================
void Player_Chara::render()
{
	player->render();
}

//==========================================
//更新
//==========================================
void Player_Chara::update(int playerType, float flametime)
{
	switch (playerType)
	{
	case P1:
		BasicUI::changeUV(player,PLAYER1_UV_VERTEX01, PLAYER1_UV_VERTEX02, PLAYER1_UV_VERTEX03, PLAYER1_UV_VERTEX04);
		if (BasicUI::position.x > END_POS_PLAYER_01)
		{
			BasicUI::changePostion(player, slideLeft,flametime);
		}
		break;

	case P2:
		BasicUI::changeUV(player,PLAYER2_UV_VERTEX01, PLAYER2_UV_VERTEX02, PLAYER2_UV_VERTEX03, PLAYER2_UV_VERTEX04);
		if (BasicUI::position.x > END_POS_PLAYER_02)
		{
			BasicUI::changePostion(player, slideLeft,flametime);
		}
		break;
	}
	
}