//===================================================================================================================================
//【TreeEffect.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/25
// [更新日]2019/12/04
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "TreeEffect.h"
#include "TextureLoader.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace TreeEffectNS;

#pragma region Instance
//===================================================================================================================================
//【マーカーインスタンスクラス：コンストラクタ】
//===================================================================================================================================
MarkerInstance::MarkerInstance(Tree* owner) : InstancingBillboardNS::Instance()
{
	//初期値の設定
	//this->position	= position + D3DXVECTOR3((float)(rand() % 6 - 3), 0.0f, (float)(rand() % 6 - 3));
	this->tree = owner;
	position = tree->center;
	position.y += tree->size.y / 2;
	scale = D3DXVECTOR2(0.5f, 0.5f);
	//rotation = D3DXVECTOR3((float)(rand() % 20 - 10), (float)(rand() % 20 - 10), (float)(rand() % 20 - 10));
	rotation = D3DXVECTOR3(0, 0, 0);
	//instance.scale				= D3DXVECTOR2((float)(rand() % 20 + 1), (float)(rand() % 20 + 1));
	//speed = D3DXVECTOR3(0, 3.0f, 0.0f);

	changeColor();
	//limitTime = (float)(rand() % 30);
	//uv.x = 0.25f*sinf(limitTime * 4);
	//uv.y = 0.25f*cosf(limitTime * 4);
}
//===================================================================================================================================
//【マーカーインスタンスクラス：更新】
//===================================================================================================================================
void MarkerInstance::update(float frameTime)
{
	//lifeTimer += frameTime;
	if (lifeTimer >= limitTime)return;
	//カラー
	//color.r = sinf(lifeTimer * 8);
	//color.g = cosf(lifeTimer * 4);
	//color.b = sinf(limitTime);
	//color.a = 1.0f - (lifeTimer / limitTime);
	changeColor();

	//UV
	//float time = limitTime - lifeTimer;
	//uv.x = 0.25f*sinf(time * 1.5);
	//uv.y = 0.25f*cosf(time * 0.5);

	//位置
	position = tree->center;
	position.y += tree->size.y / 2;
	//position += speed * frameTime;
}
//===================================================================================================================================
//【マーカーインスタンスクラス：状態別色変更】
//===================================================================================================================================
void MarkerInstance::changeColor()
{
	if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
	{
		color = D3DCOLOR_RGBA(0, 0, 255, 196);
		return;
	}

	if (tree->getTreeData()->greenState == treeNS::GREEN)
	{
		color = D3DCOLOR_RGBA(0, 255, 0, 196);
	}
	else {
		color = D3DCOLOR_RGBA(255, 0, 0, 196);
	}
}

//===================================================================================================================================
//【標識インスタンスクラス：コンストラクタ】
//===================================================================================================================================
SignInstance::SignInstance(Tree* owner) : InstancingBillboardNS::Instance()
{
	//初期値の設定
	//this->position	= position + D3DXVECTOR3((float)(rand() % 6 - 3), 0.0f, (float)(rand() % 6 - 3));
	this->tree = owner;
	position = tree->center;
	//rotation = D3DXVECTOR3((float)(rand() % 20 - 10), (float)(rand() % 20 - 10), (float)(rand() % 20 - 10));
	rotation = D3DXVECTOR3(0, 0, 0);
	//instance.scale				= D3DXVECTOR2((float)(rand() % 20 + 1), (float)(rand() % 20 + 1));
	scale = D3DXVECTOR2(1.0f*4.0f, 1.125f*4.0f);
	//speed = D3DXVECTOR3(0, 3.0f, 0.0f);
	changeColor();
	//limitTime = (float)(rand() % 30);
	//uv.x = 0.25f*sinf(limitTime * 4);
	//uv.y = 0.25f*cosf(limitTime * 4);
}
//===================================================================================================================================
//【標識インスタンスクラス：更新】
//===================================================================================================================================
void SignInstance::update(float frameTime)
{
	//lifeTimer += frameTime;
	if (lifeTimer >= limitTime)return;
	//カラー
	//color.r = sinf(lifeTimer * 8);
	//color.g = cosf(lifeTimer * 4);
	//color.b = sinf(limitTime);
	//color.a = 1.0f - (lifeTimer / limitTime);
	changeColor();
	//UV
	//float time = limitTime - lifeTimer;
	//uv.x = 0.25f*sinf(time * 1.5);
	//uv.y = 0.25f*cosf(time * 0.5);
	
	//位置
	position = tree->center;
	//position += speed * frameTime;
}

