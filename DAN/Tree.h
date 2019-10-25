//-----------------------------------------------------------------------------
// ツリー基底クラス [Tree.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/13
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace treeNS
{
	// ツリー種別
	enum TREE_TYPE
	{
		ANALOG_TREE,
		DIGITAL_TREE
	};

	// 緑化状態
	enum GREEN_STATE
	{
		GREEN,
		DEAD
	};

	// サイズ
	enum TREE_SIZE
	{
		STANDARD,
		LARGE,
		VERY_LARGE
	};

	// モデル
	enum TREE_MODEL
	{
		A_MODEL,
		B_MODEL,
		C_MODEL,
		TREE_MAX
	};


	typedef struct TreeData
	{
		int treeID;						// 識別番号
		int model	;					// モデルID
		TREE_TYPE type;					// ツリータイプ
		GREEN_STATE geenState;			// 緑化状態
		TREE_SIZE size;					// 木のサイズ
		int hp;							// HP
		D3DXVECTOR3 initialPosition;	// 初期座標
		D3DXVECTOR3 initialDirection;	// 初期方角
		void zeroClear() { ZeroMemory(this, sizeof(TreeData)); }
	} TREESET;
}

//=============================================================================
//クラス定義
//=======================================================================T======
class Tree
{
private:
	treeNS::TreeData treeData;			// ツリーデータ
	Object leaf;						// 葉オブジェクト
	Object trunk;						// 幹オブジェクト
	BoundingSphere greeningArea;		// 緑化範囲
	LPD3DXMESH	attractorMesh;			// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;		// 重力（引力）発生オブジェクトマトリックス
	static int numOfTree;				// ツリーオブジェクトの総数

public:
	Tree(treeNS::TreeData _treeData);
	~Tree();
	void update(float frameTime);
	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);

	// Getter
	Object* getLeaf();
	Object* getTrunk();
	static int getNumOfTree();			// ツリーの数を取得
	treeNS::TreeData* getTreeData();	// ツリーデータを取得

	// Setter
	void setDataToTree(treeNS::TreeData _treeData);
};