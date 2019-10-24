//-----------------------------------------------------------------------------
// ツリー管理クラス [Tree.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshRenderer.h"
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
	StaticMeshRenderer* aTrunkRenderer;			// Aモデル幹描画オブジェクト
	StaticMeshRenderer* aLeafRenderer;			// Aモデル葉描画オブジェクト
	StaticMeshRenderer* bTrunkRenderer;			// Bモデル幹描画オブジェクト
	StaticMeshRenderer* bLeafRenderer;			// Bモデル葉描画オブジェクト
	StaticMeshRenderer* cTrunkRenderer;			// Cモデル幹描画オブジェクト
	StaticMeshRenderer* cLeafRenderer;			// Cモデル葉描画オブジェクト
	int nextID;									// 次回ツリー発行ID

public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createTree(treeNS::TreeData treeData);
	void registerLeafRendering(Object* leaf, int _model);
	void unRegisterLeafRendering(Object* leaf, int _model);
	void destroyAllTree();
	int issueNewTreeID();
	void outputGUI();

	// Getter
	std::vector<Tree*>& getTreeList();
};
