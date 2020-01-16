//-----------------------------------------------------------------------------
// メッシュデータクラス [MeshData.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/20
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshLoader.h"
#include "VertexAccessor.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace meshDataNS
{
	// ポリゴン面ノード用頂点構造体
	struct VertexForFaceNode
	{
		BYTE* data;						// 頂点データ
		WORD index;						// 頂点インデックス
		D3DXVECTOR3	position;			// 頂点座標
	};

	// ポリゴン面構造体
	struct Face
	{
		VertexForFaceNode vtx[3];		// 頂点構造体配列
		D3DXVECTOR3 nor;				// 法線
		DWORD adjacency[3];				// 隣接ポリゴン面インデックス配列
	};

	// インデックス2構造体
	struct Index2
	{
		WORD index[2];					// インデックス配列
	};
}


//=============================================================================
//クラス定義
//=============================================================================
class MeshData
{
private:
	StaticMesh* staticMesh;				// メッシュ情報
	VertexAccessor*	vtxAccessor;		// 頂点アクセスオブジェクト
	BYTE* vtxArray;						// 頂点情報配列
	WORD* indexArray;					// 頂点インデックス配列
	DWORD* adjacencyArray;				// 隣接ポリゴンインデックス配列
	meshDataNS::Face* faceArray;		// ポリゴン面配列
	DWORD numVertices;					// 頂点数
	DWORD stride;						// 1頂点あたりのバイト数
	DWORD numFaces;						// 3角形ポリゴン面の数
	DWORD fvf;							// FVF(柔軟な頂点フォーマット)


public:
	// 初期化
	void initialize(StaticMesh* _staticMesh, VertexAccessor* _vtxAccessor);
	// 終了処理
	void uninitialize();
	// 頂点インデックスから頂点を取得
	BYTE* getVertexPointerFromVertexIndex(WORD number);
	BYTE* getVertexPointerFromVertexIndex(WORD number, BYTE* vtx);
	// 面インデックスから頂点インデックスを取得
	WORD getVertexIndexFromFaceIndex(DWORD number, int selecter);
	WORD getVertexIndexFromFaceIndex(DWORD number, int selecter, WORD* vertexIndex);
	// 面インデックスから隣接面インデックスを取得
	DWORD getAdjacentFaceIndexFromFaceIndex(DWORD number, int selecter);
	DWORD getAdjacentFaceIndexFromFaceIndex(DWORD number, int selecter, DWORD* adjacency);
	// 2つの面インデックスから共通の頂点インデックスを取り出す
	meshDataNS::Index2 getSharingVertexIndexFromTwoFace(DWORD number1, DWORD number2);

	// Getter
	StaticMesh* getStaticMesh();		// メッシュ情報を取得
	BYTE* getVtxArray();				// 頂点情報配列を取得
	WORD* getIndexArray();				// 頂点インデックス配列を取得
	DWORD* getAdjacencyArray();			// 隣接ポリゴンインデックス配列を取得
	meshDataNS::Face* getFaceArray();	// 面配列を取得
	DWORD getNumVertices();				// 頂点数を取得
	DWORD getStride();					// 1頂点あたりのバイト数を取得
	DWORD getNumFaces();				// 3角形ポリゴンの面の数を取得
	DWORD getFVF();						// 頂点フォーマットを取得
};