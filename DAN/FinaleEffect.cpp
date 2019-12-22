//===================================================================================================================================
//【FinaleEffect.cpp】
// [作成者] HAL東京GP12A332 11 菅野 樹
// [作成日] 2019/12/23
// [更新日] 2019/12/23
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "FinaleEffect.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace FinaleEffectNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
FinaleEffect::FinaleEffect()
{
	//フラワーエフェクト
	flowerObj = new Object();
	flowerObj->initialize(&D3DXVECTOR3(0, 0, 0));
	distanceFlower  = 15.0f;
	//羽エフェクト
	featherObj = new Object();
	featherObj->initialize(&D3DXVECTOR3(0, 0, 0));
	distanceFeather = 15.0f;
}


//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
FinaleEffect::~FinaleEffect()
{
	SAFE_DELETE(flowerObj);
	SAFE_DELETE(featherObj);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void FinaleEffect::update(Camera* camera)
{
	//フラワーエフェクト
	flowerObj->position =  camera->position + camera->getDirectionZ()*distanceFlower;
	flowerObj->postureControl(flowerObj->getAxisX()->direction, -camera->getDirectionZ(),0.1f);
	flowerObj->update();
	//羽エフェクト
	featherObj->position =  camera->position + camera->getDirectionZ()*distanceFeather;
	featherObj->postureControl(featherObj->getAxisX()->direction, -camera->getDirectionZ(),0.01f);
	featherObj->update();
}

//===================================================================================================================================
//【再生】
//===================================================================================================================================
void FinaleEffect::play()
{
	//花びらエフェクトの再生
	flowerShower = new FlowerShower(&flowerObj->matrixWorld);
	effekseerNS::play(0, flowerShower);
	//羽エフェクトの再生
	feather = new Feather(&featherObj->matrixWorld);
	effekseerNS::play(0, feather);
}

#ifdef _DEBUG

//===================================================================================================================================
//【再生】
//===================================================================================================================================
void FinaleEffect::play(int i)
{
	switch (i)
	{
	case 0:
		//花びらエフェクトの再生
		flowerShower = new FlowerShower(&flowerObj->matrixWorld);
		break;
	case 1:
		//羽エフェクトの再生
		feather = new Feather(&featherObj->matrixWorld);
		break;
	case 2:
		flowerShower = new DigitMode(&flowerObj->matrixWorld);
		break;
	case 3:
		flowerShower = new DigitMode(&flowerObj->matrixWorld);
		break;
	case 4:
		flowerShower = new DigitalShiftEffect(&flowerObj->matrixWorld);
		break;
	case 5:
		flowerShower = new SkyJumpEffect(&flowerObj->matrixWorld);
		break;
	case 6:
		flowerShower = new SkyVisionEffect(&flowerObj->matrixWorld);
		break;
	case 7:
		flowerShower = new ShiftTerminateEffect(&flowerObj->matrixWorld);
		break;
	case 8:
		flowerShower = new DigitaTreeNearEffect(&flowerObj->matrixWorld);
		break;

	}
	effekseerNS::play(0, flowerShower);

}
#endif // _DEBUG


