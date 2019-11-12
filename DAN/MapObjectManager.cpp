//-----------------------------------------------------------------------------
// マップオブジェクト管理クラス [MapObject.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/11/12
//-----------------------------------------------------------------------------
#include "MapObjectManager.h"
#include "ImguiManager.h"
using namespace mapObjectNS;


//=============================================================================
// 初期化
//=============================================================================
void MapObjectManager::initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	nextID = 0;			// 次回発行IDを0に初期化

	// 接地フィールドをセット
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	// 描画オブジェクトの作成
	stone01Renderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::STONE_003));


#if 0	// マップオブジェクトツールのデータを読み込む

#endif
}


//=============================================================================
// 終了処理
//=============================================================================
void MapObjectManager::uninitialize()
{
	// 全ツリーオブジェクトを破棄
	destroyAllMapObject();

	// ベクターの確保メモリを初期化（メモリアロケータだけに戻す）
	std::vector<MapObject*> temp;
	mapObjectList.swap(temp);

	// 描画オブジェクトの破棄
	SAFE_DELETE(stone01Renderer);
}


//=============================================================================
// 更新処理
//=============================================================================
void MapObjectManager::update(float frameTime)
{
	for (size_t i = 0; i < mapObjectList.size(); i++)
	{
		mapObjectList[i]->update(frameTime);
	}

	stone01Renderer->update();
}


//=============================================================================
// 描画処理
//=============================================================================
void MapObjectManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	stone01Renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// マップオブジェクトの作成
//=============================================================================
void MapObjectManager::createMapObject(MapObjectData mapObjectData)
{
	MapObject* mapObject = new MapObject(staticMeshNS::reference(staticMeshNS::STONE_003), mapObjectData);

	// マップオブジェクトの描画をセット
	switch (mapObjectData.type)
	{
	case STONE_01:
		mapObject->scale *= 0.1f;// サイズの調整
		stone01Renderer->registerObject(mapObject);
		break;
	}

	mapObject->setAttractor(attractorMesh, attractorMatrix);
	mapObjectList.push_back(mapObject);
}


//=============================================================================
// 全マップオブジェクトの破棄
//=============================================================================
void MapObjectManager::destroyAllMapObject()
{
	// 描画全解除
	stone01Renderer->allUnRegister();


	for (int i = 0; i < mapObjectList.size(); i++)
	{
		SAFE_DELETE(mapObjectList[i]);
	}
	mapObjectList.clear();
}


//=============================================================================
// ツリーIDを発行する
//=============================================================================
int MapObjectManager::issueNewMapObjectID()
{
	int ans = nextID;
	nextID++;
	return ans;
}


//=============================================================================
// ImGuiに出力
//=============================================================================
void MapObjectManager::outputGUI()
{
#ifdef _DEBUG

#endif
}


//=============================================================================
// Getter
//=============================================================================
std::vector<MapObject*>& MapObjectManager::getMapObjectList(){ return mapObjectList; }
