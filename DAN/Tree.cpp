//-----------------------------------------------------------------------------
// ツリー基底クラス [Tree.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/13
//-----------------------------------------------------------------------------
#include "Tree.h"
#include "ImguiManager.h"
using namespace treeNS;


int Tree::numOfTree = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
Tree::Tree(treeNS::TreeData _treeData)
{
	treeData = _treeData;
	trunk.position = treeData.initialPosition;
	leaf.position = treeData.initialPosition;

	// treeData.initialDirectionを基にした向き転換

	// treeData.sizeを基にした拡大縮小

	// 半径取得
	BoundingSphere bs;
	bs.initialize(trunk.getPosition(), staticMeshNS::reference(staticMeshNS::A_TRUNK)->numBytesPerVertex);
	radius = bs.getRadius();

	// 重力付与
	trunk.onGravity = true;

	//// スケール変更
	//trunk.scale *= 0.4f;
	//leaf.scale *= 0.4f;

	numOfTree++;
}


//=============================================================================
// デストラクタ
//=============================================================================
Tree::~Tree()
{
	numOfTree--;
}


//=============================================================================
// 更新処理
//=============================================================================
void Tree::update(float frameTime)
{
	// 重力
	if (trunk.onGravity)
	{
		D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
		trunk.setGravity(gravityDirection, 9.8f);
		trunk.speed += trunk.acceleration * frameTime;
		trunk.position += trunk.speed * frameTime;
		grounding();

	}

	// オブジェクトのアップデート
	leaf.update();
	trunk.update();
}


//=============================================================================
// 重力発生メッシュ（接地メッシュ）の設定
//=============================================================================
void Tree::setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;
}


//=============================================================================
// Getter
//=============================================================================
Object* Tree::getLeaf() { return &leaf; }
Object* Tree::getTrunk() { return &trunk; }
int Tree::getNumOfTree(){ return numOfTree; }
TreeData* Tree::getTreeData() { return &treeData; }


//=============================================================================
// Setter
//=============================================================================
void Tree::setDataToTree(TreeData _treeData) { treeData = _treeData; }


#ifdef _DEBUG
//=============================================================================
// 接地処理
//=============================================================================
void Tree::grounding()
{
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	Ray gravityRay;
	gravityRay.update(trunk.position + D3DXVECTOR3(0,radius,0), gravityDirection);

	if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	{
		if (radius + 0.1f >= gravityRay.distance)
		{// エネミーは地上に接地している

			// 重力切る
			trunk.onGravity = false;
			trunk.setGravity(D3DXVECTOR3(0,0,0), 0);
		}
	}
}
#endif
