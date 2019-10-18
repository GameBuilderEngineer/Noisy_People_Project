//===================================================================================================================================
//【TestEffect.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/10/17
// [更新日]2019/10/17
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "TestEffect.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace testEffectNS;

//===================================================================================================================================
//【インスタンスクラス：コンストラクタ】
//===================================================================================================================================
TestEffectInstance::TestEffectInstance(): InstancingBillboardNS::Instance()
{
	//初期値の設定
	position = D3DXVECTOR3((float)(rand() % 2000 - 1000), 0.0f, (float)(rand() % 2000 - 1000));
	//rotation = D3DXVECTOR3((float)(rand() % 20 - 10), (float)(rand() % 20 - 10), (float)(rand() % 20 - 10));
	rotation = D3DXVECTOR3(0, 0, 0);
	//instance.scale				= D3DXVECTOR2((float)(rand() % 20 + 1), (float)(rand() % 20 + 1));
	scale = D3DXVECTOR2(20.0f, 20.0f);
	//int pattern = rand()%6;
	int pattern = 5;
	switch (pattern)
	{
	case 0:speed = D3DXVECTOR3(0, 0, -30.0f);			break;
	case 1:speed = D3DXVECTOR3(0, 0, 30.0f);			break;
	case 2:speed = D3DXVECTOR3(0, -30.0f, 0.0f);		break;
	case 3:speed = D3DXVECTOR3(0, 30.0f, 0.0f);			break;
	case 4:speed = D3DXVECTOR3(-30, 0, 0.0f);			break;
	case 5:speed = D3DXVECTOR3(30, 0, 0.0f);			break;
	}
	color = D3DCOLOR_RGBA(rand() % 255, rand() % 255, rand() % 255, 0);
	limitTime = (float)(rand() % 300);
	uv.x = 0.25f*sinf(limitTime * 4);
	uv.y = 0.25f*cosf(limitTime * 4);
}
//===================================================================================================================================
//【インスタンスクラス：更新】
//===================================================================================================================================
void TestEffectInstance::update(float frameTime) 
{
	lifeTimer += frameTime;
	if (lifeTimer >= limitTime)return;
	//カラー
	color.r = sinf(lifeTimer * 8);
	color.g = cosf(lifeTimer * 4);
	color.b = sinf(limitTime);
	color.a = 0.5f*sinf(lifeTimer / 2) + 0.5f;
	//UV
	float time = limitTime - lifeTimer;
	uv.x = 0.25f*sinf(time * 3);
	uv.y = 0.25f*cosf(time * 2);
	//位置
	//position += speed * frameTime;
	if (position.x > 1000)position.x = -1000;
	else if (position.x < -1000)position.x = 1000;
	if (position.y > 1000)position.y = -1000;
	else if (position.y < -1000)position.y = 1000;
	if (position.z > 1000)position.z = -1000;
	else if (position.z < -1000)position.z = 1000;
}

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
TestEffect::TestEffect():InstancingBillboard::InstancingBillboard()
{
	InstancingBillboard::initialize(
		*shaderNS::reference(shaderNS::INSTANCE_BILLBOARD),
		*textureNS::reference(textureNS::NEKO));
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
TestEffect::~TestEffect()
{
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void TestEffect::update(float frameTime)
{
	generateInstance(new TestEffectInstance());//毎フレーム生成
	InstancingBillboard::update(frameTime);
}