//-----------------------------------------------------------------------------
// アイテム基底クラス [Item.cpp]
// Author : 
// 作成開始日 : 2019/  /
//-----------------------------------------------------------------------------
#include "Item.h"
#include "ImguiManager.h"
using namespace itemNS;


int Item::numOfItem = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
Item::Item(StaticMesh* _staticMesh, ItemData _itemData)
{
	numOfItem++;

	onGravity = true;
	itemData = _itemData;
	position = itemData.defaultPosition;
	Object::initialize(&position);

	if (itemData.type == POWER_UP)
	{
		sphereCollider.initialize(&position, 5.0f);
		// てかデカすぎだ
		scale *= 0.0002f;
	}
	else
	{
		sphereCollider.initialize(&position, _staticMesh->mesh);
	}

	itemEffect = new itemNS::ItemEffect(0, &position);
	if (itemData.type == POWER_UP)
	{
		itemEffect->scale *= 2;
	}
	effekseerNS::play(0, itemEffect);
	//itemDestroyEffect = new itemNS::ItemDestroyEffect(0, &position);
}


//=============================================================================
// デストラクタ
//=============================================================================
Item::~Item()
{
	//effekseerNS::stop(0);
	effekseerNS::stop(0, itemEffect);
	//effekseerNS::stop(0, itemDestroyEffect);
	numOfItem--;
}


//=============================================================================
// 更新処理
//=============================================================================
void Item::update(float frameTime)
{
	Object::update();
	
}


//=============================================================================
// 重力発生メッシュ（接地メッシュ）の設定
//=============================================================================
void Item::setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;
}


//=============================================================================
// Getter
//=============================================================================
int Item::getNumOfItem(){ return numOfItem; }
ItemData* Item::getItemData() { return &itemData; }


//=============================================================================
// Setter
//=============================================================================
void Item::setDataToItem(ItemData _itemData) { itemData = _itemData; }
