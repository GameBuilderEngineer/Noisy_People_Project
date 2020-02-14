//-----------------------------------------------------------------------------
// ツリー管理クラス [TreeManager.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
// 更新日 : 2019/12/22 【菅野 樹】
//-----------------------------------------------------------------------------

//=============================================================================
//【インクルード】
//=============================================================================
#include "TreeManager.h"
#include "ImguiManager.h"
#include "TreeTools.h"
#include "UtilityFunction.h"

//=============================================================================
//【using宣言】
//=============================================================================
using namespace treeNS;

//=============================================================================
//【初期化】
//=============================================================================
void TreeManager::initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	nextID = 0;			// 次回発行IDを0に初期化
	Tree::resetNumOfTree();//ツリーのカウントをリセット
	// 接地フィールドをセット
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	// 描画オブジェクトの作成
	aTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_TRUNK));
	aLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_LEAF));
	bTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_TRUNK));
	bLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_LEAF));
	cTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::C_TRUNK));
	cLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::C_LEAF));

	//アナログツリー(葉)
	//透過処理を有効にする
	aLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//デジタルツリーエフェクト
	treeEffect = new TreeEffect();
	playedDigitalTreeEffect[gameMasterNS::PLAYER_1P] = false;
	playedDigitalTreeEffect[gameMasterNS::PLAYER_2P] = false;

	gameMaster = NULL;

}

//=============================================================================
//【デストラクタ】
//=============================================================================
TreeManager::~TreeManager()
{
	uninitialize();
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

	//デジタルツリーエフェクト
	SAFE_DELETE(treeEffect);
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
		int beforeGreenState = tree->getTreeData()->greenState;
		//状態遷移を行おうとしている場合
		if (tree->getTransState())
		{
			//レンダラーの切替を行う
			needSwap = true;
		}

		//更新
		tree->update(frameTime);

		TreeData* data = tree->getTreeData();
		TreeTable in;
		in.id = data->treeID;
		in.position = tree->position;
		in.rotation = tree->quaternion;
		in.scale = tree->scale;
		in.modelType = data->model;
		in.playBacked = false;
		in.onRecord = true;
		//プレイヤーNOの記録
		in.player = tree->playerNo;

		//緑化状態
		in.greenState = data->greenState;
		
		//ネットワークの送信情報へ記録
		NETWORK_CLIENT::recordTreeTable(in,i);

		//ツリーのステート切替イベント発生
		if (needSwap)
		{
			//イベントのタイプの識別
			if (beforeGreenState == treeNS::DEAD)
			{
				switch (tree->getTreeData()->type)
				{
				case treeNS::DIGITAL_TREE:
					in.eventType = gameMasterNS::TO_GREEN_WITH_DIGITAL;
					break;
				case treeNS::ANALOG_TREE:
					in.eventType = gameMasterNS::TO_GREEN_WITH_ANALOG;
					break;
				}
			}
			else if (beforeGreenState == treeNS::GREEN){
				in.eventType = gameMasterNS::TO_DEAD;
			}

			//ゲームマスターへ記録
			if (gameMaster)
			{
				gameMaster->recordTreeTable(in);
			}
		}



		//緑化している木をカウント
		if (treeList[i]->getTreeData()->greenState == treeNS::GREEN)
		{
			greeningTreeNum++;
		}
	}


	//緑化率の更新
	if (treeList.size() != 0) {
		greeningRate = (float)greeningTreeNum / (float)treeList.size();
	}



	//デジタルツリーエフェクトの更新
	treeEffect->update(frameTime);

}

//=============================================================================
//【描画】
//=============================================================================
void TreeManager::render(Camera* camera)
{
#ifdef _DEBUG
	for (size_t i = 0; i < treeList.size(); i++)
	{
		treeList[i]->render();
	}
#endif // _DEBUG
	
	//全描画解除
	allUnRegister();

	//全描画登録
	allRegister();

	//アナログツリー
	aTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);
	aLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);
	bTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);
	bLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);
	cTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);
	cLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);

	//デジタルツリーエフェクトの描画
	treeEffect->render(camera->view, camera->projection, camera->position);
}

//=============================================================================
//【ツールデータを使用してツリーを作成】
//=============================================================================
void TreeManager::createUsingTool()
{
	TREE_TOOLS* treeTools = new TREE_TOOLS;
	for (int i = 0; i < treeTools->GetTreeMax(); i++)
	{
		createTree(treeTools->GetTreeSet(i));
	}
	SAFE_DELETE(treeTools);
}

//=============================================================================
//【ツリーオブジェクトの作成】
//=============================================================================
void TreeManager::createTree(TreeData treeData)
{
	Tree* tree = new Tree(treeData);	// ツリー作成

	//ビルボード（マーカー/標識）の作成
	treeEffect->generateInstance(TreeEffectNS::INSTANCE_MARKER, tree);
	treeEffect->generateInstance(TreeEffectNS::INSTANCE_SIGN, tree);

	tree->setAttractor(attractorMesh, attractorMatrix);
	treeList.push_back(tree);
	treeNum = tree->getNumOfTree();
}

