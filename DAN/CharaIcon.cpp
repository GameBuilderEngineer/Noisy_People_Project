//==================================
//キャラアイコン処理[CharaIcon.cpp]
//染矢　晃介
//==================================

//==================================
//インクルード
//==================================
#include "CharaIcon.h"

//==================================
// using宣言
//==================================
using namespace charaIconNS;

//==================================
//コンストラクタ
//==================================
CharaIcon::CharaIcon()
{
	//スプライトの作成
	charaIcon = new Sprite;
}

//==================================
//デストラクタ
//==================================
CharaIcon::~CharaIcon()
{
	delete charaIcon;
}

//==================================
//初期化
//==================================
void CharaIcon::initialize()
{
	//初期表示位置の代入
	BasicUI::assingPosition(POSITION_ICON);
	//初期サイズの代入
	BasicUI::assingSize(WIDTH_ICON, HEIGHT_ICON);
	//テクスチャの初期化
	BasicUI::initialize(charaIcon, textureNS::reference(textureNS::UI_ICON_P1));
}

//=================================
//描画
//=================================
void CharaIcon::render()
{
	charaIcon->render();
}
