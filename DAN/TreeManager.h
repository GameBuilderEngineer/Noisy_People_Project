//-----------------------------------------------------------------------------
// ツリー管理クラス [Tree.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
// 更新日 : 2019/11/12 【菅野 樹】
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshRenderer.h"
#include "Tree.h"
#include "AnalogTree.h"
#include "DigitalTree.h"
#include "DigitalTreeEffect.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace treeManagerNS
{	
	const float EFFECT_CYCLE = 0.1f;
}


//=============================================================================
//クラス定義
//=============================================================================
class TreeManager
{
private:
	std::vector<Tree*> treeList;				// ツリーポインタリスト
	StaticMeshRenderer* aTrunkRenderer;			// Aモデル幹描画オブジェクト
	StaticMeshRenderer* aLeafRenderer;			// Aモデル葉描画オブジェクト
	StaticMeshRenderer* bTrunkRenderer;			// Bモデル幹描画オブジェクト
	StaticMeshRenderer* bLeafRenderer;			// Bモデル葉描画オブジェクト
	StaticMeshRenderer* cTrunkRenderer;			// Cモデル幹描画オブジェクト
	StaticMeshRenderer* cLeafRenderer;			// Cモデル葉描画オブジェクト

	StaticMeshRenderer* aDTrunkRenderer;		// Aモデル幹描画オブジェクト
	StaticMeshRenderer* aDLeafRenderer;			// Aモデル葉描画オブジェクト
	StaticMeshRenderer* bDTrunkRenderer;		// Bモデル幹描画オブジェクト
	StaticMeshRenderer* bDLeafRenderer;			// Bモデル葉描画オブジェクト
	StaticMeshRenderer* cDTrunkRenderer;		// Cモデル幹描画オブジェクト
	StaticMeshRenderer* cDLeafRenderer;			// Cモデル葉描画オブジェクト



	float effectCycle;							//エフェクト周期
	DigitalTreeEffect*	digitalTreeEffect;		//デジタルツリーエフェクト
	int nextID;									// 次回ツリー発行ID
	LPD3DXMESH	attractorMesh;					// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;				// 重力（引力）発生オブジェクトマトリックス

	int greeningTreeNum;						//緑化本数
	float greeningRate;							//緑化率


public:
	void initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//ツリーの生成
	void createTree(treeNS::TreeData treeData);

	//アナログとデジタルの変換
	void swapDA(Tree* tree,int beforeType);

	//葉(アナログ)の描画登録/解除
	void registerLeafRendering(Object* leaf, int _model);
	void unRegisterLeafRendering(Object* leaf, int _model);

	//デジタルツリー描画登録/解除
	void registerDigital(Tree* tree);
	void unRegisterDigital(Tree* tree);

	//アナログツリー描画登録/解除
	void registerAnalog(Tree* tree);
	void unRegisterAnalog(Tree* tree);


	void destroyAllTree();
	int	issueNewTreeID();
	void outputGUI();

	void changeWireFrame();
	void changeSolid();

	// Getter
	std::vector<Tree*>& getTreeList();
	float getGreeningRate();
};
