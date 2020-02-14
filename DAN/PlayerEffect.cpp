//===================================================================================================================================
//【PlayerEffect.cpp】
// [作成者] HAL東京GP12A332 11 菅野 樹
// [作成日] 2019/12/23
// [更新日] 2019/12/23
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "PlayerEffect.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace PlayerEffectNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
PlayerEffect::PlayerEffect(int managerNo)
{
	//プレイヤーのポインタ
	this->player = NULL;
	//スカイポジションのポインタ
	this->skyPosition = NULL;

	//エフェクシアーのマネージャー番号
	this->managerNo = managerNo;

	distanceFlower = 15.0f;
	distanceFeather = 15.0f;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
PlayerEffect::~PlayerEffect()
{

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void PlayerEffect::update(float frameTime)
{

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void PlayerEffect::setPointer(Object* player, D3DXVECTOR3* skyPosition)
{
	//プレイヤーのポインタ
	this->player = player;
	//シフトラインのポインタ
	this->skyPosition = skyPosition;
}

//===================================================================================================================================
//【再生】
//===================================================================================================================================
void PlayerEffect::play(int i)
{
	switch (i)
	{
	case EFFECT_LIST::DIGHITAL_SHIFT:
		digitalShift = new DigitalShiftEffect(&player->matrixCenter, managerNo);
		effekseerNS::play(managerNo, digitalShift);
		break;
	case EFFECT_LIST::DIGIT_MODE:
		digitMode = new DigitMode(&player->matrixCenter,managerNo);
		effekseerNS::play(managerNo, digitMode);
		break;
	case EFFECT_LIST::SHIFT_TERMINATE:
		shiftTerminate = new ShiftTerminateEffect(&player->matrixCenter,managerNo);
		effekseerNS::play(managerNo, shiftTerminate);
		break;
	case EFFECT_LIST::SKY_JUMP:
		skyJump= new SkyJumpEffect(&player->matrixCenter,skyPosition,managerNo);
		effekseerNS::play(managerNo, skyJump);
		break;
	case EFFECT_LIST::SKY_VISION:
		skyVision = new SkyVisionEffect(&player->matrixCenter,skyPosition,managerNo);
		effekseerNS::play(managerNo, skyVision);
		break;
	}
}

//===================================================================================================================================
//【停止】
//===================================================================================================================================
void PlayerEffect::stop(int i)
{
	switch (i)
	{
	case EFFECT_LIST::DIGHITAL_SHIFT:
		effekseerNS::stop(managerNo, digitalShift);
		break;
	case EFFECT_LIST::DIGIT_MODE:
		effekseerNS::stop(managerNo, digitMode);
		break;
	case EFFECT_LIST::SHIFT_TERMINATE:
		effekseerNS::stop(managerNo, shiftTerminate);
		break;
	case EFFECT_LIST::SKY_JUMP:
		effekseerNS::stop(managerNo, skyJump);
		break;
	case EFFECT_LIST::SKY_VISION:
		effekseerNS::stop(managerNo, skyVision);
		break;
	}
}

