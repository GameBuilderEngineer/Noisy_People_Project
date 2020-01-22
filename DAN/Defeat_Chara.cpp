//==========================================
//撃退数文字処理
//染矢晃介
//==========================================

//==========================================
//インクルード
//==========================================
#include "Defeat_chara.h"

//==========================================
// using宣言
//==========================================
using namespace defeat_CharaNS;
using namespace basicUiNS;

//==========================================
//コンストラクタ
//==========================================
Defeat_Chara::Defeat_Chara()
{
	defeat = new Sprite;
}

//==========================================
//デストラクタ
//==========================================
Defeat_Chara::~Defeat_Chara()
{
	delete defeat;
}

//==========================================
//初期化
//==========================================
void Defeat_Chara::initialize(int playerType)
{
	switch (playerType)
	{
	case P1:
		BasicUI::assingSize(WIDTH_DEFEAT, HEIGHT_DEFEAT);
		BasicUI::assingPosition(POSITION_DEFEAT_01);
		BasicUI::initialize(defeat, textureNS::reference(textureNS::RESULT_DEFEAT));
		break;

	case P2:
		BasicUI::assingSize(WIDTH_DEFEAT, HEIGHT_DEFEAT);
		BasicUI::assingPosition(POSITION_DEFEAT_02);
		BasicUI::initialize(defeat, textureNS::reference(textureNS::RESULT_DEFEAT));
		break;
	}

}

//==========================================
//描画
//==========================================
void Defeat_Chara::render()
{
	defeat->render();
}

//==========================================
//更新
//==========================================
void Defeat_Chara::update(int playerType,float flametime)
{
	if (BasicUI::position.y > END_POS_DEFEAT)
	{
		BasicUI::changePostion(defeat, slideUp,flametime);
	}
}