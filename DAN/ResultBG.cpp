//======================================
//リザルトBG処理[ResultBG.cpp]
//染矢晃介
//======================================

//======================================
//インクルード
//======================================
#include "ResultBG.h"

//======================================
// using宣言
//======================================
using namespace resultBGNS;

//======================================
//コンストラクタ
//======================================
ResultBG::ResultBG()
{
	resultBG = new Sprite;
}

//======================================
//デストラクタ
//======================================
ResultBG::~ResultBG()
{
	delete resultBG;
}

//======================================
//初期化
//======================================
void ResultBG::initialize()
{
	BasicUI::assingPosition(BG_POSITION);
	BasicUI::assingSize(WIDTH_BG, HEIGHT_BG);
	BasicUI::assingColor(COLOR);
	BasicUI::initialize(resultBG, textureNS::reference(textureNS::RESULT_BG));
}

//=====================================
//描画
//=====================================
void ResultBG::render()
{
	resultBG->render();
}

//=====================================
//更新
//======================================
void ResultBG::update()
{

}