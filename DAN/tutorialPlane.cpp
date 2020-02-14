//===================================================================================================================================
//【tutorialPlane.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/11/05
// [更新日]2019/11/05
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "tutorialPlane.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "TutorialUI.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace tutorialPlaneNS;

//===================================================================================================================================
//【インスタンスクラス：コンストラクタ】
//===================================================================================================================================
PlaneInstance::PlaneInstance(D3DXVECTOR3 position) : InstancingBillboardNS::Instance()
{
	//初期値の設定
	this->position = position;
	this->rotation = D3DXVECTOR3(0, 0, 0);
	this->scale = D3DXVECTOR2(5.0f, 4.0f);
	this->limitTime = 1.0f;
	this->uv.x = (1.0f / 4.0f) * 0.0f;
	this->uv.y = (1.0f / 2.0f) * 0.0f;

	this->tmpCnt = 0;
}

//===================================================================================================================================
//【インスタンスクラス：更新】
//===================================================================================================================================
void PlaneInstance::update(float frameTime)
{
	if (lifeTimer >= limitTime)return;
}

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
TutorialPlane::TutorialPlane(const D3DXVECTOR3 position) :InstancingBillboard::InstancingBillboard()
{
	//初期化
	planeStep = TUTORIAL_PLANE_ID::PLANE_ENEMY;

	InstancingBillboard::initialize(*textureNS::reference(textureNS::TUTORIAL_UI_PLANE), 3, 1);
	setRenderType(InstancingBillboardNS::FOREGROUND_PASS);

	generateInstance(new PlaneInstance(position));
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
TutorialPlane::~TutorialPlane()
{

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void TutorialPlane::update(float frameTime, int inPlaneStep)
{
	InstancingBillboard::update(frameTime);

	switch (inPlaneStep)
	{
	case TUTORIAL_PLANE_ID::PLANE_ENEMY:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv = 
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 0.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 0.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_TREE:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 1.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 0.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_DIGITAL:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 2.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 0.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_SKY:
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 3.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 0.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_SHIFT_0:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 0.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 1.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_SHIFT_1:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 1.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 1.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_CLEAR:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 2.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 1.0f);
		InstancingBillboard::updateUV();
		break;
	default:
		break;
	}

	//コピー
	planeStep = inPlaneStep;
}
