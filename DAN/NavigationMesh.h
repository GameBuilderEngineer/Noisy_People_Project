//-----------------------------------------------------------------------------
// ナビゲーションメッシュ処理クラス [NavigationMesh.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/5
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "Object.h"
#include "VertexAccessor.h"
#include "MeshData.h"
#include "AStar.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace navigationMeshNS
{
	struct Edge
	{
		D3DXVECTOR3 v0;		
		D3DXVECTOR3 v1;
	};

}


//=============================================================================
//クラス定義
//=============================================================================
class NavigationMesh: public Object
{
private:
	StaticMesh* staticMesh;						// メッシュ情報
	MeshData meshData;							// メッシュデータオブジェクト
	VertexAccessor vtxAccessor;					// 頂点アクセスオブジェクト
	AStar aStar;								// パス検索オブジェクト


public:
	NavigationMesh(StaticMesh* staticMesh);
	void initialize();
	void uninitialize();
	void update();

	// 探索実行前に現在地ポリゴンと目的地ポリゴンを調整するフラグ
	enum SEARCH_FLAG
	{
		STANDARD,	// 指定座標に最も近いポリゴン
		GRAVITY		// 指定座標から重力方向にレイキャストして衝突したポリゴン
	};

	// 経路探索
	void pathSearch(D3DXVECTOR3 from, D3DXVECTOR3 dest, SEARCH_FLAG flag);

	// ステアリング
	void steering();


#ifdef _DEBUG
	void debugRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void outputGUI();
#endif // _DEBUG
};
