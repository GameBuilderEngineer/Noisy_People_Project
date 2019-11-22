//====================================
//電力ゲージ(空)処理[ElectGuage.cpp]
//染矢晃介
//====================================

//====================================
//インクルード
//====================================
#include "ElectGuageeEmp.h"

//====================================
// using宣言
//====================================
using namespace electGuageEmpNS;

//====================================
//コンストラクタ
//====================================
ElectGuageEmp::ElectGuageEmp()
{
	electGuageEmp = new Sprite;
}

//====================================
//デストラクタ
//====================================
ElectGuageEmp::~ElectGuageEmp()
{
	delete electGuageEmp;
}

//====================================
//初期化
//====================================
void ElectGuageEmp::initialize(int playerType)
{
	switch (playerType)
	{
	case basicUiNS::P1:
		BasicUI::assingPosition(POSITION_HP_GUAGE_EMP);
		BasicUI::assingSize(WIDTH_HP_GUAGE_EMP, HEIGHT_HP_GUAGE_EMP);
		BasicUI::initialize(electGuageEmp, textureNS::reference(textureNS::UI_EN_GUAGE_EMP_P1));
		break;

	case basicUiNS::P2:
		BasicUI::assingPosition(POSITION_HP_GUAGE_EMP_P2);
		BasicUI::assingSize(WIDTH_HP_GUAGE_EMP, HEIGHT_HP_GUAGE_EMP);
		BasicUI::initialize(electGuageEmp, textureNS::reference(textureNS::UI_EN_GUAGE_EMP_P2));
		break;

	}
}

//====================================
//描画
//====================================
void ElectGuageEmp::render()
{
	electGuageEmp->render();
}

//====================================
//更新
//====================================
void ElectGuageEmp::update()
{

}