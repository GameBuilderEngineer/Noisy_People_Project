//-----------------------------------------------------------------------------
// ツリー管理クラス [Tree.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
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
	StaticMeshObject* tree1Renderer;
	StaticMeshObject* leaf1Renderer;
	StaticMeshObject* tree2Renderer;
	StaticMeshObject* leaf2Renderer;
	StaticMeshObject* tree3Renderer;
	StaticMeshObject* leaf3Renderer;

	int nextID;									// 次回ツリー発行ID


public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createTree(treeNS::TreeData* treeData);
	void destroyTree(int _id);
	void destroyAllTree();
	void outputGUI();
};
