//====================================
//HPゲージ(空)処理[HPguage.cpp]
//染矢晃介
//====================================

//====================================
//インクルード
//====================================
#include "HPguageEmp.h"

//====================================
// using宣言
//====================================
using namespace hpGuageEmpNS;

//====================================
//コンストラクタ
//====================================
HPguageEmp::HPguageEmp()
{
	hpGuageEmp = new Sprite;
}

//====================================
//デストラクタ
//====================================
HPguageEmp::~HPguageEmp()
{
	delete hpGuageEmp;
}

//====================================
//初期化
//====================================
void HPguageEmp::initialize()
{
	BasicUI::assingPosition(POSITION_HP_GUAGE_EMP);
	BasicUI::assingSize(WIDTH_HP_GUAGE_EMP, HEIGHT_HP_GUAGE_EMP);
	BasicUI::initialize(hpGuageEmp, textureNS::reference(textureNS::UI_HP_GUAGE_EMP_P1));
}

//====================================
//描画
//====================================
void HPguageEmp::render()
{
	hpGuageEmp->render();
}

//====================================
//更新
//====================================
void HPguageEmp::update()
{

}