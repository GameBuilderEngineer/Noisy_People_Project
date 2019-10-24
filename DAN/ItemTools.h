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
#include "StaticMeshRenderer.h"
#include "Player.h"
#include "Item.h"

//===================================================================================================================================
//【マクロ】
//===================================================================================================================================
#define ITEM_CHUNK_ID		(4)
#define ITEM_CHUNK			("ITEM")
#define IFMT_CHUNK			("IFMT")
#define ITEM_FILE_PATH		("item.item")
#define ITEM_GUI_ID			(2)

//===================================================================================================================================
//【構造体】
//===================================================================================================================================
typedef struct	//ITEMチャンク
{
	char chunkId[ITEM_CHUNK_ID];	//チャンクID
	short size;				//サイズ
	short itemMax;			//アイテムの数
}ITEM_ITEM;

typedef struct	//IFMTチャンク
{
	char chunkId[ITEM_CHUNK_ID];	//チャンクID
	short size;				//このチャンクのサイズ
	short itemId;			//アイテムのID
	short itemType;			//アイテムの種類
	float posX;				//アイテムの生成位置(X)
	float posY;				//アイテムの生成位置(Y)
	float posZ;				//アイテムの生成位置(Z)
	float dirX;				//アイテムの向き(X)
	float dirY;				//アイテムの向き(Y)
	float dirZ;				//アイテムの向き(Z)
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

	//ファイル
	int GetItemMax(void);									//アイテムの数を取得
	itemNS::ITEMSET GetItemSet(short itemId);				//アイテムセット構造体を取得

#ifdef _DEBUG
	//変数	
	BoundingSphere *bodyCollide;							//当たり判定
	Object** object;										//オブジェクト

	//GUI
	void outputItemToolsGUI(int GUIid,						//GUI
		const D3DXVECTOR3 pos,const D3DXVECTOR3 dir);

	//レンダラー
	void initRender();										//レンダラーの初期化
	void initObject();										//オブジェクトの初期化
	void update();											//更新
	void render(D3DXMATRIX view,							//描画
		D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generate(Object *object, short enemyType,			//作成
		D3DXVECTOR3 position);
#endif

private:

	//変数
	ITEM_FILE itemFile;										//アイテム構造体

	//アイテムファイル
	void OutputItemFile(void);								//アイテムファイルの書き出し処理
	void CreatNewItemFile(void);							//アイテムファイルの新規作成

#ifdef _DEBUG
	//変数	
	StaticMeshRenderer** renderer;							//レンダラー
	bool needUpdate;										//更新フラグ
	int ItemListboxCurrent;									//アイテム選択用(リストボックス)
	int ItemListboxType;									//アイテムの種類(リストボックス)

	//レンダラー
	void ResetRenderer(void);								//レンダラーをリセット
	int  GetStaticMeshNo(short itemType);					//メッシュIDを取得

	//アイテムフォーマット構造体
	void UpdateIfmt(int oldItemMax);						//アイテムのフォーマット構造体を整理
	void DeleteItemFormat(short itemId);					//アイテムのフォーマット構造体を消す
	void AddItemFormat(short itemType,
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	//アイテムの設置
	void SetItemType(short itemId, short itemType);			//アイテムの種類を設定	
	void SetItemPos(short itemId, const D3DXVECTOR3 pos);	//アイテムの状態を設定
	void SetItemDir(short itemId, const D3DXVECTOR3 dir);	//アイテムの位置を設定
	void SetItem(short itemId, short itemType,				//アイテムの向きを設定
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);		//アイテムの設置

#endif
};