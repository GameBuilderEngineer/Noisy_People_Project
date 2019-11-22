//-----------------------------------------------------------------------------
// ツリー管理クラス [Tree.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
// 更新日 : 2019/11/12 【菅野 樹】
//-----------------------------------------------------------------------------
#include "TreeManager.h"
#include "ImguiManager.h"
using namespace treeNS;


//=============================================================================
// 初期化
//=============================================================================
void TreeManager::initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	nextID = 0;			// 次回発行IDを0に初期化

	// 接地フィールドをセット
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	// 描画オブジェクトの作成
	aTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_TRUNK));
	aLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_LEAF));
	bTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_TRUNK));
	bLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_LEAF));
	cTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	cLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DEAD_TREE));

	//葉のライトを切る
	aLeafRenderer->disableLight();
	bLeafRenderer->disableLight();
	cLeafRenderer->disableLight();
	//透過処理を有効にする
	aLeafRenderer->enableTransparent();
	bLeafRenderer->enableTransparent();
	cLeafRenderer->enableTransparent();

#if 0	// ツリーツールのデータを読み込む

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
	SAFE_DELETE(cLeafRenderer);
	SAFE_DELETE(cTrunkRenderer);
	SAFE_DELETE(bLeafRenderer);
	SAFE_DELETE(bTrunkRenderer);
	SAFE_DELETE(aLeafRenderer);
	SAFE_DELETE(aTrunkRenderer);
}


//=============================================================================
// 更新処理
//=============================================================================
void TreeManager::update(float frameTime)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		treeList[i]->update(frameTime);

		//葉の描画状態を切り替える
		if (treeList[i]->getLeaf()->onActive) 
		{//描画されている場合
			//枯木状態
			if (treeList[i]->getTreeData()->greenState == treeNS::DEAD)
			{
				treeList[i]->getLeaf()->onActive = false;//アクティブ化
				//レンダラーへ登録
				unRegisterLeafRendering(
					treeList[i]->getLeaf(),				//葉オブジェクト
					treeList[i]->getTreeData()->model);	//モデル情報
			}
		}
		else 
		{//描画されていない場合
			//緑化状態
			if (treeList[i]->getTreeData()->greenState == treeNS::GREEN)
			{
				treeList[i]->getLeaf()->onActive = true;//アクティブ化
				//レンダラーへ登録
				registerLeafRendering(
					treeList[i]->getLeaf(),				//葉オブジェクト
					treeList[i]->getTreeData()->model);	//モデル情報
				
			}
		}
	}

	//レンダラーの更新
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
#ifdef _DEBUG
	for (size_t i = 0; i < treeList.size(); i++)
	{
		treeList[i]->render();
	}
#endif // _DEBUG

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

	// 幹の描画をセット
	switch (treeData.model)
	{
	case A_MODEL:
		aTrunkRenderer->registerObject(tree->getTrunk());
		break;

	case B_MODEL:
		bTrunkRenderer->registerObject(tree->getTrunk());
		break;

	case C_MODEL:
		cTrunkRenderer->registerObject(tree->getTrunk());
		break;
	}

	// 葉の描画をセット
	if (treeData.greenState == GREEN)
	{
		switch (treeData.model)
		{
		case A_MODEL:
			aLeafRenderer->registerObject(tree->getLeaf());
			break;

		case B_MODEL:
			bLeafRenderer->registerObject(tree->getLeaf());
			break;

		case C_MODEL:
			cLeafRenderer->registerObject(tree->getLeaf());
			break;
		}
	}
	tree->setAttractor(attractorMesh, attractorMatrix);
	treeList.push_back(tree);
}


//=============================================================================
// リーフ描画登録
//=============================================================================
void TreeManager::registerLeafRendering(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:
		aLeafRenderer->registerObject(leaf);
		break;

	case B_MODEL:
		bLeafRenderer->registerObject(leaf);
		break;

	case C_MODEL:
		cLeafRenderer->registerObject(leaf);
		break;
	}
}


//=============================================================================
// リーフ描画解除
//=============================================================================
void TreeManager::unRegisterLeafRendering(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:
		aLeafRenderer->unRegisterObjectByID(leaf->id);
		break;

	case B_MODEL:
		aLeafRenderer->unRegisterObjectByID(leaf->id);
		break;

	case C_MODEL:
		aLeafRenderer->unRegisterObjectByID(leaf->id);
		break;
	}
}


//=============================================================================
// 全ツリーオブジェクトの破棄
//=============================================================================
void TreeManager::destroyAllTree()
{
	// 描画全解除
	aLeafRenderer->allUnRegister();
	aTrunkRenderer->allUnRegister();
	bLeafRenderer->allUnRegister();
	bTrunkRenderer->allUnRegister();
	cLeafRenderer->allUnRegister();
	cTrunkRenderer->allUnRegister();

	for (int i = 0; i < treeList.size(); i++)
	{
		SAFE_DELETE(treeList[i]);
	}
	treeList.clear();
}


//=============================================================================
// ツリーIDを発行する
//=============================================================================
int TreeManager::issueNewTreeID()
{
	int ans = nextID;
	nextID++;
	return ans;
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


//=============================================================================
// Getter
//=============================================================================
std::vector<Tree*>& TreeManager::getTreeList(){ return treeList; }
