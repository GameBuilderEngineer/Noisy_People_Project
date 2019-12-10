//===================================================================================================================================
//【TreeTools.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#include "TreeTools.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
TREE_TOOLS::TREE_TOOLS()
{
	//ツリーファイル構造体の初期化
	treeFile = { 0 };
	treeFile.tfmt = nullptr;

	//パス
	setToolsDirectory();

#ifdef _DEBUG
	//レンダラーの初期化
	initRender();
#endif

	//ファイル
	FILE	*fp = NULL;
	fp = fopen(TREE_FILE_PATH, "rb");
	if (fp == NULL)	//ファイルが見つかりません
	{
		//ツリーファイルの新規作成
		CreatNewTreeFile();
	}
	else
	{
		//TREEの読み込み
		fread(&treeFile.tree, sizeof(TREE_TREE), 1, fp);

		//メモリ確保とTFMTの読み込み
		if (treeFile.tree.treeMax != NULL)
		{
			//メモリ確保
			treeFile.tfmt = new TREE_TFMT[treeFile.tree.treeMax];

			for (int i = 0; i < treeFile.tree.treeMax; i++)
			{
				//TFMTの読み込み
				fread(&treeFile.tfmt[i], sizeof(TREE_TFMT), 1, fp);
			}
		}

		//ファイル
		fclose(fp);

#ifdef _DEBUG
		//オブジェクトの初期化
		initObject();
#endif
	}

#ifdef _DEBUG
	//ツリー情報
	TreeListboxCurrent = 0;
	TreeListboxType = treeNS::TREE_TYPE::ANALOG_TREE;
	TreeListboxState = treeNS::GREEN_STATE::GREEN;
	TreeListboxSize = treeNS::TREE_SIZE::STANDARD;
	TreeListboxModel = treeNS::TREE_MODEL::A_MODEL;

	Model[treeNS::TREE_TYPE::ANALOG_TREE] = staticMeshNS::A_TRUNK;
	Model[treeNS::TREE_TYPE::DIGITAL_TREE] = staticMeshNS::B_TRUNK;

	//レンダラーのリセット処理
	ResetRenderer();

	input = getInput();
#endif
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
TREE_TOOLS::~TREE_TOOLS()
{
	//ファイルの書き出し
	OutputTreeFile();

	if (treeFile.tree.treeMax != NULL)
	{
		SAFE_DELETE_ARRAY(treeFile.tfmt);
#ifdef _DEBUG
		for (int i = 0; i < treeFile.tree.treeMax; i++)
		{
			SAFE_DELETE(object[i]);
		}
		SAFE_DELETE_ARRAY(object);
		SAFE_DELETE_ARRAY(bodyCollide);
#endif
	}

#ifdef _DEBUG
	//レンダラー
	for (int i = 0; i < treeNS::TREE_MODEL::TREE_MAX; i++)
	{
		SAFE_DELETE(trunkRenderer[i]);
	}
	SAFE_DELETE_ARRAY(trunkRenderer);
	for (int i = 0; i < treeNS::GREEN_STATE::GREEN_STATE_MAX; i++)
	{
		SAFE_DELETE(leafRenderer[i]);
	}
	SAFE_DELETE_ARRAY(leafRenderer);
#endif
}

//===================================================================================================================================
//【ツリーの最大数を取得】
//===================================================================================================================================
int TREE_TOOLS::GetTreeMax(void)
{
	return treeFile.tree.treeMax;
}

//===================================================================================================================================
//【ツリー構造体を取得】
//===================================================================================================================================
treeNS::TREESET TREE_TOOLS::GetTreeSet(short treeId)
{
	treeNS::TREESET tmpTreeSet = { 0 };
	tmpTreeSet.treeID = treeFile.tfmt[treeId].treeId;
	tmpTreeSet.type = (treeNS::TREE_TYPE)treeFile.tfmt[treeId].treeType;
	tmpTreeSet.greenState = (treeNS::GREEN_STATE)treeFile.tfmt[treeId].treeState;
	tmpTreeSet.size = (treeNS::TREE_SIZE)treeFile.tfmt[treeId].treeSize;
	tmpTreeSet.model = (treeNS::TREE_MODEL)treeFile.tfmt[treeId].treeModel;
	tmpTreeSet.initialPosition = D3DXVECTOR3(treeFile.tfmt[treeId].posX, treeFile.tfmt[treeId].posY, treeFile.tfmt[treeId].posZ);
	tmpTreeSet.initialDirection = D3DXVECTOR3(treeFile.tfmt[treeId].dirX, treeFile.tfmt[treeId].dirY, treeFile.tfmt[treeId].dirZ);

	return tmpTreeSet;
}

#ifdef _DEBUG
//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void TREE_TOOLS::outputTreeToolsGUI(int GUIid, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
#ifdef _DEBUG
	if (GUIid == TREE_GUI_ID)
	{
		//フラグ
		bool creatFlag = false;
		bool deleteFlag = false;
		if (input->wasKeyPressed('J'))
		{
			creatFlag = true;
		}
		else if (input->wasKeyPressed('K'))
		{
			deleteFlag = true;
		}

		//機能
		ImGui::Checkbox("New Tree", &creatFlag);
		ImGui::Checkbox("Delete", &deleteFlag);

		//ツリーの種類
		const char* listboxTreeType[] = { "ANALOG_TREE" ,"DIGITAL_TREE" };
		ImGui::ListBox("Tree Type", &TreeListboxType, listboxTreeType, treeNS::TREE_TYPE::TREE_TYPE_MAX);

		//ツリーの状態
		const char* listboxTreeState[] = { "GREEN" ,"DEAD" };
		ImGui::ListBox("Tree State", &TreeListboxState, listboxTreeState, treeNS::GREEN_STATE::GREEN_STATE_MAX);

		//ツリーのサイズ
		const char* listboxTreeSize[] = { "STANDARD" ,"LARGE","VERY_LARGE" };
		ImGui::ListBox("Tree Size", &TreeListboxSize, listboxTreeSize, treeNS::TREE_SIZE::TREE_SIZE_MAX);

		//ツリーのモデル
		const char* listboxTreeModel[] = { "A_MODEL" ,"B_MODEL" ,"C_MODEL" };
		ImGui::ListBox("Tree Model", &TreeListboxModel, listboxTreeModel, treeNS::TREE_MODEL::TREE_MAX);

		//ツリーの情報
		if (treeFile.tree.treeMax != NULL)
		{
			//メモリ確保
			char **ListboxIndexName = new char*[treeFile.tree.treeMax];
			for (int i = 0; i < treeFile.tree.treeMax; i++)
			{
				ListboxIndexName[i] = new char[CHAR_MAX];
				sprintf(ListboxIndexName[i], "%d", i);
			}

			ImGui::ListBox("Tree ", &TreeListboxCurrent, ListboxIndexName, treeFile.tree.treeMax);
			ImGui::Text("ITem ID:%d", treeFile.tfmt[TreeListboxCurrent].treeId);
			ImGui::Text("Tree pos:%f %f %f", treeFile.tfmt[TreeListboxCurrent].posX,
				treeFile.tfmt[TreeListboxCurrent].posY,
				treeFile.tfmt[TreeListboxCurrent].posZ);
			ImGui::Text("Tree dir:%f %f %f", treeFile.tfmt[TreeListboxCurrent].dirX,
				treeFile.tfmt[TreeListboxCurrent].dirY,
				treeFile.tfmt[TreeListboxCurrent].dirZ);
			ImGui::Text("Tree Type:%s",
				listboxTreeType[treeFile.tfmt[TreeListboxCurrent].treeType]);
			ImGui::Text("Tree State:%s",
				listboxTreeState[treeFile.tfmt[TreeListboxCurrent].treeState]);
			ImGui::Text("Tree Size:%s",
				listboxTreeSize[treeFile.tfmt[TreeListboxCurrent].treeSize]);
			ImGui::Text("Tree Model:%s",
				listboxTreeModel[treeFile.tfmt[TreeListboxCurrent].treeModel]);

			//メモリ解放
			for (int i = 0; i < treeFile.tree.treeMax; i++)
			{
				delete[] ListboxIndexName[i];
			}
			delete[] ListboxIndexName;
		}

		//削除
		if (deleteFlag)
		{
			//削除
			DeleteTreeFormat(TreeListboxCurrent);
		}

		//新規作成
		if (creatFlag)
		{
			//作成
			AddTreeFormat(TreeListboxType, TreeListboxState, TreeListboxSize, TreeListboxModel, pos, dir);
		}
	}
#endif
}

