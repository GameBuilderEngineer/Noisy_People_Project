//==================================
//キャラアイコン処理[Mark.cpp]
//染矢　晃介
//==================================

//==================================
//インクルード
//==================================
#include "Mark.h"

//==================================
// using宣言
//==================================
using namespace markNS;

//==================================
//コンストラクタ
//==================================
Mark::Mark()
{
	//スプライトの作成
	mark = new Sprite;
}

//==================================
//デストラクタ
//==================================
Mark::~Mark()
{
	delete mark;
}

//==================================
//初期化
//==================================
void Mark::initialize(int playerType)
{
	switch (playerType)
	{
	case basicUiNS::P1:
		//初期表示位置の代入
		BasicUI::assingPosition(POSITION_MARK);
		//初期サイズの代入
		BasicUI::assingSize(WIDTH_MARK, HEIGHT_MARK);
		//テクスチャの初期化
		BasicUI::initialize(mark, textureNS::reference(textureNS::UI_EN_MARK));
		break;

	case basicUiNS::P2:
		//初期表示位置の代入
		BasicUI::assingPosition(POSITION_MARK_P2);
		//初期サイズの代入
		BasicUI::assingSize(WIDTH_MARK, HEIGHT_MARK);
		//テクスチャの初期化
		BasicUI::initialize(mark, textureNS::reference(textureNS::UI_EN_MARK));
	}

}

//=================================
//描画
//=================================
void Mark::render()
{
	mark->render();
}
