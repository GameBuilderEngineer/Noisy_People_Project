//-----------------------------------------------------------------------------
// クラス [exampleItem.cpp]
// Author : 新里
// 作成開始日 : 2019/ 10 / 17 
//-----------------------------------------------------------------------------
#include "exampleItem.h"
#include "ImguiManager.h"
using namespace itemNS;

//=============================================================================
// コンストラクタ
//=============================================================================
exampleItem::exampleItem(StaticMesh* _staticMesh, ItemData _itemData): Item(_staticMesh, _itemData)
{

}


//exampleItem::~exampleItem()
//{
//}

//=============================================================================
// 更新処理
//=============================================================================
void exampleItem::update(float frameTime)
{
	Item::update(frameTime);
}

//=============================================================================
// 描画処理
//=============================================================================
void exampleItem::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//Item::render(view, projection, cameraPosition);
}