//===================================================================================================================================
//【レンダラーの初期化】描画用
//===================================================================================================================================
void TREE_TOOLS::initRender()
{
	//レンダーの初期化
	trunkRenderer = new StaticMeshRenderer*[treeNS::TREE_MODEL::TREE_MAX];
	for (int i = 0; i < treeNS::TREE_MODEL::TREE_MAX; i++)
	{
		trunkRenderer[i] = new StaticMeshRenderer(staticMeshNS::reference(GetStaticTrunkMeshID(i)));
	}
	leafRenderer = new StaticMeshRenderer*[treeNS::GREEN_STATE::GREEN_STATE_MAX];
	for (int i = 0; i < treeNS::GREEN_STATE::GREEN_STATE_MAX; i++)
	{
		leafRenderer[i] = new StaticMeshRenderer(staticMeshNS::reference(GetStaticLeafMeshID(i)));
	}

	//更新フラグ
	needUpdate = true;
};

//===================================================================================================================================
//【オブジェクトの初期化】描画用
//===================================================================================================================================
void TREE_TOOLS::initObject()
{
	//オブジェクト
	object = new Object*[treeFile.tree.treeMax];
	bodyCollide = new BoundingSphere[treeFile.tree.treeMax];
	for (int i = 0; i < treeFile.tree.treeMax; i++)
	{
		//オブジェクトの生成
		object[i] = new Object();
		generateTrunk(object[i], treeFile.tfmt[i].treeType,
			D3DXVECTOR3(treeFile.tfmt[i].posX, treeFile.tfmt[i].posY, treeFile.tfmt[i].posZ), 
			D3DXVECTOR3(treeFile.tfmt[i].dirX, treeFile.tfmt[i].dirY, treeFile.tfmt[i].dirZ));
		generateLeaf(object[i], treeFile.tfmt[i].treeState, 
			D3DXVECTOR3(treeFile.tfmt[i].posX, treeFile.tfmt[i].posY, treeFile.tfmt[i].posZ),
			D3DXVECTOR3(treeFile.tfmt[i].dirX, treeFile.tfmt[i].dirY, treeFile.tfmt[i].dirZ));
		trunkRenderer[treeFile.tfmt[i].treeType]->updateAccessList();
		leafRenderer[treeFile.tfmt[i].treeState]->updateAccessList();

		// コライダの初期化
		bodyCollide->initialize(&D3DXVECTOR3(treeFile.tfmt[i].posX, treeFile.tfmt[i].posY, treeFile.tfmt[i].posZ),
			staticMeshNS::reference(GetStaticLeafMeshID(treeFile.tfmt[i].treeState))->mesh);
	}
}

