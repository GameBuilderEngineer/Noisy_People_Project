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

	// 描画オブジェクトの作成
	//デジタルツリー
	aDTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_TRUNK));
	aDLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_LEAF));
	bDTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_TRUNK));
	bDLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_LEAF));
	cDTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	cDLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DEAD_TREE));

	//アナログツリー(葉)
	//透過処理を有効にする
	aLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//デジタルツリー(葉)
	//透過処理を有効にする
	aDLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bDLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cDLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//デジタルツリーエフェクト
	digitalTreeEffect = new DigitalTreeEffect();
	playedDigitalTreeEffect[gameMasterNS::PLAYER_1P] = false;
	playedDigitalTreeEffect[gameMasterNS::PLAYER_2P] = false;
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
	//アナログツリー
	SAFE_DELETE(cLeafRenderer);
	SAFE_DELETE(cTrunkRenderer);
	SAFE_DELETE(bLeafRenderer);
	SAFE_DELETE(bTrunkRenderer);
	SAFE_DELETE(aLeafRenderer);
	SAFE_DELETE(aTrunkRenderer);
	//デジタルツリー
	SAFE_DELETE(cDLeafRenderer);
	SAFE_DELETE(cDTrunkRenderer);
	SAFE_DELETE(bDLeafRenderer);
	SAFE_DELETE(bDTrunkRenderer);
	SAFE_DELETE(aDLeafRenderer);
	SAFE_DELETE(aDTrunkRenderer);

	//デジタルツリーエフェクト
	SAFE_DELETE(digitalTreeEffect);
}


//=============================================================================
// 更新処理
//=============================================================================
void TreeManager::update(float frameTime)
{
	//緑化した本数を数えなおす
	greeningTreeNum = 0;
	//各ツリーの更新
	for (size_t i = 0; i < treeList.size(); i++)
	{
		Tree* tree = treeList[i];

		bool needSwap = false;
		int beforeType = tree->getTreeData()->type;
		//状態遷移を行おうとしている場合
		if (tree->getTransState())
		{
			//レンダラーの切替を行う
			needSwap = true;
		}

		//更新
		tree->update(frameTime);

		//レンダラーの切替
		if (needSwap)
		{
			swapDA(tree,beforeType);
		}

		//緑化している木をカウント
		if (treeList[i]->getTreeData()->greenState == treeNS::GREEN)
		{
			greeningTreeNum++;
		}

		//デジタルツリーの周囲にエフェクトを発生
		//if (treeList[i]->getTreeData()->type == treeNS::DIGITAL_TREE)
		//{
		//	//エフェクトの生成
		//	digitalTreeEffect->generateInstance(treeList[i]->position);
		//}
	}



	//緑化率の更新
	if (treeList.size() != 0) {
		greeningRate = (float)greeningTreeNum / (float)treeList.size();
	}


	//レンダラーの更新
	aTrunkRenderer->update();
	aLeafRenderer->update();
	bTrunkRenderer->update();
	bLeafRenderer->update();
	cTrunkRenderer->update();
	cLeafRenderer->update();

	//レンダラーの更新
	aDTrunkRenderer->update();
	aDLeafRenderer->update();
	bDTrunkRenderer->update();
	bDLeafRenderer->update();
	cDTrunkRenderer->update();
	cDLeafRenderer->update();

	//デジタルツリーエフェクトの更新
	digitalTreeEffect->update(frameTime);

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

	//アナログツリー
	aTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	aLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);

	//デジタルツリー
	aDTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	aDLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bDTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bDLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cDTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cDLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);

	//デジタルツリーエフェクトの描画
	digitalTreeEffect->render(view, projection, cameraPosition);
}

//=============================================================================
//【デジタルツリー用のエフェクトを再生する】
//=============================================================================
void TreeManager::playDigitalTreeEffect(int playerNo)
{
	if (playedDigitalTreeEffect[playerNo] == true)return;

	playedDigitalTreeEffect[playerNo] = true;

	for (size_t i = 0; i < treeList.size(); i++)
	{
		Tree* tree = treeList[i];
		if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
		{
			digitalTreeEffect->playStandardEffect(&tree->position);
		}
	}

}

//=============================================================================
//【デジタルツリー用のエフェク停止停する】
//=============================================================================
void TreeManager::stopDigitalTreeEffect(int playerNo)
{
	if (playedDigitalTreeEffect[playerNo] == false)return;

	playedDigitalTreeEffect[playerNo] = false;

	for (size_t i = 0; i < treeList.size(); i++)
	{
		Tree* tree = treeList[i];
		if (tree->getTreeData()->type = treeNS::DIGITAL_TREE)
		{
			digitalTreeEffect->playStandardEffect(&tree->position);
		}
	}
}