//=============================================================================
//【全ツリーオブジェクトの破棄】
//=============================================================================
void TreeManager::destroyAllTree()
{
	// 描画全解除
	allUnRegister();

	//全削除
	for (int i = 0; i < treeList.size(); i++)
	{
		SAFE_DELETE(treeList[i]);
	}
	treeList.clear();
}

//=============================================================================
//【ツリー描画登録】
//=============================================================================
void TreeManager::registerTrunk(Tree* tree)
{
	//幹の登録
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aTrunkRenderer->registerObject(tree);break;
	case B_MODEL:bTrunkRenderer->registerObject(tree);break;
	case C_MODEL:cTrunkRenderer->registerObject(tree);break;
	}
}

//=============================================================================
//【ツリー描画解除】
//=============================================================================
void TreeManager::unRegisterTrunk(Tree* tree)
{
	//幹の解除
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case B_MODEL:bTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case C_MODEL:cTrunkRenderer->unRegisterObjectByID(tree->id); break;
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
// 全ツリーの描画登録
//=============================================================================
void TreeManager::allRegister()
{
	Tree* tree = NULL;
	TreeData data;
	for (int i = 0; i < treeList.size(); i++)
	{
		tree = treeList[i];
		data = *tree->getTreeData();

		//カリング
		if (tree->culling)continue;

		//幹の登録
		registerTrunk(tree);

		//葉の登録
		switch (data.type)
		{
		case treeNS::ANALOG_TREE:
			//葉の登録
			if(data.greenState == treeNS::GREEN)
				registerLeafRendering(tree, data.model);
			break;
		case treeNS::DIGITAL_TREE:
			//葉の登録
			registerLeafRendering(tree, data.model);
			break;
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
// 全ツリーの描画解除
//=============================================================================
void TreeManager::allUnRegister()
{
	//アナログ
	aLeafRenderer->allUnRegister();
	aTrunkRenderer->allUnRegister();
	bLeafRenderer->allUnRegister();
	bTrunkRenderer->allUnRegister();
	cLeafRenderer->allUnRegister();
	cTrunkRenderer->allUnRegister();

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
	aTrunkRenderer->setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	aLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bTrunkRenderer->setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	bLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cTrunkRenderer->setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	cLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	aTrunkRenderer->enableRender();
	aLeafRenderer->enableRender();
	bTrunkRenderer->enableRender();
	bLeafRenderer->enableRender();
	cTrunkRenderer->enableRender();
	cLeafRenderer->enableRender();


	//ツリーエフェクト
	treeEffect->disableRender();

	//デジタルツリーエフェクトの非表示
	for (size_t i = 0; i < treeList.size(); i++)
	{
		Tree* tree = treeList[i];
		if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
		{
			//通常描画時は描画しない
			tree->switchingShownDigitalEffect(false, playerNo);
		}
	}
};

//=============================================================================
//【ビジョンビューに切り替える】
//=============================================================================
void TreeManager::switchingVisionView(int playerNo)
{
	//描画しない
	aTrunkRenderer->disableRender();
	aLeafRenderer->disableRender();
	bTrunkRenderer->disableRender();
	bLeafRenderer->disableRender();
	cTrunkRenderer->disableRender();
	cLeafRenderer->disableRender();

	//ツリーエフェクト
	treeEffect->enableRender();

	//デジタルツリーエフェクトの表示
	for (size_t i = 0; i < treeList.size(); i++)
	{
		Tree* tree = treeList[i];
		if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
		{
			if (tree->getSelected(playerNo))
			{
				//選択されている場合は、ライトを表示しない
				tree->switchingShownDigitalEffect(false, playerNo);
			}
			else {
				//選択されていなければデジタルツリーエフェクトを表示する
				tree->switchingShownDigitalEffect(true, playerNo);
			}
		}
	}

};

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
			treeEffect->playStandardEffect(&tree->position);
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
			treeEffect->playStandardEffect(&tree->position);
		}
	}
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
// ツリーを探す
//=============================================================================
Tree* TreeManager::findTree(int _treeID)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		if (treeList[i]->getTreeData()->treeID == _treeID)
		{
			return treeList[i];
		}
	}

	return NULL;
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
		
		ImGui::Text("TrunkA[Num:%d]",	aTrunkRenderer->getObjectNum());
		ImGui::Text("TrunkB[Num:%d]",	bTrunkRenderer->getObjectNum());
		ImGui::Text("TrunkC[Num:%d]",	cTrunkRenderer->getObjectNum());
		ImGui::Text("LeafA[Num:%d]",	aLeafRenderer->getObjectNum());
		ImGui::Text("LeafB[Num:%d]",	bLeafRenderer->getObjectNum());
		ImGui::Text("LeafC[Num:%d]",	cLeafRenderer->getObjectNum());

	}
#endif
}

//=============================================================================
//【Setter】
//=============================================================================
void TreeManager::setGameMaster(GameMaster* gameMaster) { this->gameMaster = gameMaster; }

//=============================================================================
//【Getter】
//=============================================================================
std::vector<Tree*>& TreeManager::getTreeList(){ return treeList; }
float TreeManager::getGreeningRate() { return greeningRate; }
int TreeManager::getTreeNum() { return treeNum; }