//===================================================================================================================================
//【更新】描画用
//===================================================================================================================================
void TREE_TOOLS::update()
{
	if (!needUpdate)return;
	for (int i = 0; i < treeNS::TREE_MODEL::TREE_MAX; i++)
	{
		trunkRenderer[i]->updateBuffer();
		trunkRenderer[i]->updateArray();
		trunkRenderer[i]->update();
	}
	for (int i = 0; i < treeNS::GREEN_STATE::GREEN_STATE_MAX; i++)
	{
		leafRenderer[i]->updateBuffer();
		leafRenderer[i]->updateArray();
		leafRenderer[i]->update();
	}

	needUpdate = false;
}

//===================================================================================================================================
//【描画】描画用
//===================================================================================================================================
void TREE_TOOLS::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	if (treeFile.tree.treeMax != NULL)
	{
		for (int i = 0; i < treeNS::TREE_MODEL::TREE_MAX; i++)
		{
			trunkRenderer[i]->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
		}
		for (int i = 0; i < treeNS::GREEN_STATE::GREEN_STATE_MAX; i++)
		{
			leafRenderer[i]->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
		}
	}
}

//===================================================================================================================================
//【生成】描画用
//===================================================================================================================================
void TREE_TOOLS::generateTrunk(Object *object, short treeType, D3DXVECTOR3 position, D3DXVECTOR3 dir)
{
	object->initialize(&position);
	object->postureControl(object->axisZ.direction, dir, 1.0f);
	object->existenceTimer = 1.0f;		// < 0 なら消える
	trunkRenderer[treeType]->registerObject(object);
}

