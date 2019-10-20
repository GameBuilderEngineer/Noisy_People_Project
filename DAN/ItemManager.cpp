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
	itemList.reserve(INITIAL_RESERVE);
}


//=============================================================================
// 終了処理
//=============================================================================
void ItemManager::uninitialize()
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		SAFE_DELETE(itemList[i]);
	}

	std::vector<Item*> temp;
	itemList.swap(temp);
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
}


//=============================================================================
// 描画処理
//=============================================================================
void ItemManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		itemList[i]->render(view, projection, cameraPosition);
	}
}


//=============================================================================
// アイテムオブジェクトの作成
//=============================================================================
void ItemManager::createItem(ItemData* itemData)
{
	switch (itemData->itemType)
	{
	case BATTERY:
		itemList.emplace_back(new Battery(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS), itemData));
		break;
	case EXAMPLE:
		itemList.emplace_back(new exampleItem(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2), itemData));
		break;
	}
}


//=============================================================================
// アイテムオブジェクトの破棄
//=============================================================================
void ItemManager::destroyItem(int _id)
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		if (itemList[i]->getItemData()->id == _id)
		{
			delete itemList[i];
			itemList.erase(itemList.begin() + i);
		}
	}
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