//===================================================================================================================================
//【ItemTools.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#include "ItemTools.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
ITEM_TOOLS::ITEM_TOOLS()
{
	//アイテムファイル構造体の初期化
	itemFile = { 0 };
	itemFile.ifmt = nullptr;

	//パス
	setToolsDirectory();

	//レンダラーの初期化
	initRender();

	//ファイル
	FILE	*fp = NULL;
	fp = fopen(ITEM_FILE_PATH, "rb");
	if (fp == NULL)	//ファイルが見つかりません
	{
		//アイテムファイルの新規作成
		CreatNewItemFile();
	}
	else
	{
		//ITEMの読み込み
		fread(&itemFile.item, sizeof(ITEM_ITEM), 1, fp);

		//メモリ確保とIFMTの読み込み
		if (itemFile.item.itemMax != NULL)
		{
			//メモリ確保
			itemFile.ifmt = new ITEM_IFMT[itemFile.item.itemMax];

			for (int i = 0; i < itemFile.item.itemMax; i++)
			{
				//IFMTの読み込み
				fread(&itemFile.ifmt[i], sizeof(ITEM_IFMT), 1, fp);
			}
		}

		//ファイル
		fclose(fp);

		//オブジェクトの初期化
		initObject();
	}

	//アイテム情報
	ItemListboxCurrent = 0;
	ItemListboxType = itemNS::ITEM_TYPE::BATTERY;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
ITEM_TOOLS::~ITEM_TOOLS()
{
	//ファイルの書き出し
	OutputItemFile();

	if (itemFile.item.itemMax != NULL)
	{
		SAFE_DELETE_ARRAY(itemFile.ifmt);
		for (int i = 0; i < itemFile.item.itemMax; i++)
		{
			SAFE_DELETE(object[i]);
		}
		SAFE_DELETE_ARRAY(object);
		SAFE_DELETE_ARRAY(bodyCollide);
	}

	//レンダラー
	for (int i = 0; i < itemNS::ITEM_TYPE::ITEM_TYPE_MAX; i++)
	{
		SAFE_DELETE(renderer[i]);
	}
	SAFE_DELETE_ARRAY(renderer);
}

//===================================================================================================================================
//【アイテムの最大数を取得】
//===================================================================================================================================
int ITEM_TOOLS::GetItemMax(void)
{
	return itemFile.item.itemMax;
}

//===================================================================================================================================
//【アイテム構造体を取得】
//===================================================================================================================================
itemNS::ITEMSET ITEM_TOOLS::GetItemSet(short itemId)
{
	itemNS::ITEMSET tmpItemSet = { 0 };
	tmpItemSet.itemID = itemFile.ifmt[itemId].itemId;
	tmpItemSet.type = itemFile.ifmt[itemId].itemType;
	tmpItemSet.defaultPosition = D3DXVECTOR3(itemFile.ifmt[itemId].posX, itemFile.ifmt[itemId].posY, itemFile.ifmt[itemId].posZ);
	tmpItemSet.defaultDirection = D3DXVECTOR3(itemFile.ifmt[itemId].dirX, itemFile.ifmt[itemId].dirY, itemFile.ifmt[itemId].dirZ);

	return tmpItemSet;
}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void ITEM_TOOLS::outputItemToolsGUI(int GUIid, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
#ifdef _DEBUG
	if (GUIid == ITEM_GUI_ID)
	{
		//フラグ
		bool creatFlag = false;
		bool deleteFlag = false;

		//機能
		ImGui::Checkbox("New Item", &creatFlag);
		ImGui::Checkbox("Delete", &deleteFlag);

		//アイテムの種類
		const char* listboxItemType[] = { "BATTERY" ,"EXAMPLE" };
		ImGui::ListBox("Item Type", &ItemListboxType, listboxItemType, itemNS::ITEM_TYPE::ITEM_TYPE_MAX);

		//アイテムの情報
		if (itemFile.item.itemMax != NULL)
		{
			//メモリ確保
			char **ListboxIndexName = new char*[itemFile.item.itemMax];
			for (int i = 0; i < itemFile.item.itemMax; i++)
			{
				ListboxIndexName[i] = new char[CHAR_MAX];
				sprintf(ListboxIndexName[i], "%d", i);
			}

			ImGui::ListBox("Item ", &ItemListboxCurrent, ListboxIndexName, itemFile.item.itemMax);
			ImGui::Text("ITem ID:%d", itemFile.ifmt[ItemListboxCurrent].itemId);
			ImGui::Text("Item pos:%f %f %f", itemFile.ifmt[ItemListboxCurrent].posX,
				itemFile.ifmt[ItemListboxCurrent].posY,
				itemFile.ifmt[ItemListboxCurrent].posZ);
			ImGui::Text("Item dir:%f %f %f", itemFile.ifmt[ItemListboxCurrent].dirX,
				itemFile.ifmt[ItemListboxCurrent].dirY,
				itemFile.ifmt[ItemListboxCurrent].dirZ);
			ImGui::Text("Item Type:%s",
				listboxItemType[itemFile.ifmt[ItemListboxCurrent].itemType]);

			//メモリ解放
			for (int i = 0; i < itemFile.item.itemMax; i++)
			{
				delete[] ListboxIndexName[i];
			}
			delete[] ListboxIndexName;
		}

		//削除
		if (deleteFlag)
		{
			//削除
			DeleteItemFormat(ItemListboxCurrent);
		}

		//新規作成
		if (creatFlag)
		{
			//作成
			AddItemFormat(ItemListboxType, pos, dir);
		}
	}
#endif
}

//===================================================================================================================================
//【レンダラーの初期化】描画用
//===================================================================================================================================
void ITEM_TOOLS::initRender()
{
	//レンダーの初期化
	renderer = new StaticMeshRenderer*[itemNS::ITEM_TYPE::ITEM_TYPE_MAX];
	for (int i = 0; i < itemNS::ITEM_TYPE::ITEM_TYPE_MAX; i++)
	{
		renderer[i] = new StaticMeshRenderer(staticMeshNS::reference(GetStaticMeshID(i)));
	}

	//更新フラグ
	needUpdate = true;
};

//===================================================================================================================================
//【オブジェクトの初期化】描画用
//===================================================================================================================================
void ITEM_TOOLS::initObject()
{
	//オブジェクト
	object = new Object*[itemFile.item.itemMax];
	bodyCollide = new BoundingSphere[itemFile.item.itemMax];
	for (int i = 0; i < itemFile.item.itemMax; i++)
	{
		//オブジェクトの生成
		object[i] = new Object();
		generate(object[i], itemFile.ifmt[i].itemType, D3DXVECTOR3(itemFile.ifmt[i].posX, itemFile.ifmt[i].posY, itemFile.ifmt[i].posZ),
			D3DXVECTOR3(itemFile.ifmt[i].dirX, itemFile.ifmt[i].dirY, itemFile.ifmt[i].dirZ));
		renderer[itemFile.ifmt[i].itemType]->updateAccessList();

		// コライダの初期化
		bodyCollide->initialize(&D3DXVECTOR3(itemFile.ifmt[i].posX, itemFile.ifmt[i].posY, itemFile.ifmt[i].posZ),
			staticMeshNS::reference(GetStaticMeshID(itemFile.ifmt[i].itemType))->mesh);
	}
}

//===================================================================================================================================
//【更新】描画用
//===================================================================================================================================
void ITEM_TOOLS::update()
{
	if (!needUpdate)return;
	for (int i = 0; i < itemNS::ITEM_TYPE::ITEM_TYPE_MAX; i++)
	{
		renderer[i]->updateBuffer();
		renderer[i]->updateArray();
		renderer[i]->update();
	}
	needUpdate = false;
}

//===================================================================================================================================
//【描画】描画用
//===================================================================================================================================
void ITEM_TOOLS::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	if (itemFile.item.itemMax != NULL)
	{
		for (int i = 0; i < itemNS::ITEM_TYPE::ITEM_TYPE_MAX; i++)
		{
			renderer[i]->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
		}
	}
}