//===================================================================================================================================
//【生成】描画用
//===================================================================================================================================
void TREE_TOOLS::generateLeaf(Object *object, short treeState, D3DXVECTOR3 position, D3DXVECTOR3 dir)
{
	object->initialize(&position);
	object->postureControl(object->axisZ.direction, dir, 1.0f);
	object->existenceTimer = 1.0f;		// < 0 なら消える
	leafRenderer[treeState]->registerObject(object);
}
#endif

//===================================================================================================================================
//【ツリーファイルの書き出し処理】
//===================================================================================================================================
void TREE_TOOLS::OutputTreeFile(void)
{
	//パス
	setToolsDirectory();

	FILE	*fp = NULL;					//ファイル
	fp = fopen(TREE_FILE_PATH, "wb");

	//TREEの書き出し
	fwrite(&treeFile.tree, sizeof(TREE_TREE), 1, fp);

	//TFMTの書き出し
	for (int i = 0; i < treeFile.tree.treeMax; i++)
	{
		//TFMTの書き出し
		fwrite(&treeFile.tfmt[i], sizeof(TREE_TFMT), 1, fp);
	}

	fclose(fp);
}

//===================================================================================================================================
//【ツリーファイルの新規作成】
//===================================================================================================================================
void TREE_TOOLS::CreatNewTreeFile(void)
{
	//ファイル
	FILE	 *fp = NULL;
	fp = fopen(TREE_FILE_PATH, "wb");

	//ツリーファイル構造体
	TREE_FILE tmpEnemyFile;

	//チャンク
	memcpy(tmpEnemyFile.tree.chunkId, TREE_CHUNK, sizeof(tmpEnemyFile.tree.chunkId));

	//ツリーの初期数
	tmpEnemyFile.tree.treeMax = 0;

	//ENMYのサイズ
	tmpEnemyFile.tree.size = sizeof(TREE_TREE);

	//書き出し
	fwrite(&tmpEnemyFile.tree, sizeof(TREE_TREE), 1, fp);

	//ファイル
	fclose(fp);
}

#ifdef _DEBUG
//===================================================================================================================================
//【レンダラーのリセット処理】描画用
//===================================================================================================================================
void TREE_TOOLS::ResetRenderer(void)
{
	//終了処理
	for (int i = 0; i < treeFile.tree.treeMax - 1; i++)
	{
		SAFE_DELETE(object[i]);
	}
	SAFE_DELETE_ARRAY(object);

	if (treeFile.tree.treeMax != NULL)
	{
		for (int i = 0; i < treeNS::TREE_MODEL::TREE_MAX; i++)
		{
			SAFE_DELETE(trunkRenderer[i]);
		}
		SAFE_DELETE_ARRAY(trunkRenderer);

		for (int i = 0; i < treeNS::GREEN_STATE::GREEN_STATE_MAX; i++)
		{
			SAFE_DELETE(leafRenderer[i]);
		}
		SAFE_DELETE_ARRAY(leafRenderer);

		//レンダラーの初期化
		initRender();

		//オブジェクトの初期化
		initObject();
	}
}

