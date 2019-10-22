//-----------------------------------------------------------------------------
// ツリー基底クラス [Tree.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/13
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshObject.h"
#include "BoundingSphere.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace treeNS
{
	enum TREE_TYPE
	{
		ANALOG_TREE,
		DIGITAL_TREE
	};

	enum GREEN_STATE
	{
		GREEN,
		DEAD
	};

	enum TREE_SIZE
	{
		STANDARD,
		LARGE,
		VERY_LARGE
	};

	enum TREE_MODEL
	{
		TREE_01,
		TREE_02,
		TREE_03,
		TREE_MAX
	};

	struct TreeData
	{
		int id;							// 識別番号
		int modelId;					// モデルID
		TREE_TYPE type;					// ツリータイプ
		GREEN_STATE geenState;			// 緑化状態
		TREE_SIZE size;					// 木のサイズ
		int hp;							// HP
		int initialPosition;			// 初期座標
		void initialize() { ZeroMemory(this, sizeof(TreeData)); }
	};
}


//=============================================================================
//クラス定義
//=======================================================================T======
class Tree: public StaticMeshObject
{
private:
	treeNS::TreeData* treeData;

	// Static
	// ↓サイズの数静的メンバでもって自分のサイズのものを参照すればいい
	BoundingSphere greeningArea;		// 緑化範囲
	static int numOfTree;				// ツリーオブジェクトの総数

public:
	Tree();
	~Tree();
	virtual void update(float frameTime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	// Getter
	static int getNumOfTree();			// ツリーの数を取得
	treeNS::TreeData* getTreeData();	// ツリーデータを取得

	// Setter
	void setDataToTree(treeNS::TreeData* _treeData);
};
