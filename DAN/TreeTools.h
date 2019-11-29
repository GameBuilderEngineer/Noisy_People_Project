//===================================================================================================================================
//【TreeTools.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/28
// [更新日]2019/10/28
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【ライブラリのロード】
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshRenderer.h"
#include "Player.h"
#include "Tree.h"

//===================================================================================================================================
//【マクロ】
//===================================================================================================================================
#define TREE_CHUNK_ID		(4)
#define TREE_CHUNK			("TREE")
#define TFMT_CHUNK			("TFMT")
#define TREE_FILE_PATH		("tree.tree")
#define TREE_GUI_ID			(3)

//===================================================================================================================================
//【構造体】
//===================================================================================================================================
typedef struct	//TREEチャンク
{
	char chunkId[TREE_CHUNK_ID];	//チャンクID
	short size;				//サイズ
	short treeMax;			//ツリーの数
}TREE_TREE;

typedef struct	//TFMTチャンク
{
	char chunkId[TREE_CHUNK_ID];	//チャンクID
	short size;				//このチャンクのサイズ
	short treeId;			//ツリーのID
	short treeType;			//ツリーの種類
	short treeState;		//ツリーの状態
	short treeSize;			//ツリーのサイズ
	short treeModel;		//ツリーのモデル
	float posX;				//ツリーの生成位置(X)
	float posY;				//ツリーの生成位置(Y)
	float posZ;				//ツリーの生成位置(Z)
	float dirX;				//ツリーの向き(X)
	float dirY;				//ツリーの向き(Y)
	float dirZ;				//ツリーの向き(Z)
}TREE_TFMT;

typedef struct	//TREEファイル構造体
{
	TREE_TREE tree;			//TREEチャンク	
	TREE_TFMT *tfmt;		//TFMTチャンク
}TREE_FILE;

//===================================================================================================================================
//【ツリーツール】
//ツリーツール用クラス
//===================================================================================================================================
class TREE_TOOLS : public Base
{
public:
	TREE_TOOLS();
	~TREE_TOOLS();

	//ファイル
	int GetTreeMax(void);									//ツリーの数を取得
	treeNS::TREESET GetTreeSet(short treeId);				//ツリーセット構造体を取得

#ifdef _DEBUG
	//変数
	int TreeListboxType;									//ツリーの種類(リストボックス)
	int TreeListboxState;									//ツリーの状態(リストボックス)
	int TreeListboxSize;									//ツリーのサイズ(リストボックス)
	int TreeListboxModel;									//ツリーのモデル(リストボックス)
	BoundingSphere *bodyCollide;							//当たり判定
	Object** object;										//オブジェクト

	//GUI
	void outputTreeToolsGUI(int GUIid,						//GUI
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	//レンダラー
	int  GetStaticTrunkMeshID(short treeType);				//メッシュIDを取得(木)
	int  GetStaticLeafMeshID(short treeState);				//メッシュIDを取得(葉)
	void initRender();										//レンダラーの初期化
	void initObject();										//オブジェクトの初期化
	void update();											//更新
	void render(D3DXMATRIX view,							//描画
		D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generateTrunk(Object *object, short treeType,		//作成
		 D3DXVECTOR3 position, D3DXVECTOR3 dir);
	void generateLeaf(Object *object, short treeState,		//作成
		D3DXVECTOR3 position, D3DXVECTOR3 dir);
#endif

private:

	//変数
	TREE_FILE treeFile;										//ツリー構造体

	//ツリーファイル
	void OutputTreeFile(void);								//ツリーファイルの書き出し処理
	void CreatNewTreeFile(void);							//ツリーファイルの新規作成

#ifdef _DEBUG
	//変数	
	StaticMeshRenderer** trunkRenderer;						//レンダラー
	StaticMeshRenderer** leafRenderer;						//レンダラー
	bool needUpdate;										//更新フラグ
	int TreeListboxCurrent;									//ツリー選択用(リストボックス)

	//レンダラー
	void ResetRenderer(void);								//レンダラーをリセット

	//ツリーフォーマット構造体
	void UpdateTfmt(int oldTreeMax);						//ツリーのフォーマット構造体を整理
	void DeleteTreeFormat(short treeId);					//ツリーのフォーマット構造体を消す
	void AddTreeFormat(short treeType,
		short treeState, short treeSize, short treeModel,
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	//ツリーの設置
	void SetTreeType(short treeId, short treeType);			//ツリーの種類を設定
	void SetTreeState(short treeId, short treeState);		//ツリーの状態を設定
	void SetTreeSize(short treeId, short treeSize);			//ツリーのサイズを設定
	void SetTreeModel(short treeId, short treeModel);		//ツリーのモデルを設定
	void SetTreePos(short treeId, const D3DXVECTOR3 pos);	//ツリーの位置を設定
	void SetTreeDir(short treeId, const D3DXVECTOR3 dir);	//ツリーの向きを設定
	void SetTree(short treeId, short treeType,				//ツリーの設置
		short treeState, short treeSize, short treeModel,
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

#endif
};