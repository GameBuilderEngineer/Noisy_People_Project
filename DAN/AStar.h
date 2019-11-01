//-----------------------------------------------------------------------------
// A*アルゴリズムによるパス検索クラス [Astar.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/20
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"
#include "LinkedList.h"
#include "VertexAccessor.h"
#include "MeshData.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace aStarNS
{
	// ノードステータス
	enum NODE_STATUS
	{
		INVALID,		// 無効値（リストに未追加)
		OPEN,			// オープン済
		CLOSED,			// 基準ノードとして使用済みでクローズ
	};

	// Aスターノード構造体
	struct AStarNode
	{
		DWORD				faceIndex;			// 面インデックス
		NODE_STATUS			status;				// ステータス
		float				cost;				// 移動コスト
		float				heuristic;			// 推定コスト
		float				score;				// スコア
		meshDataNS::Index2	edge;				// オープンするにあたり通過したエッジ
		D3DXVECTOR3			pos;				// エッジ上の通過座標（コスト計算に使用）
		AStarNode			*parent;			// 親ノード
		AStarNode			*next;				// 次の要素へのポインタ
	};
}


//=============================================================================
//クラス定義
//=============================================================================
class AStar: public Base
{
private:
	MeshData* meshData;							// メッシュデータオブジェクト
	VertexAccessor*	vtxAccessor;				// 頂点アクセスオブジェクト
	aStarNS::AStarNode*	head;					// 先頭ノード
	aStarNS::AStarNode*	work;					// 作業ノード
	bool isImpasse;								// 袋小路フラグ
	LinkedList<meshDataNS::Index2>*	edgeList;	// エッジリストのポインタ

public:
	// 初期化
	void initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor);
	// 終了処理
	void uninitialize();
	// 経路探索
	LinkedList<meshDataNS::Index2>* pathSearch(DWORD startIndex, DWORD destIndex,
		D3DXVECTOR3 startPos, D3DXVECTOR3 destPos);
	// リストにノードを追加
	HRESULT addAStarNode(aStarNS::AStarNode *parentNode, aStarNS::AStarNode setData);
	// リストの解放
	void releaseAStarNode();
	// リストから最も低スコアなノードを取得
	aStarNS::AStarNode* getLeastCostNode();
	// リストから最も推定コストの低いノードを取得
	aStarNS::AStarNode* getLeastHeuristicNode();
	// リストから経路を取得
	LinkedList<meshDataNS::Index2>* createEdgeList(LinkedList<meshDataNS::Index2>* edgeList, aStarNS::AStarNode *work);
	// リストからノードを探す
	aStarNS::AStarNode* searchNodeByFaceIndex(DWORD searchIndex);
	// 座標から2つのポリゴンの境界線までの距離を求める
	float culcDistanceToEdge(float* outLen, D3DXVECTOR3* outPos, D3DXVECTOR3 inPos, DWORD faceInd1, DWORD faceInd2);
};