//===================================================================================================================================
//【生成】描画用
//===================================================================================================================================
void ITEM_TOOLS::generate(Object *object, short enemyType, D3DXVECTOR3 position, D3DXVECTOR3 dir)
{
	object->initialize(&position);
	object->postureControl(object->axisZ.direction, dir, 1.0f);
	object->existenceTimer = 1.0f;		// < 0 なら消える
	renderer[enemyType]->registerObject(object);
}

//===================================================================================================================================
//【アイテムファイルの書き出し処理】
//===================================================================================================================================
void ITEM_TOOLS::OutputItemFile(void)
{
	//パス
	setToolsDirectory();

	FILE	*fp = NULL;					//ファイル
	fp = fopen(ITEM_FILE_PATH, "wb");

	//ITEMの書き出し
	fwrite(&itemFile.item, sizeof(ITEM_ITEM), 1, fp);

	//IFMTの書き出し
	for (int i = 0; i < itemFile.item.itemMax; i++)
	{
		//IFMTの書き出し
		fwrite(&itemFile.ifmt[i], sizeof(ITEM_IFMT), 1, fp);
	}

	fclose(fp);
}

//===================================================================================================================================
//【アイテムファイルの新規作成】
//===================================================================================================================================
void ITEM_TOOLS::CreatNewItemFile(void)
{
	//ファイル
	FILE	 *fp = NULL;
	fp = fopen(ITEM_FILE_PATH, "wb");

	//アイテムファイル構造体
	ITEM_FILE tmpEnemyFile;

	//チャンク
	memcpy(tmpEnemyFile.item.chunkId, ITEM_CHUNK, sizeof(tmpEnemyFile.item.chunkId));

	//アイテムの初期数
	tmpEnemyFile.item.itemMax = 0;

	//ENMYのサイズ
	tmpEnemyFile.item.size = sizeof(ITEM_ITEM);

	//書き出し
	fwrite(&tmpEnemyFile.item, sizeof(ITEM_ITEM), 1, fp);

	//ファイル
	fclose(fp);
}

