//==========================================
//緑化本数文字処理
//染矢晃介
//==========================================

//==========================================
//インクルード
//==========================================
#include "Greening_Moji.h"

//==========================================
// using宣言
//==========================================
using namespace greenig_MojiNS;
using namespace basicUiNS;

//==========================================
//コンストラクタ
//==========================================
Greening_Moji::Greening_Moji()
{
	greening = new Sprite;
}

//==========================================
//デストラクタ
//==========================================
Greening_Moji::~Greening_Moji()
{
	delete greening;
}

//==========================================
//初期化
//==========================================
void Greening_Moji::initialize(int playerType)
{
	switch (playerType)
	{
	case P1:
		BasicUI::assingSize(WIDTH_RYOKUKA, HEIGHT_RYOKUKA);
		BasicUI::assingPosition(POSITION_RYOKUKA_01);
		BasicUI::initialize(greening, textureNS::reference(textureNS::RESULT_RYOKUKA_KAZU));
		break;

	case P2:
		BasicUI::assingSize(WIDTH_RYOKUKA, HEIGHT_RYOKUKA);
		BasicUI::assingPosition(POSITION_RYOKUKA_02);
		BasicUI::initialize(greening, textureNS::reference(textureNS::RESULT_RYOKUKA_KAZU));
		break;
	}

}

//==========================================
//描画
//==========================================
void Greening_Moji::render()
{
	greening->render();
}

//==========================================
//更新
//==========================================
void Greening_Moji::update(int playerType)
{
	if (BasicUI::position.y > END_POS_RYOKUKA)
	{
		BasicUI::changePostion(greening, slideUp);
	}
}