//===================================================================================================================================
//【スタティックメッシュのIDを取得】描画用
//===================================================================================================================================
int TREE_TOOLS::GetStaticTrunkMeshID(short treeType)
{
	int staticMeshNo = 0;
	switch (treeType)
	{
	case treeNS::TREE_MODEL::A_MODEL:
		staticMeshNo = staticMeshNS::A_TRUNK;
		break;
	case treeNS::TREE_MODEL::B_MODEL:
		staticMeshNo = staticMeshNS::B_TRUNK;
		break;
	case treeNS::TREE_MODEL::C_MODEL:
		staticMeshNo = staticMeshNS::GREEN_TREE_002;
		break;
	default:
		break;
	}
	return staticMeshNo;
}

//===================================================================================================================================
//【スタティックメッシュのIDを取得】描画用
//===================================================================================================================================
int TREE_TOOLS::GetStaticLeafMeshID(short treeState)
{
	int staticMeshNo = 0;
	switch (treeState)
	{
	case treeNS::GREEN_STATE::GREEN:
		staticMeshNo = staticMeshNS::A_LEAF;
		break;
	case treeNS::GREEN_STATE::DEAD:
		staticMeshNo = staticMeshNS::GRASS;
		break;
	default:
		break;
	}
	return staticMeshNo;
}

//===================================================================================================================================
//【ツリーの種類を設定】
//===================================================================================================================================
void TREE_TOOLS::SetTreeType(short treeId, short treeType)
{
	treeFile.tfmt[treeId].treeType = treeType;
}

//===================================================================================================================================
//【ツリーの状態を設定】
//===================================================================================================================================
void TREE_TOOLS::SetTreeState(short treeId, short treeState)
{
	treeFile.tfmt[treeId].treeState = treeState;
}

//===================================================================================================================================
//【ツリーのサイズを設定】
//===================================================================================================================================
void TREE_TOOLS::SetTreeSize(short treeId, short treeSize)
{
	treeFile.tfmt[treeId].treeSize = treeSize;
}

//===================================================================================================================================
//【ツリーのモデルを設定】
//===================================================================================================================================
void TREE_TOOLS::SetTreeModel(short treeId, short treeModel)
{
	treeFile.tfmt[treeId].treeModel = treeModel;
}

//===================================================================================================================================
//【ツリーの位置を設定】
//===================================================================================================================================
void TREE_TOOLS::SetTreePos(short treeId, const D3DXVECTOR3 pos)
{
	treeFile.tfmt[treeId].posX = pos.x;
	treeFile.tfmt[treeId].posY = pos.y;
	treeFile.tfmt[treeId].posZ = pos.z;
}

//===================================================================================================================================
//【ツリーの回転軸を設定】
//===================================================================================================================================
void TREE_TOOLS::SetTreeDir(short treeId, const D3DXVECTOR3 dir)
{
	treeFile.tfmt[treeId].dirX = dir.x;
	treeFile.tfmt[treeId].dirY = dir.y;
	treeFile.tfmt[treeId].dirZ = dir.z;
}

//===================================================================================================================================
//【ツリーの設置】
//===================================================================================================================================
void TREE_TOOLS::SetTree(short treeId, short treeType, short treeState,
	short treeSize, short treeModel, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//チャンク
	memcpy(treeFile.tfmt[treeId].chunkId, TREE_CHUNK, sizeof(treeFile.tfmt[treeId].chunkId));

	//ツリー情報
	SetTreeType(treeId, treeType);
	SetTreeState(treeId, treeState);
	SetTreeSize(treeId, treeSize);
	SetTreeModel(treeId, treeModel);
	SetTreePos(treeId, pos);
	SetTreeDir(treeId, dir);

	//サイズ
	treeFile.tfmt[treeId].size = (short)sizeof(TREE_TFMT);
	//ID
	treeFile.tfmt[treeId].treeId = treeId;
}

