//-----------------------------------------------------------------------------
// アイテム基底クラス [Item.h]
// Author : 
// 作成開始日 : 2019/  /
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace itemNS
{
	enum ITEM_TYPE
	{
		BATTERY,
		EXAMPLE,
		ITEM_TYPE_MAX
	};

	typedef struct ItemData
	{
		int itemID;						// 識別番号(0..*)
		int type;						// アイテムタイプ
		D3DXVECTOR3 defaultPosition;	// 初期座標
		D3DXVECTOR3 defaultDirection;	// 初期正面方向
		void zeroClear() { ZeroMemory(this, sizeof(ItemData)); }
	}ITEMSET;
}


//=============================================================================
//クラス定義
//=============================================================================
class Item: public Object
{
private:
	itemNS::ItemData itemData;
	LPD3DXMESH	attractorMesh;				// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;			// 重力（引力）発生オブジェクトマトリックス

	// Static
	static int numOfItem;					// アイテムオブジェクトの総数

public:
	BoundingSphere sphereCollider;			// コライダ
	Item(StaticMesh* _staticMesh, itemNS::ItemData _itemData);
	~Item();
	virtual void update(float frameTime);
	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);

	// Getter
	static int getNumOfItem();				// アイテムの数を取得
	itemNS::ItemData* getItemData();		// アイテムデータを取得

	// Setter
	void setDataToItem(itemNS::ItemData _itemData);
};
