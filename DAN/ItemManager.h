//-----------------------------------------------------------------------------
// アイテム管理クラス [Item.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshRenderer.h"
#include "Item.h"
#include "Battery.h"
#include "exampleItem.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace itemNS
{	
	const int NUM_SURPLUS_DATA = 10;			// 初期アイテムデータ数に加えて確保する余剰分のベクター要素数

}


//=============================================================================
//クラス定義
//=============================================================================
class ItemManager
{
private:
	static ItemManager* instance;				// 外部取得用ポインタ
	std::vector<Item*> itemList;				// アイテムポインタリスト
	StaticMeshRenderer* batteryRenderer;		// 描画オブジェクト
	StaticMeshRenderer* exampleItemRender;		// テスト用アイテム
	int nextID;									// 次回ツリー発行ID
	LPD3DXMESH	attractorMesh;					// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;				// 重力（引力）発生オブジェクトマトリックス

public:
	// 外部取得用
	ItemManager() { instance = this; }	// ※シングルトンではない
	~ItemManager();
	static ItemManager* get() { return instance; }

	void initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createItem(itemNS::ItemData itemData);
	void destroyItem(int _itemID);
	void destroyAllItem();
	int issueNewItemID();
	void outputGUI();

	// Getter
	std::vector<Item*>& getItemList();
};