//===================================================================================================================================
//【標識インスタンスクラス：状態別色変更】
//===================================================================================================================================
void SignInstance::changeColor()
{
	if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
	{
		color = D3DCOLOR_RGBA(0, 0, 255, 196);
		return;
	}

	if (tree->getTreeData()->greenState == treeNS::GREEN)
	{
		color = D3DCOLOR_RGBA(0, 255, 0, 196);
	}
	else {
		color = D3DCOLOR_RGBA(255, 0, 0, 196);
	}
}

#pragma endregion


//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
TreeEffect::TreeEffect()
{
	effectCycle = 0.0f;

	//状態マーカー
	marker = new InstancingBillboard();
	marker->initialize(*textureNS::reference(textureNS::UI_TREE_MARKER));
	marker->setRenderType(InstancingBillboardNS::FOREGROUND_PASS);

	//ツリー標識
	sign = new InstancingBillboard();
	sign->initialize(*textureNS::reference(textureNS::UI_TREE_SIGN));
	sign->setRenderType(
		InstancingBillboardNS::FOREGROUND_PASS | 
		InstancingBillboardNS::Y_BILLBOARD_PASS);
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
TreeEffect::~TreeEffect()
{
	SAFE_DELETE(marker);
	SAFE_DELETE(sign);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void TreeEffect::update(float frameTime)
{
	//エフェクトの発生
	//effectCycle += frameTime;
	//更新
	marker->update(frameTime);
	sign->update(frameTime);

	resetEffectCycle();

}

//===================================================================================================================================
//【生成】
//===================================================================================================================================
void TreeEffect::generateInstance(int type,Tree* tree)
{
	switch (type)
	{
	case INSTANCE_MARKER:	marker->generateInstance(new MarkerInstance(tree));		break;
	case INSTANCE_SIGN:		sign->generateInstance(new SignInstance(tree));						break;
	}
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void TreeEffect::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//描画
	marker->render(view,projection,cameraPosition);
	sign->render(view,projection,cameraPosition);
}

//===================================================================================================================================
//【描画機能を有効化する】
//===================================================================================================================================
void TreeEffect::enableRender()
{
	marker->enableRender();
	sign->enableRender();
}

//===================================================================================================================================
//【描画機能を無効化する】
//===================================================================================================================================
void TreeEffect::disableRender()
{
	marker->disableRender();
	sign->disableRender();
}



//===================================================================================================================================
//【標準エフェクト】
//===================================================================================================================================
void TreeEffect::resetEffectCycle()
{
	//エフェクトの発生
	if (effectCycle >= EFFECT_CYCLE)
	{
		effectCycle = 0.0f;
	}

}

//===================================================================================================================================
//【標準エフェクト】
//===================================================================================================================================
void TreeEffect::playStandardEffect(D3DXVECTOR3* position)
{
	//TreeEffectNS::DigitTree* instance = new TreeEffectNS::DigitTree(position);
	//effekseerNS::play(instance);
}

//===================================================================================================================================
//【被襲撃エフェクト】
//===================================================================================================================================
void TreeEffect::playRaidEffect(int no,D3DXVECTOR3* position)
{
	TreeEffectNS::DigitTreeRaid* instance = new TreeEffectNS::DigitTreeRaid(position);
	effekseerNS::play(no,instance);
}
