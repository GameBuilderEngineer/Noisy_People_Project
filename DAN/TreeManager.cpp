//-----------------------------------------------------------------------------
// ツリー管理クラス [Tree.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#include "TreeManager.h"
#include "ImguiManager.h"
using namespace treeNS;


//=============================================================================
// 初期化
//=============================================================================
void TreeManager::initialize()
{
	nextID = 0;								// 次回発行IDを0に初期化

	// 描画オブジェクトの作成
	aTrunkRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_001));
	aLeafRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_PLAYSTATION));
	bTrunkRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	bLeafRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
	cTrunkRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	cLeafRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));

#if 0
#endif
}


//=============================================================================
// 終了処理
//=============================================================================
void TreeManager::uninitialize()
{
	// 全ツリーオブジェクトを破棄
	destroyAllTree();

	// ベクターの確保メモリを初期化（メモリアロケータだけに戻す）
	std::vector<Tree*> temp;
	treeList.swap(temp);

	// 描画オブジェクトの破棄
	SAFE_DELETE(aTrunkRenderer);
	SAFE_DELETE(aLeafRenderer);
	SAFE_DELETE(bTrunkRenderer);
	SAFE_DELETE(bLeafRenderer);
	SAFE_DELETE(cTrunkRenderer);
	SAFE_DELETE(cLeafRenderer);
}


//=============================================================================
// 更新処理
//=============================================================================
void TreeManager::update(float frameTime)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		treeList[i]->update(frameTime);
	}

	aTrunkRenderer->update();
	aLeafRenderer->update();
	bTrunkRenderer->update();
	bLeafRenderer->update();
	cTrunkRenderer->update();
	cLeafRenderer->update();
}


//=============================================================================
// 描画処理
//=============================================================================
void TreeManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	aTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	aLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// ツリーオブジェクトの作成
//=============================================================================
void TreeManager::createTree(TreeData treeData)
{
	Tree* tree = new Tree(treeData);	// ツリー作成

	// 幹を作成
	tree->setTrunk(new Object);
	switch (treeData.model)
	{
	case A_MODEL:
		aTrunkRenderer->generateObject(tree->getTrunk());
		break;

	case B_MODEL:
		bTrunkRenderer->generateObject(tree->getTrunk());
		break;

	case C_MODEL:
		cTrunkRenderer->generateObject(tree->getTrunk());
		break;
	}

	// リーフ作成
	if (treeData.geenState == GREEN)
	{
		createLeaf(tree->getLeaf(), treeData.model);
	}

	treeList.push_back(tree);
}


//=============================================================================
// リーフ作成
//=============================================================================
void TreeManager::createLeaf(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:
		leaf = new Object;
		aLeafRenderer->generateObject(leaf);
		break;

	case B_MODEL:
		leaf = new Object;
		bLeafRenderer->generateObject(leaf);
		break;

	case C_MODEL:
		leaf = new Object;
		cLeafRenderer->generateObject(leaf);
		break;
	}
}


//=============================================================================
// リーフ破棄
//=============================================================================
void TreeManager::destroyLeaf(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:
		aLeafRenderer->deleteObjectByID(leaf->id);
		break;

	case B_MODEL:
		aLeafRenderer->deleteObjectByID(leaf->id);
		break;

	case C_MODEL:
		aLeafRenderer->deleteObjectByID(leaf->id);
		break;
	}
}


//=============================================================================
// 全ツリーオブジェクトの破棄
//=============================================================================
void TreeManager::destroyAllTree()
{
	aLeafRenderer->allDelete();
	aTrunkRenderer->allDelete();
	bLeafRenderer->allDelete();
	bTrunkRenderer->allDelete();
	cLeafRenderer->allDelete();
	cTrunkRenderer->allDelete();
	
	for (int i = 0; i < treeList.size(); i++)
	{
		SAFE_DELETE(treeList[i]);
	}
	treeList.clear();
}



//=============================================================================
// ImGuiに出力
//=============================================================================
void TreeManager::outputGUI()
{
#ifdef _DEBUG

	if (ImGui::CollapsingHeader("TreeInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		//float limitTop = 1000;
		//float limitBottom = -1000;

		ImGui::Text("numOfTree:%d", Tree::getNumOfTree());

	}

#endif
}