//===================================================================================================================================
//【レンダラーのリセット処理】描画用
//===================================================================================================================================
void ITEM_TOOLS::ResetRenderer(void)
{
	//終了処理
	for (int i = 0; i < itemFile.item.itemMax - 1; i++)
	{
		SAFE_DELETE(object[i]);
	}
	SAFE_DELETE_ARRAY(object);

	if (itemFile.item.itemMax != NULL)
	{
		for (int i = 0; i < itemNS::ITEM_TYPE::ITEM_TYPE_MAX; i++)
		{
			SAFE_DELETE(renderer[i]);
		}
		SAFE_DELETE_ARRAY(renderer);

		//レンダラーの初期化
		initRender();

		//オブジェクトの初期化
		initObject();
	}
}

//===================================================================================================================================
//【スタティックメッシュのIDを取得】描画用
//===================================================================================================================================
int ITEM_TOOLS::GetStaticMeshID(short enemyType)
{
	int staticMeshNo = 0;
	switch (enemyType)
	{
	case itemNS::ITEM_TYPE::BATTERY:
		staticMeshNo = staticMeshNS::SAMPLE_SCISSORS;
		break;
	case itemNS::ITEM_TYPE::EXAMPLE:
		staticMeshNo = staticMeshNS::YAMADA_ROBOT2;
	default:
		break;
	}
	return staticMeshNo;
}

//===================================================================================================================================
//【アイテムの種類を設定】
//===================================================================================================================================
void ITEM_TOOLS::SetItemType(short itemId, short itemType)
{
	itemFile.ifmt[itemId].itemType = itemType;
}

//===================================================================================================================================
//【アイテムの位置を設定】
//===================================================================================================================================
void ITEM_TOOLS::SetItemPos(short itemId, const D3DXVECTOR3 pos)
{
	itemFile.ifmt[itemId].posX = pos.x;
	itemFile.ifmt[itemId].posY = pos.y;
	itemFile.ifmt[itemId].posZ = pos.z;
}

//===================================================================================================================================
//【アイテムの回転軸を設定】
//===================================================================================================================================
void ITEM_TOOLS::SetItemDir(short itemId, const D3DXVECTOR3 dir)
{
	itemFile.ifmt[itemId].dirX = dir.x;
	itemFile.ifmt[itemId].dirY = dir.y;
	itemFile.ifmt[itemId].dirZ = dir.z;
}

