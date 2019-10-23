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
	const int INITIAL_RESERVE = 500;			// ツリーポインタの初期確保数
}


//=============================================================================
//クラス定義
//=============================================================================
class TreeManager
{
private:
	std::vector<Tree*> treeList;				// ツリーポインタリスト
	StaticMeshRenderer* greenA;
	StaticMeshRenderer* deadA;
	StaticMeshRenderer* greenB;
	StaticMeshRenderer* deadB;

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
