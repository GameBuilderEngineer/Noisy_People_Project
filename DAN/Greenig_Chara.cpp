//==========================================
//緑化本数文字処理
//染矢晃介
//==========================================

//==========================================
//インクルード
//==========================================
#include "Greenig_chara.h"

//==========================================
// using宣言
//==========================================
using namespace greenig_CharaNS;
using namespace basicUiNS;

//==========================================
//コンストラクタ
//==========================================
Greening_Chara::Greening_Chara()
{
	greening = new Sprite;
}

//==========================================
//デストラクタ
//==========================================
Greening_Chara::~Greening_Chara()
{
	delete greening;
}

//==========================================
//初期化
//==========================================
void Greening_Chara::initialize(int playerType)
{
	switch (playerType)
	{
	case P1:
		BasicUI::assingSize(WIDTH_GREENIG, HEIGHT_GREENIG);
		BasicUI::assingPosition(POSITION_GREENIG_01);
		BasicUI::initialize(greening, textureNS::reference(textureNS::RESULT_GREEN_NUM));
		break;

	case P2:
		BasicUI::assingSize(WIDTH_GREENIG, HEIGHT_GREENIG);
		BasicUI::assingPosition(POSITION_GREENIG_02);
		BasicUI::initialize(greening, textureNS::reference(textureNS::RESULT_GREEN_NUM));
		break;
	}

}

//==========================================
//描画
//==========================================
void Greening_Chara::render()
{
	greening->render();
}

//==========================================
//更新
//==========================================
void Greening_Chara::update(int playerType, float flametime)
{
	if (BasicUI::position.y > END_POS_GREENING)
	{
		BasicUI::changePostion(greening, slideUp,flametime);
	}
}