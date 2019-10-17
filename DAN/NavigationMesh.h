//-----------------------------------------------------------------------------
// ナビゲーションメッシュ処理クラス [NavigationMesh.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/5
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "VertexAccessor.h"
#include "StaticMeshObject.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace navigationMeshNS
{
	struct VertexNaviMesh
	{
	};

}

struct Index3
{
	DWORD index[3];
};


//=============================================================================
//クラス定義
//=============================================================================
class NavigationMesh: public StaticMeshObject
{
private:
	LPDIRECT3DVERTEXBUFFER9	vertexBuffer;		// 頂点バッファ

	LPD3DXMESH mesh;							// メッシュ
	BYTE* vtx;									// 頂点情報
	WORD* index;								// (頂点)インデックス情報
	DWORD *adjacency;							// 隣接性情報
	

	VertexAccessor vtxAccessor;					// 頂点アクセス
	DWORD numVertices;
	DWORD stride;
	DWORD numFaces;

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
	void searchRoute(D3DXVECTOR3 from, D3DXVECTOR3 dest, SEARCH_FLAG flag);

	//BYTE* getVertexFromIndex(BYTE* pVtx, WORD index);

	//WORD getIndexFromPolygon();
	//BYTE* getVertexFromPolygon(DWORD polyIndex);
	//Index3 getAdjacencyFromPolygon(DWORD polyIndex);

#ifdef _DEBUG
	void debugRender();
	void outputGUI();
#endif // _DEBUG
};
