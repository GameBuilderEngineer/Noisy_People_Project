//====================================
//HPゲージ処理[HPguage.cpp]
//染矢晃介
//====================================

//====================================
//インクルード
//====================================
#include "HPguage.h"

//====================================
// using宣言
//====================================
using namespace hpGuageNS;

//====================================
//コンストラクタ
//====================================
HPguage::HPguage()
{
	hpGuage = new Sprite;
}

//====================================
//デストラクタ
//====================================
HPguage::~HPguage()
{
	delete hpGuage;
}

//====================================
//初期化
//====================================
void HPguage::initialize()
{
	BasicUI::assingPosition(POSITION_HP_GUAGE);
	BasicUI::assingSize(WIDTH_HP_GUAGE, HEIGHT_HP_GUAGE);
	BasicUI::initialize(hpGuage, textureNS::reference(textureNS::UI_HP_GUAGE_P1));
}

//====================================
//描画
//====================================
void HPguage::render()
{
	hpGuage->render();
}

//====================================
//更新
//====================================
void HPguage::update()
{

}