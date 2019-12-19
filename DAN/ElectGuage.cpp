//===================================
//電力ゲージ処理[ElectGuage.cpp]
//染矢晃介
//===================================

//===================================
//インクルード
//===================================
#include "ElectGuage.h"
#include "Input.h"

//===================================
// usuing宣言
//===================================
using namespace electGuageNS;

//===================================
//コンストラクタ
//===================================
ElectGuage::ElectGuage()
{
	electGuage = new Sprite;
	width = WIDTH_EN_GUAGE;
}

//===================================
//デストラクタ
//===================================
ElectGuage::~ElectGuage()
{
	delete electGuage;
}

//===================================
//初期化
//===================================
void ElectGuage::initialize(int playerType)
{
	switch (playerType)
	{
	case basicUiNS::P1:
		BasicUI::assingColor(COLOR);
		BasicUI::assingSize(WIDTH_EN_GUAGE, HEIGHT_EN_GUAGE);
		BasicUI::assingPosition(POSITION_EN_GUAGE);
		BasicUI::changePivot(SpriteNS::TOP_LEFT);
		BasicUI::initialize(electGuage, textureNS::reference(textureNS::UI_EN_GUAGE_P1));
		break;
	case basicUiNS::P2:
		BasicUI::assingSize(WIDTH_EN_GUAGE, HEIGHT_EN_GUAGE);
		BasicUI::assingPosition(POSITION_EN_GUAGE_P2);
		BasicUI::changePivot(SpriteNS::TOP_RIGHT);
		BasicUI::initialize(electGuage, textureNS::reference(textureNS::UI_EN_GUAGE_P2));
		break;

	}
}

//===================================
//描画
//===================================
void ElectGuage::render()
{
	
	electGuage->render();
}

//===================================
//更新
//引数：電力
//===================================
void ElectGuage::update(int power)
{
	changeGuage(power);
	if (power < 70)
	{
		BasicUI::changeColor(electGuage, LED_COLOR);
	}
	else
	{
		BasicUI::changeColor(electGuage,COLOR);
	}
}

//===================================
//ゲージの変化処理
//引数：現在の電力
//===================================
void ElectGuage::changeGuage(int power)
{
	//HPを参照しサイズを変更
	float enPersent = (float)power / 300;//現在のENを最大HPで割って割合を出す
	/*BasicUI::changeUV(electGuage, D3DXVECTOR2(0, 0), D3DXVECTOR2(enPersent, 0), D3DXVECTOR2(0, 1), D3DXVECTOR2(enPersent, 1));*/
	if (BasicUI::widthSize>WIDTH_EN_GUAGE*enPersent)
	{
		BasicUI::reductionWidthSize(electGuage, 1.5f);
	}
	else if(enPersent <=1.0f &&BasicUI::widthSize<512)
	{
		BasicUI::expansionWidthSize(electGuage, 1.5f);
	}
}