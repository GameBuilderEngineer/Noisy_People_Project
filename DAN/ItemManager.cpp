//-----------------------------------------------------------------------------
// アイテム管理クラス [Item.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#include "ItemManager.h"
#include "ImguiManager.h"
#include "ItemTools.h"
using namespace itemNS;

//=============================================================================
// 初期化
//=============================================================================
void ItemManager::initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	nextID = 0;		// 次回発行IDを0に初期化

	// 描画オブジェクトを作成
	batteryRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
	exampleItemRender = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2));

#if 1	// アイテムツールのデータを読み込む
	ITEM_TOOLS* itemTools = new ITEM_TOOLS;
	for (int i = 0; i < itemTools->GetItemMax(); i++)
	{
		createItem(itemTools->GetItemSet(i));
	}
	SAFE_DELETE(itemTools);
#endif
}


//=============================================================================
// 終了処理
//=============================================================================
void ItemManager::uninitialize()
{
	// 全アイテムオブジェクトを破棄
	destroyAllItem();

	// ベクターの確保メモリを初期化（メモリアロケータだけに戻す）
	std::vector<Item*> temp;
	itemList.swap(temp);

	// 描画オブジェクトの破棄
	SAFE_DELETE(batteryRenderer);
	SAFE_DELETE(exampleItemRender);
}


//=============================================================================
// 更新処理
//=============================================================================
void ItemManager::update(float frameTime)
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		itemList[i]->update(frameTime);
	}

	batteryRenderer->update();
	exampleItemRender->update();
}


//=============================================================================
// 描画処理
//=============================================================================
void ItemManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	batteryRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	exampleItemRender->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// アイテムオブジェクトの作成
//=============================================================================
void ItemManager::createItem(ItemData itemData)
{
	Item* item = NULL;

	switch (itemData.type)
	{
	case BATTERY:
		item = new Battery(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS), itemData);
		item->setAttractor(attractorMesh, attractorMatrix);
		itemList.emplace_back(item);
		batteryRenderer->registerObject(item);
		break;
	case EXAMPLE:
		item = new exampleItem(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2), itemData);
		item->setAttractor(attractorMesh, attractorMatrix);
		itemList.emplace_back(item);
		exampleItemRender->registerObject(item);
		//itemList.emplace_back(new exampleItem(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2), itemData));

		break;
	}
}


//=============================================================================
// アイテムオブジェクトの破棄
//=============================================================================
void ItemManager::destroyItem(int _itemID)
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		if (itemList[i]->getItemData()->itemID == _itemID)
		{
			// 描画の解除
			switch (itemList[i]->getItemData()->type)
			{
			case BATTERY:
				batteryRenderer->unRegisterObjectByID(itemList[i]->id);
				break;
			}
			SAFE_DELETE(itemList[i]);				// インスタンス破棄
			itemList.erase(itemList.begin() + i);	// ベクター要素を消去
			break;
		}
	}
}


//=============================================================================
// 全アイテムオブジェクトの破棄
//=============================================================================
void ItemManager::destroyAllItem()
{
	// 描画を全解除
	batteryRenderer->allUnRegister();
	exampleItemRender->allUnRegister();

	for (size_t i = 0; i < itemList.size(); i++)
	{
		SAFE_DELETE(itemList[i]);
	}
	itemList.clear();
}


//=============================================================================
// アイテムIDを発行する
//=============================================================================
int ItemManager::issueNewItemID()
{
	int ans = nextID;
	nextID++;
	return ans;
}


//=============================================================================
// ImGuiに出力
//=============================================================================
void ItemManager::outputGUI()
{
#ifdef _DEBUG
	bool debugDestroyAllFlag = false;

	if (ImGui::CollapsingHeader("ItemInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Text("numOfItem:%d", Item::getNumOfItem());

		ImGui::Checkbox("Delete All Item", &debugDestroyAllFlag);
	}
#endif
}


//=============================================================================
// Getter
//=============================================================================
std::vector<Item*>& ItemManager::getItemList() { return itemList; }