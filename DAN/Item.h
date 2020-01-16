//-----------------------------------------------------------------------------
// アイテム基底クラス [Item.h]
// Author : 
// 作成開始日 : 2019/  /
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"
#include "EffekseerManager.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace itemNS
{
	enum ITEM_TYPE
	{
		BATTERY,
		EXAMPLE,
		POWER_UP,
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

	//=============================================================================
	// アイテム標準エフェクト
	//=============================================================================
	class ItemEffect :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		ItemEffect(int no,D3DXVECTOR3* sync)
		{
			syncPosition = sync;
			managerNo = no;
			effectNo = effekseerNS::DROP_ITEM;
		}
		virtual void update()
		{
			position = *syncPosition;
			Instance::update();
		};
	};

	/*class ItemDestroyEffect :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		ItemDestroyEffect(int no, D3DXVECTOR3* sync)
		{
			syncPosition = sync;
			managerNo = no;
			effectNo = effekseerNS::DROP_ITEM;
		}
		virtual void update()
		{
			position = *syncPosition;
			Instance::update();
		}
	};*/

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


	itemNS::ItemEffect* itemEffect;
	//itemNS::ItemDestroyEffect* itemDestroyEffect;

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
