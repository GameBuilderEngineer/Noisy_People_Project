//-----------------------------------------------------------------------------
// ツリー管理クラス [Tree.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshObject.h"
#include "Tree.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace treeNS
{	
}


//=============================================================================
//クラス定義
//=============================================================================
class TreeManager
{
private:
	std::vector<Tree*> treeList;				// ツリーポインタリスト
	StaticMeshObject* aTrunkRenderer;			// Aモデル幹描画オブジェクト
	StaticMeshObject* aLeafRenderer;			// Aモデル葉描画オブジェクト
	StaticMeshObject* bTrunkRenderer;			// Bモデル幹描画オブジェクト
	StaticMeshObject* bLeafRenderer;			// Bモデル葉描画オブジェクト
	StaticMeshObject* cTrunkRenderer;			// Cモデル幹描画オブジェクト
	StaticMeshObject* cLeafRenderer;			// Cモデル葉描画オブジェクト
	int nextID;									// 次回ツリー発行ID

public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createTree(treeNS::TreeData treeData);
	void createLeaf(Object* leaf, int _model);
	void destroyLeaf(Object* leaf, int _model);
	void destroyAllTree();
	void outputGUI();
};
