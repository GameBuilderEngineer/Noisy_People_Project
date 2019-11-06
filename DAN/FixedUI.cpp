//================================
// 固定されたUI[FixedUI.cpp]
// 染矢晃介
//================================

//================================
//インクルード
//================================
#include "FixedUI.h"

//================================
// using宣言
//================================
using namespace fixedUiNS;

//================================
// コンストラクタ
//================================
FixedUI::FixedUI()
{
	fixedUI = new Sprite;
}

//================================
// デストラクタ
//================================
FixedUI::~FixedUI()
{
	delete fixedUI;
}

//================================
// 初期化
//================================
void FixedUI::initialize()
{
	fixedUI->initialize(
		*textureNS::reference(textureNS::UI_TIMERFLAME)//テクスチャ
		, SpriteNS::CENTER							//中心
		, WIDTH_TIMERFLAME							//横幅
		, HEIGHT_TIMERFLAME							//縦幅
		, POSITION_TIMERFLAME						//表示位置
		, ROTATION_TIMERFLAME						//回転
		, COLOR_TIMERFLAME);						//色
}

//================================
// 描画
//================================
void FixedUI::render()
{
	fixedUI->render();
}

//================================
// 更新
//================================
void FixedUI::update()
{

}

//================================
// 終了
//================================
void FixedUI::uninitialize()
{

}
