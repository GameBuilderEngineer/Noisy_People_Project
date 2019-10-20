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
#if 0
	// ツリーファイルを読み込む
	FILE	*fp = NULL;
	fopen("enemy.enemy", "rb");
	ENEMY_FILE enemyFile;
	memset(&enemyFile, 0, sizeof(ENEMY_FILE));
	fread(&enemyFile.enmy, sizeof(ENEMY_ENMY), 1, fp);
	enemyFile.efmt = new ENEMY_EFMT[enemyFile.enmy.enemyMax];
	fread(&enemyFile.efmt, sizeof(ENEMY_EFMT), enemyFile.enmy.enemyMax, fp);
	fclose(fp);

	for (size_t i = 0; i < enemyFile.enmy.enemyMax; i++)
	{
		tree.XX = //enemyFile.efmt[i].chunkId;
	}
	SAFE_DELETE_ARRAY(enemyFile.efmt);
#endif

	// 描画オブジェクトの作成
	greenA = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_001));
	deadA = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
	greenB = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	deadB = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
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
	SAFE_DELETE(greenA);
	SAFE_DELETE(deadA);
	SAFE_DELETE(greenB);
	SAFE_DELETE(deadB);
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
}


//=============================================================================
// 描画処理
//=============================================================================
void TreeManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		treeList[i]->render(view, projection, cameraPosition);
	}
}


//=============================================================================
// ツリーオブジェクトの作成
//=============================================================================
void TreeManager::createTree(TreeData* treeData)
{
	switch (treeData->treeType)
	{
	case ANALOG_TREE:
		break;

	case DIGITAL_TREE:
		break;

	default:
		treeList.emplace_back(new Tree);
		break;
	}

	treeList.back()->setDataToTree(treeData);
}


//=============================================================================
// ツリーオブジェクトの破棄
// ※使用不可
//=============================================================================
void TreeManager::destroyTree(int _id)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		if (treeList[i]->getTreeData()->id == _id)
		{
			SAFE_DELETE(treeList[i]);
			treeList.erase(treeList.begin() + i);
			break;
		}
	}
	// 今はStaticMeshObjectに登録されたオブジェクトを個別に破棄できない
}


//=============================================================================
// 全ツリーオブジェクトの破棄
// ※仮実装
//=============================================================================
void TreeManager::destroyAllTree()
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		SAFE_DELETE(treeList[i]);
	}

	treeList.clear();
	SAFE_DELETE(greenA);
	SAFE_DELETE(deadA);
	SAFE_DELETE(greenB);
	SAFE_DELETE(deadB);
	greenA = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_001));
	deadA = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
	greenB = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	deadB = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
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

		//ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//位置
		//ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//回転
		//ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);					//スケール
		//ImGui::SliderFloat("radius", &radius, 0, limitTop);							//半径
		//ImGui::SliderFloat("alpha", &alpha, 0, 255);									//透過値
		//ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);					//速度
		//ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//加速度
		//ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);				//重力

		//ImGui::Checkbox("onGravity", &onGravity);										//重力有効化フラグ
		//ImGui::Checkbox("onActive", &onActive);										//アクティブ化フラグ
		//ImGui::Checkbox("onRender", &onRender);										//描画有効化フラグ
		//ImGui::Checkbox("onLighting", &onLighting);									//光源処理フラグ
		//ImGui::Checkbox("onTransparent", &onTransparent);								//透過フラグ
		//ImGui::Checkbox("operationAlpha", &operationAlpha);							//透過値の操作有効フラグ

		//ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);					//透過値の操作有効フラグ
	}

#endif
}