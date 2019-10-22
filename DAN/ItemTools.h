//===================================================================================================================================
//【ItemTools.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/26
// [更新日]2019/09/26
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【ライブラリのロード】
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshObject.h"
#include "Player.h"
#include "Item.h"

//===================================================================================================================================
//【マクロ】
//===================================================================================================================================
#define ITEM_CHUNK_ID		(4)
#define ITEM_CHUNK			("ITEM")
#define IFMT_CHUNK			("IFMT")
#define ITEM_FILE_PATH		("item.item")

//===================================================================================================================================
//【構造体】
//===================================================================================================================================
typedef struct	//ITEMチャンク
{
	char chunkId[ITEM_CHUNK_ID];	//チャンクID
	short size;						//以後のサイズ(ファイルのサイズ - 8byte)
	short itemMax;					//敵の数
}ITEM_ITEM;

typedef struct	//IFMTチャンク
{
	char chunkId[ITEM_CHUNK_ID];	//チャンクID
	short size;						//このチャンクのサイズ
	short itemId;					//アイテムのID
	short itemType;					//アイテムの種類
	float posX;						//アイテムの生成位置(X)
	float posY;						//アイテムの生成位置(Y)
	float posZ;						//アイテムの生成位置(Z)
	float dirX;						//アイテムの向き(X)
	float dirY;						//アイテムの向き(Y)
	float dirZ;						//アイテムの向き(Z)
}ITEM_IFMT;

typedef struct	//ITEMファイル構造体
{
	ITEM_ITEM item;			//ITEMチャンク	
	ITEM_IFMT *ifmt;		//IFMTチャンク
}ITEM_FILE;

//===================================================================================================================================
//【アイテムツール】
//アイテムツール用クラス
//===================================================================================================================================
class ITEM_TOOLS : public Base
{
public:
	ITEM_TOOLS();
	~ITEM_TOOLS();

	//変数
	bool resetItem;

	//関数
	itemNS::ITEMSET GetItemSet(short itemId);
	int GetItemMax(void);
	void SetItem(short itemId, short itemType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

#ifdef _DEBUG
	//関数
	void outputItemToolsGUI(const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	////描画用
	//void initialize();
	//void update();
	//void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	//void generate(D3DXVECTOR3 position);
#endif

private:

	//変数
	ITEM_FILE itemFile;					//アイテム構造体
	int ItemListboxCurrent;				//アイテム選択用(リストボックス)
	int ItemListboxType;				//アイテムの種類(リストボックス)

#ifdef _DEBUG
	////描画用
	//StaticMeshObject* renderer;
	//bool needUpdate;
#endif

	//関数
	void SetItemType(short itemId, short itemType);
	void SetItemPos(short itemId, const D3DXVECTOR3 pos);
	void SetItemDir(short itemId, const D3DXVECTOR3 dir);
	void OutputItemFile(void);				//アイテムファイルの書き出し処理
	void CreatNewItemFile(void);			//アイテムファイルの新規作成
	void AddItemFormat(short itemType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);
	void DeleteItemFormat(short itemId);	//アイテムのフォーマット構造体を消す
	void UpdateIfmt(int oldItemMax);		//アイテムのフォーマット構造体を整理

};