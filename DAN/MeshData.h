//-----------------------------------------------------------------------------
// メッシュデータクラス [MeshData.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/20
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshLoader.h"


//=============================================================================
//クラス定義
//=============================================================================
class MeshData
{
private:
	StaticMesh* staticMesh;				// メッシュ情報
	BYTE* vtxArray;						// 頂点情報配列
	WORD* indexArray;					// 頂点インデックス配列
	DWORD* adjacencyArray;				// 隣接ポリゴンインデックス配列
	DWORD numVertices;					// 頂点数
	DWORD stride;						// 1頂点あたりのバイト数
	DWORD numFaces;						// 3角形ポリゴン面の数

public:
	// 初期化
	void initialize(StaticMesh* _staticMesh);
	// 終了処理
	void uninitialize();
	// 頂点インデックスから頂点を取得
	BYTE* getVertexPointerFromVertexIndex(BYTE* vtx, WORD number);
	// 面インデックスから頂点インデックスを取得
	WORD* getVertexIndexPointerFromFaceIndex(WORD* vertexIndex, DWORD number);
	// 面インデックスから頂点を取得
	BYTE* getVertexPointerFromFaceIndex(BYTE* vtx, WORD* vertexIndex, DWORD number);
	// 面インデックスから隣接面インデックスを取得
	DWORD* getAdjacentFaceIndexPointerFromFaceIndex(DWORD* adjacency, DWORD number);

	// Getter
	StaticMesh* getStaticMesh();		// メッシュ情報を取得
	BYTE* getVtxArray();				// 頂点情報配列を取得
	WORD* getIndexArray();				// 頂点インデックス配列を取得
	DWORD* getAdjacencyArray();			// 隣接ポリゴンインデックス配列を取得
	DWORD getNumVertices();				// 頂点数を取得
	DWORD getStride();					// 1頂点あたりのバイト数を取得
	DWORD getNumFaces();				// 3角形ポリゴンの面の数を取得
};