//===================================================================================================================================
//【アイテムの設置】
//===================================================================================================================================
void ITEM_TOOLS::SetItem(short itemId, short itemType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//チャンク
	memcpy(itemFile.ifmt[itemId].chunkId, ITEM_CHUNK, sizeof(itemFile.ifmt[itemId].chunkId));

	//アイテム情報
	SetItemType(itemId, itemType);
	SetItemPos(itemId, pos);
	SetItemDir(itemId, dir);

	//サイズ
	itemFile.ifmt[itemId].size = (short)sizeof(ITEM_IFMT);
	//ID
	itemFile.ifmt[itemId].itemId = itemId;
}

//===================================================================================================================================
//【アイテムのフォーマット構造体を整理】
//===================================================================================================================================
void ITEM_TOOLS::UpdateIfmt(int oldItemMax)
{
	//バックアップ用
	ITEM_IFMT *backUpIfmt = new ITEM_IFMT[itemFile.item.itemMax];

	//整理(itemTypeはITEM_TYPE::ITEM_TYPE_MAXなら解放)
	int newItemMax = 0;	//中身があるifmt
	for (int i = 0; i < oldItemMax; i++)
	{
		if (itemFile.ifmt[i].itemType == itemNS::ITEM_TYPE::ITEM_TYPE_MAX)
		{
			//スキップ
			continue;
		}
		else
		{
			//バックアップを取る
			backUpIfmt[newItemMax] = itemFile.ifmt[i];
			newItemMax++;
		}
	}

	//もし作り直す必要があれば
	if (oldItemMax > 0)
	{
		//メモリ解放
		SAFE_DELETE_ARRAY(itemFile.ifmt);
	}

	//メモリ確保し直す
	if (itemFile.item.itemMax > 0)
	{
		itemFile.ifmt = new ITEM_IFMT[itemFile.item.itemMax];

		//ロードバックアップデータ
		for (int i = 0; i < newItemMax; i++)
		{
			itemFile.ifmt[i] = backUpIfmt[i];
			itemFile.ifmt[i].itemId = i;	//Idの更新
		}

		//ITEMの書き換え
		itemFile.item.size = (short)(sizeof(ITEM_ITEM) + (sizeof(ITEM_IFMT) * itemFile.item.itemMax));
	}
	//メモリ解放
	SAFE_DELETE_ARRAY(backUpIfmt);
}

//===================================================================================================================================
//【アイテムのフォーマット構造体を消す】
//===================================================================================================================================
void ITEM_TOOLS::DeleteItemFormat(short itemId)
{
	//消す用(MAXなら消す)
	if (itemFile.item.itemMax != NULL)
	{
		//アイテムの数-1
		itemFile.item.itemMax--;
		if (itemFile.item.itemMax < 0)
		{
			itemFile.item.itemMax = 0;
		}

		//TYPE_MAXなら消す
		itemFile.ifmt[itemId].itemType = itemNS::ITEM_TYPE::ITEM_TYPE_MAX;

		//アイテムのフォーマット構造体を整理
		UpdateIfmt(itemFile.item.itemMax + 1);
	}

	//戻す
	ItemListboxCurrent--;
	if (ItemListboxCurrent < 0)
	{
		ItemListboxCurrent = 0;
	}

	//ファイルのアップデート
	OutputItemFile();

	//レンダラーのリセット処理
	ResetRenderer();
}

//===================================================================================================================================
//【アイテムのフォーマット構造体を追加】
//===================================================================================================================================
void ITEM_TOOLS::AddItemFormat(short itemType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//アイテムの数+1
	itemFile.item.itemMax++;

	//アイテムのフォーマット構造体を整理
	UpdateIfmt(itemFile.item.itemMax - 1);

	//アイテムのフォーマット構造体の最後に追加
	SetItem(itemFile.item.itemMax - 1, itemType, pos, dir);

	//進む
	ItemListboxCurrent = itemFile.item.itemMax - 1;

	//ファイルのアップデート
	OutputItemFile();

	//レンダラーのリセット処理
	ResetRenderer();
}