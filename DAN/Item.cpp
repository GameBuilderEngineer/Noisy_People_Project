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
Item::Item(StaticMesh* _staticMesh, ItemData* _itemData)
{
	numOfItem++;

	onGravity = true;
	itemData = _itemData;
	position = itemData->initialPosition;
	Object::initialize(&position);
	sphereCollider.initialize(&position, _staticMesh->mesh);
	//sphereCollider.initialize(2);

}


//=============================================================================
// デストラクタ
//=============================================================================
Item::~Item()
{
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
// 描画処理
//=============================================================================
//void Item::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
//{
//	Object::render(
//		*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
//
////デバッグ時描画
//#ifdef _DEBUG
//	sphereCollider.render(matrixWorld);
//#endif // _DEBUG
//}


//=============================================================================
// Getter
//=============================================================================
int Item::getNumOfItem(){ return numOfItem; }
ItemData* Item::getItemData() { return itemData; }


//=============================================================================
// Setter
//=============================================================================
void Item::setDataToItem(ItemData* _itemData) { itemData = _itemData; }
