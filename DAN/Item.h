//-----------------------------------------------------------------------------
// アイテム基底クラス [Item.h]
// Author : 
// 作成開始日 : 2019/  /
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshObject.h"
#include "BoundingSphere.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace itemNS
{
	enum ITEM_TYPE
	{
		BATTERY,
		ITEM_TYPE_MAX
	};


	struct ItemData
	{
		int id;							// 識別番号
		ITEM_TYPE itemType;				// アイテムタイプ
		D3DXVECTOR3 initialPosition;	// 初期座標
		void initialize() { ZeroMemory(this, sizeof(ItemData)); }
	};
}


//=============================================================================
//クラス定義
//=============================================================================
class Item: public StaticMeshObject
{
private:
	itemNS::ItemData* itemData;

	// Static
	static int numOfItem;					// アイテムオブジェクトの総数

public:
	Item(StaticMesh* _staticMesh, itemNS::ItemData* _itemData);
	~Item();
	virtual void update(float frameTime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	BoundingSphere sphereCollider;


	// Getter
	static int getNumOfItem();				// アイテムの数を取得
	itemNS::ItemData* getItemData();		// アイテムデータを取得

	// Setter
	void setDataToItem(itemNS::ItemData* _itemData);
};
