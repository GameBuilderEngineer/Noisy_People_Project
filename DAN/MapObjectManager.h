//-----------------------------------------------------------------------------
// マップオブジェクト管理クラス [MapObject.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/11/12
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshRenderer.h"
#include "MapObject.h"

//=============================================================================
// 名前空間
//=============================================================================


//=============================================================================
//クラス定義
//=============================================================================
class MapObjectManager
{
private:
	std::vector<MapObject*> mapObjectList;		// ツリーポインタリスト
	StaticMeshRenderer* stone01Renderer;		// 岩01描画オブジェクト
	int nextID;									// 次回発行マップオブジェクトID
	LPD3DXMESH	attractorMesh;					// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;				// 重力（引力）発生オブジェクトマトリックス

public:
	void initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createMapObject(mapObjectNS::MapObjectData treeData);
	void destroyAllMapObject();
	int issueNewMapObjectID();
	void outputGUI();

	// Getter
	std::vector<MapObject*>& getMapObjectList();
};