//=============================================================================
//【通常ビューに切り替える】
//=============================================================================
void TreeManager::switchingNormalView(int playerNo)
{
	//アナログツリーの透過値の設定
	aTrunkRenderer->setAlpha(1.0f);
	aLeafRenderer->setAlpha(1.0f);
	bTrunkRenderer->setAlpha(1.0f);
	bLeafRenderer->setAlpha(1.0f);
	cTrunkRenderer->setAlpha(1.0f);
	cLeafRenderer->setAlpha(1.0f);

	//アナログツリー
	aTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	aLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	bLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	cLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//デジタルツリー
	aDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	aDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	bDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	cDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//デジタルツリーエフェクトの非表示
	//for (size_t i = 0; i < treeList.size(); i++)
	//{
	//	Tree* tree = treeList[i];
	//	if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
	//	{
	//		//本来は、近い場合のみデジタルツリーエフェクトを表示する
	//
	//		//通常描画時は描画しない
	//		tree->switchingShownDigitalEffect(true, playerNo);
	//
	//
	//	}
	//}
};

//=============================================================================
//【ビジョンビューに切り替える】
//=============================================================================
void TreeManager::switchingVisionView(int playerNo)
{

	//アナログツリーの透過値の設定
	aTrunkRenderer->setAlpha(0.3f);
	aLeafRenderer->setAlpha(0.3f);
	bTrunkRenderer->setAlpha(0.3f);
	bLeafRenderer->setAlpha(0.3f);
	cTrunkRenderer->setAlpha(0.3f);
	cLeafRenderer->setAlpha(0.3f);
	//アナログツリー
	aTrunkRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	aLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bTrunkRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cTrunkRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//デジタルツリー
	aDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	aDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	bDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	cDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//デジタルツリーエフェクトの表示
	//for (size_t i = 0; i < treeList.size(); i++)
	//{
	//	Tree* tree = treeList[i];
	//	if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
	//	{
	//		if (tree->getSelected(playerNo))
	//		{
	//			//選択されている場合は、ライトを表示しない
	//			tree->switchingShownDigitalEffect(true,playerNo);
	//		}
	//		else {
	//			//選択されていなければデジタルツリーエフェクトを表示する
	//			tree->switchingShownDigitalEffect(false,playerNo);//falseで最前面ライト
	//		}
	//	}
	//}

};

//=============================================================================
//【ワイヤーフレーム描画に切り替える】
//=============================================================================
void TreeManager::changeWireFrame()
{
	aTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	aLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);
	bTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	bLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);
	cTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	cLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);
	aTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	aLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);
	bTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	bLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);
	cTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	cLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);

};

//=============================================================================
//【ソリッド描画に切り替える】
//=============================================================================
void TreeManager::changeSolid()
{
	aTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	aLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
	bTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	bLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
	cTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	cLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
	aTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	aLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
	bTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	bLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
	cTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	cLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
};

//=============================================================================
// ツリーオブジェクトの作成
//=============================================================================
void TreeManager::createTree(TreeData treeData)
{
	Tree* tree = new Tree(treeData);	// ツリー作成
	
	//アナログツリーレンダラーへ登録
	registerAnalog(tree);

	tree->setAttractor(attractorMesh, attractorMatrix);
	treeList.push_back(tree);
}

//=============================================================================
//【(アナログ<->デジタル)レンダラーの交換】
//=============================================================================
void TreeManager::swapDA(Tree* tree,int beforeType)
{
	if (tree->getTreeData()->type == beforeType)
	{
		switch (tree->getTreeData()->type)
		{
		case ANALOG_TREE:
			unRegisterAnalog(tree);
			registerAnalog(tree);
			break;
		case DIGITAL_TREE:
			unRegisterDigital(tree);
			registerDigital(tree);
			break;
		}
		return;
	}

	switch (tree->getTreeData()->type)
	{
	case ANALOG_TREE:
		registerAnalog(tree);
		unRegisterDigital(tree);
		break;
	case DIGITAL_TREE:
		registerDigital(tree);
		unRegisterAnalog(tree);
		break;
	}
}


//=============================================================================
//【デジタルツリー描画登録】
//=============================================================================
void TreeManager::registerDigital(Tree* tree)
{
	//幹の登録
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aDTrunkRenderer->registerObject(tree);break;
	case B_MODEL:bDTrunkRenderer->registerObject(tree);break;
	case C_MODEL:cDTrunkRenderer->registerObject(tree);break;
	}

	//葉の登録
	tree->getLeaf()->onActive = true;
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aDLeafRenderer->registerObject(tree->getLeaf());break;
	case B_MODEL:bDLeafRenderer->registerObject(tree->getLeaf());break;
	case C_MODEL:cDLeafRenderer->registerObject(tree->getLeaf());break;
	}
}

