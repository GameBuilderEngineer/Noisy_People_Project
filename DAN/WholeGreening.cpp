//==========================================
//緑化本数文字処理
//染矢晃介
//==========================================

//==========================================
//インクルード
//==========================================
#include "WholeGreening.h"

//==========================================
// using宣言
//==========================================
using namespace wholeGreenig_CharaNS;
using namespace basicUiNS;

//==========================================
//コンストラクタ
//==========================================
WholeGreening::WholeGreening()
{
	greening = new Sprite;
}

//==========================================
//デストラクタ
//==========================================
WholeGreening::~WholeGreening()
{
	delete greening;
}

//==========================================
//初期化
//==========================================
void WholeGreening::initialize()
{
		BasicUI::assingSize(WIDTH_GREENIG, HEIGHT_GREENIG);
		BasicUI::assingPosition(POSITION_GREENIG);
		BasicUI::initialize(greening, textureNS::reference(textureNS::RESULT_RYOKUKA));
}

//==========================================
//描画
//==========================================
void WholeGreening::render()
{
	greening->render();
}

//==========================================
//更新
//==========================================
void WholeGreening::update()
{
	
}