//===================================================================================================================================
//【ツリーのフォーマット構造体を整理】
//===================================================================================================================================
void TREE_TOOLS::UpdateTfmt(int oldTreeMax)
{
	//バックアップ用
	TREE_TFMT *backUpTfmt = new TREE_TFMT[treeFile.tree.treeMax];

	//整理(treeTypeはTREE_TYPE::TREE_TYPE_MAXなら解放)
	int newTreeMax = 0;	//中身があるtfmt
	for (int i = 0; i < oldTreeMax; i++)
	{
		if (treeFile.tfmt[i].treeType == treeNS::TREE_TYPE::TREE_TYPE_MAX)
		{
			//スキップ
			continue;
		}
		else
		{
			//バックアップを取る
			backUpTfmt[newTreeMax] = treeFile.tfmt[i];
			newTreeMax++;
		}
	}

	//もし作り直す必要があれば
	if (oldTreeMax > 0)
	{
		//メモリ解放
		SAFE_DELETE_ARRAY(treeFile.tfmt);
	}

	//メモリ確保し直す
	if (treeFile.tree.treeMax > 0)
	{
		treeFile.tfmt = new TREE_TFMT[treeFile.tree.treeMax];

		//ロードバックアップデータ
		for (int i = 0; i < newTreeMax; i++)
		{
			treeFile.tfmt[i] = backUpTfmt[i];
			treeFile.tfmt[i].treeId = i;	//Idの更新
		}

		//TREEの書き換え
		treeFile.tree.size = (short)(sizeof(TREE_TREE) + (sizeof(TREE_TFMT) * treeFile.tree.treeMax));
	}
	//メモリ解放
	SAFE_DELETE_ARRAY(backUpTfmt);
}

//===================================================================================================================================
//【ツリーのフォーマット構造体を消す】
//===================================================================================================================================
void TREE_TOOLS::DeleteTreeFormat(short treeId)
{
	//消す用(MAXなら消す)
	if (treeFile.tree.treeMax != NULL)
	{
		//ツリーの数-1
		treeFile.tree.treeMax--;
		if (treeFile.tree.treeMax < 0)
		{
			treeFile.tree.treeMax = 0;
		}

		//TYPE_MAXなら消す
		treeFile.tfmt[treeId].treeType = treeNS::TREE_TYPE::TREE_TYPE_MAX;

		//ツリーのフォーマット構造体を整理
		UpdateTfmt(treeFile.tree.treeMax + 1);
	}

	//戻す
	TreeListboxCurrent--;
	if (TreeListboxCurrent < 0)
	{
		TreeListboxCurrent = 0;
	}

	//ファイルのアップデート
	OutputTreeFile();

	//レンダラーのリセット処理
	ResetRenderer();
}

//===================================================================================================================================
//【ツリーのフォーマット構造体を追加】
//===================================================================================================================================
void TREE_TOOLS::AddTreeFormat(short treeType, short treeState,
	short treeSize, short treeModel, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//ツリーの数+1
	treeFile.tree.treeMax++;

	//ツリーのフォーマット構造体を整理
	UpdateTfmt(treeFile.tree.treeMax - 1);

	//ツリーのフォーマット構造体の最後に追加
	SetTree(treeFile.tree.treeMax - 1, treeType, treeState,
		treeSize, treeModel, pos, dir);

	//進む
	TreeListboxCurrent = treeFile.tree.treeMax - 1;

	//ファイルのアップデート
	OutputTreeFile();

	//レンダラーのリセット処理
	ResetRenderer();
}
#endif