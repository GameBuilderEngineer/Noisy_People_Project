//-----------------------------------------------------------------------------
// ナビゲーションメッシュ処理クラス [NavigationMesh.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/5
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshObject.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace enemyNS
{

}


//=============================================================================
//クラス定義
//=============================================================================
class NavigationMesh: public StaticMeshObject
{
private:
	StaticMesh* staticMesh;						// ナビゲーション用メッシュ
	LPDIRECT3DVERTEXBUFFER9	vtxBuffer;			// 頂点バッファ
	LPDIRECT3DINDEXBUFFER9	indexBuffer;		// インデックスバッファ
	DWORD *adjacency;							// 隣接性バッファ

	D3DINDEXBUFFER_DESC desc;



public:
	//NavigationMesh();
	//~NavigationMesh();

	void initialize();
	void uninitialize();
	void update();

#ifdef _DEBUG
	void debugRender();
#endif // _DEBUG

};