//=============================================================================
//【デジタルツリー描画解除】
//=============================================================================
void TreeManager::unRegisterDigital(Tree* tree)
{
	//幹の解除
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aDTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case B_MODEL:bDTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case C_MODEL:cDTrunkRenderer->unRegisterObjectByID(tree->id); break;
	}

	//葉の解除
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aDLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
	case B_MODEL:bDLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
	case C_MODEL:cDLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
	}
}

//=============================================================================
//【アナログツリー描画登録】
//=============================================================================
void TreeManager::registerAnalog(Tree* tree)
{
	//幹の登録
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aTrunkRenderer->registerObject(tree);break;
	case B_MODEL:bTrunkRenderer->registerObject(tree);break;
	case C_MODEL:cTrunkRenderer->registerObject(tree);break;
	}

	//葉の登録
	if (tree->getTreeData()->greenState == GREEN) {
		tree->getLeaf()->onActive = true;
		switch (tree->getTreeData()->model)
		{
		case A_MODEL:aLeafRenderer->registerObject(tree->getLeaf());break;
		case B_MODEL:bLeafRenderer->registerObject(tree->getLeaf());break;
		case C_MODEL:cLeafRenderer->registerObject(tree->getLeaf());break;
		}
	}

}

//=============================================================================
//【アナログツリー描画解除】
//=============================================================================
void TreeManager::unRegisterAnalog(Tree* tree)
{
	//幹の解除
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case B_MODEL:bTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case C_MODEL:cTrunkRenderer->unRegisterObjectByID(tree->id); break;
	}

	//葉の解除
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
	case B_MODEL:bLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
	case C_MODEL:cLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
	}
}

//=============================================================================
// リーフ描画登録
//=============================================================================
void TreeManager::registerLeafRendering(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:	aLeafRenderer->registerObject(leaf);	break;
	case B_MODEL:	bLeafRenderer->registerObject(leaf);	break;
	case C_MODEL:	cLeafRenderer->registerObject(leaf);	break;
	}
}

//=============================================================================
// リーフ描画解除
//=============================================================================
void TreeManager::unRegisterLeafRendering(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:	aLeafRenderer->unRegisterObjectByID(leaf->id);	break;
	case B_MODEL:	bLeafRenderer->unRegisterObjectByID(leaf->id);	break;
	case C_MODEL:	cLeafRenderer->unRegisterObjectByID(leaf->id);	break;
	}
}


//=============================================================================
// 全ツリーオブジェクトの破棄
//=============================================================================
void TreeManager::destroyAllTree()
{
	// 描画全解除
	//アナログ
	aLeafRenderer->allUnRegister();
	aTrunkRenderer->allUnRegister();
	bLeafRenderer->allUnRegister();
	bTrunkRenderer->allUnRegister();
	cLeafRenderer->allUnRegister();
	cTrunkRenderer->allUnRegister();

	//デジタル
	aDLeafRenderer->allUnRegister();
	aDTrunkRenderer->allUnRegister();
	bDLeafRenderer->allUnRegister();
	bDTrunkRenderer->allUnRegister();
	cDLeafRenderer->allUnRegister();
	cDTrunkRenderer->allUnRegister();

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

		ImGui::Text("numOfTree:%d", Tree::getNumOfTree());
		ImGui::Text("greeningRate:%.02f%", greeningRate*100.0f);
		
		ImGui::Text("AnalogTrunkA[Num:%d]",	aTrunkRenderer->getObjectNum());
		ImGui::Text("AnalogTrunkB[Num:%d]",	bTrunkRenderer->getObjectNum());
		ImGui::Text("AnalogTrunkC[Num:%d]",	cTrunkRenderer->getObjectNum());
		ImGui::Text("AnalogLeafA[Num:%d]",	aLeafRenderer->getObjectNum());
		ImGui::Text("AnalogLeafB[Num:%d]",	bLeafRenderer->getObjectNum());
		ImGui::Text("AnalogLeafC[Num:%d]",	cLeafRenderer->getObjectNum());

		ImGui::Text("DigitalTrunkA[Num:%d]", aDTrunkRenderer->getObjectNum());
		ImGui::Text("DigitalTrunkB[Num:%d]", bDTrunkRenderer->getObjectNum());
		ImGui::Text("DigitalTrunkC[Num:%d]", cDTrunkRenderer->getObjectNum());
		ImGui::Text("DigitalLeafA[Num:%d]", aDLeafRenderer->getObjectNum());
		ImGui::Text("DigitalLeafB[Num:%d]", bDLeafRenderer->getObjectNum());
		ImGui::Text("DigitalLeafC[Num:%d]", cDLeafRenderer->getObjectNum());

		bTrunkRenderer->outputGUI();

	}
#endif
}


//=============================================================================
// Getter
//=============================================================================
std::vector<Tree*>& TreeManager::getTreeList(){ return treeList; }
float TreeManager::getGreeningRate() { return greeningRate; }
