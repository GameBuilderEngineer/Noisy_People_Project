//-----------------------------------------------------------------------------
// アイテム管理クラス [Item.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#include "ItemManager.h"
#include "StaticMeshObject.h"
#include "ImguiManager.h"
using namespace itemNS;

//=============================================================================
// 初期化
//=============================================================================
void ItemManager::initialize()
{
#if 0
	// アイテムデータリストの追加
	ItemData itemData;
	itemDataList.reserve(/*ツールで生成したファイルに格納されたアイテムデータの数*/ +NUM_SURPLUS_DATA);
	for (int i = 0; i < itemDataList.size(); i++)
	{
		itemData.zeroClear();
		if (i >= /*ツールで生成したファイルに格納されたアイテムデータの数*/ 0/*仮*/) continue;

		// ファイルからデータを読み取る

		// データリストに追加する
		itemDataList.push_back(itemData);
	}

	// アイテムオブジェクトの作成
	itemList.reserve(itemDataList.size());
	for (size_t i = 0; itemDataList.size() - NUM_SURPLUS_DATA; i++)
	{
		createItem(&itemDataList[i]);
	}
#endif

	// 描画オブジェクトを作成
	batteryRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
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
}


//=============================================================================
// 描画処理
//=============================================================================
void ItemManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	batteryRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// アイテムオブジェクトの作成
//=============================================================================
void ItemManager::createItem(ItemData* itemData)
{
	Item* item = NULL;

	switch (itemData->itemType)
	{
	case BATTERY:
		item = new Battery(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS), itemData);
		itemList.emplace_back(item);
		batteryRenderer->generateObject(item);
		break;
	case EXAMPLE:
		itemList.emplace_back(new exampleItem(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2), itemData));
		break;
	}
}


//=============================================================================
// アイテムオブジェクトの破棄
// ※使用不可
//=============================================================================
void ItemManager::destroyItem(int _id)
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		if (itemList[i]->getItemData()->id == _id)
		{
			SAFE_DELETE(itemList[i]);
			itemList.erase(itemList.begin() + i);
			break;
		}
	}
	// batteryRenderer->deleteObject();
	// 今はStaticMeshObjectに登録されたオブジェクトを個別に破棄できない
}


//=============================================================================
// 全アイテムオブジェクトの破棄
// ※仮実装
//=============================================================================
void ItemManager::destroyAllItem()
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		SAFE_DELETE(itemList[i]);
	}
	
	itemList.clear();
	SAFE_DELETE(batteryRenderer);
	batteryRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
}


//=============================================================================
// ImGuiに出力
//=============================================================================
void ItemManager::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("ItemInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		//float limitTop = 1000;
		//float limitBottom = -1000;

		ImGui::Text("numOfItem:%d", Item::getNumOfItem());

		//ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//位置
		//ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//回転
		//ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);					//スケール
		//ImGui::SliderFloat("radius", &radius, 0, limitTop);							//半径
		//ImGui::SliderFloat("alpha", &alpha, 0, 255);									//透過値
		//ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);					//速度
		//ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//加速度
		//ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);				//重力

		//ImGui::Checkbox("onGravity", &onGravity);										//重力有効化フラグ
		//ImGui::Checkbox("onActive", &onActive);										//アクティブ化フラグ
		//ImGui::Checkbox("onRender", &onRender);										//描画有効化フラグ
		//ImGui::Checkbox("onLighting", &onLighting);									//光源処理フラグ
		//ImGui::Checkbox("onTransparent", &onTransparent);								//透過フラグ
		//ImGui::Checkbox("operationAlpha", &operationAlpha);							//透過値の操作有効フラグ

		//ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);					//透過値の操作有効フラグ
	}
#endif
}


//=============================================================================
// Getter
//=============================================================================
std::vector<Item*>& ItemManager::getList() { return itemList; }