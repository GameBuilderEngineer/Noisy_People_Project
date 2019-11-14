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
	uvCoord1 = D3DXVECTOR2(0.0, 0.0);
	uvCoord2 = D3DXVECTOR2(1.0, 0.0);
	uvCoord3 = D3DXVECTOR2(0.0, 1.0);
	uvCoord4 = D3DXVECTOR2(1.0, 1.0);
	width = WIDTH_HP_GUAGE;
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
	BasicUI::assingUV(uvCoord1, uvCoord2, uvCoord3, uvCoord4);
	BasicUI::changePivot();
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
void HPguage::update(int hp)
{
	changeGage(hp);
}

//====================================
//HPゲージの伸び縮み処理
//====================================
void HPguage::changeGage(int hp)
{
	//HPを参照しサイズを変更
	float hpPersent = (float)hp / 100;//現在のHPを最大HPで割って割合を出す
	if (BasicUI::widthSize>WIDTH_HP_GUAGE*hpPersent)
	{
		BasicUI::reductionWidthSize(hpGuage, 1.0f);
	}
}