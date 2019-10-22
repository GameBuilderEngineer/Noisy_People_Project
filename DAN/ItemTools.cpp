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


		fclose(fp);

		//アイテム情報
		ItemListboxCurrent = 0;
		ItemListboxType = itemNS::ITEM_TYPE::BATTERY;

		//描画用
		//renderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
		//initialize();
		//for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
		//{
		//	generate(D3DXVECTOR3(enemyFile.efmt[i].posX, enemyFile.efmt[i].posY, enemyFile.efmt[i].posZ));
		//	renderer->updateAccessList();
		//}
	}

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
	}

	//描画用
	//SAFE_DELETE(renderer);
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

	fclose(fp);
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

	//描画用
	//generate(pos);
	//renderer->updateAccessList();
	//needUpdate = true;
}

//===================================================================================================================================
//【アイテムの最大数を取得】
//===================================================================================================================================
int ITEM_TOOLS::GetItemMax(void)
{
	return itemFile.item.itemMax;
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
//【アイテム構造体を取得】
//===================================================================================================================================
itemNS::ITEMSET ITEM_TOOLS::GetItemSet(short itemId)
{
	itemNS::ITEMSET tmpItemSet = { 0 };
	tmpItemSet.id = itemFile.ifmt[itemId].itemId;
	tmpItemSet.type = itemFile.ifmt[itemId].itemType;
	tmpItemSet.defaultPosition = D3DXVECTOR3(itemFile.ifmt[itemId].posX, itemFile.ifmt[itemId].posY, itemFile.ifmt[itemId].posZ);
	tmpItemSet.defaultDirection = D3DXVECTOR3(itemFile.ifmt[itemId].dirX, itemFile.ifmt[itemId].dirY, itemFile.ifmt[itemId].dirZ);

	return tmpItemSet;
}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void ITEM_TOOLS::outputItemToolsGUI(const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
#ifdef _DEBUG
	//フラグ
	bool creatFlag = false;
	bool deleteFlag = false;

	ImGui::Begin("Item Tools");

	//機能
	ImGui::Checkbox("New Item", &creatFlag);
	ImGui::Checkbox("Delete", &deleteFlag);

	//アイテムの種類
	const char* listboxItemType[] = { "BATTERY" };
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

	ImGui::End();

	//選択中


	//削除
	if (deleteFlag)
	{
		//削除
		DeleteItemFormat(ItemListboxCurrent);

		//戻す
		ItemListboxCurrent--;
		if (ItemListboxCurrent < 0)
		{
			ItemListboxCurrent = 0;
		}
	}

	//新規作成
	if (creatFlag)
	{
		//作成
		AddItemFormat(ItemListboxType, pos, dir);

		//進む
		ItemListboxCurrent = itemFile.item.itemMax - 1;
	}
#endif
}


//===================================================================================================================================
//【アイテムのフォーマット構造体を消す】
//===================================================================================================================================
void ITEM_TOOLS::DeleteItemFormat(short itemId)
{
	//消す用(MAXなら消す)
	if (itemFile.item.itemMax != NULL)
	{
		//一つを消す
		itemFile.item.itemMax--;
		if (itemFile.item.itemMax < 0)
		{
			itemFile.item.itemMax = 0;
		}

		//TYPE_MAXなら消す
		itemFile.ifmt[itemId].itemType = itemNS::ITEM_TYPE::ITEM_TYPE_MAX;

		//アイテムのフォーマット構造体を整理
		UpdateIfmt(itemFile.item.itemMax + 1);

		//描画用
		//renderer->deleteObject(enemyFile.enmy.enemyMax - enemyId);
		//renderer->updateAccessList();
		//needUpdate = true;
	}
}
