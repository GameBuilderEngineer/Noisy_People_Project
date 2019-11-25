//-----------------------------------------------------------------------------
// A*アルゴリズムによるパス検索クラス [Astar.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/20
//-----------------------------------------------------------------------------
#include "Astar.h"
using namespace meshDataNS;
using namespace aStarNS;


//=============================================================================
// 初期化処理
//=============================================================================
void AStar::initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor)
{
	meshData = _meshData;
	vtxAccessor = _vtxAccessor;
}


//=============================================================================
// 終了処理
//=============================================================================
void AStar::uninitialize(void)
{

}


//=============================================================================
// 経路探索
//[引数1] DWORD start　開始ポリゴン面インデックス
//[引数2] DWORD dest 　目的地ポリゴン面インデックス
//[引数3] DWORD start　開始座標
//[引数4] DWORD start　目的地座標
//=============================================================================
LinkedList<meshDataNS::Index2>* AStar::pathSearch(
	DWORD startIndex, DWORD destIndex, D3DXVECTOR3 startPos, D3DXVECTOR3 destPos)
{
	releaseAStarNode();		// A*ノードリストを解放
	head = NULL;			// 先頭ノード初期化
	work = NULL;			// 作業ノード初期化
	isImpasse = false;		// 袋小路フラグ初期化

	// 開始ノードをリストに追加
	AStarNode startNode;
	startNode.faceIndex = startIndex;
	startNode.status = INVALID;
	startNode.cost = startNode.heuristic = startNode.score = 0.0f;
	startNode.edge.index[0] = startNode.edge.index[1] = -1;
	startNode.pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	startNode.parent = startNode.next = NULL;
	addAStarNode(work, startNode);

	// メインループ
	while (head != NULL)
	{
		// リストの中で最も低スコアのノードを基準ノードに設定
		work = getLeastCostNode();

		// 経路が袋小路もしくは目的地ノードまで探索完了
		if (work == NULL || work->faceIndex == destIndex)
		{
			if(work == NULL) isImpasse = true;
			break;
		}

		// 基準ノードの周りをオープン
		for (int i = 0; i < 3; i++)
		{
			DWORD adjacentIndex = meshData->getFaceArray()[work->faceIndex].adjacency[i];
			if (adjacentIndex == -1){ continue; }								// 隣接面がない
			if (searchNodeByFaceIndex(adjacentIndex) != NULL) { continue; }		// リストに既出

			// 新しいノードをリストに追加
			AStarNode data;
			data.faceIndex = adjacentIndex;
			data.status = OPEN;
			float len;
			culcDistanceToEdge(&len, &data.pos, work->pos, work->faceIndex, adjacentIndex);
			data.cost = work->cost + len;
			data.heuristic = D3DXVec3Length(&(destPos - data.pos));
			data.score = data.cost + data.score;
			data.edge = meshData->getSharingVertexIndexFromTwoFace(work->faceIndex, data.faceIndex);
			addAStarNode(work, data);
		}

		// 基準ノードをクローズ
		work->status = CLOSED;
	}

	// 袋小路の場合最も推定コストの低いノードを目的地として設定する
	if (isImpasse)
	{
		//work = getLeastHeuristicNode();
		return NULL;
	}

	// エッジリストを作成し返却する
	return createEdgeList(edgeList = new LinkedList<meshDataNS::Index2>, work);
}


//=============================================================================
// リストにノードを追加
//=============================================================================
HRESULT AStar::addAStarNode(AStarNode *parentNode, AStarNode setData)
{
	AStarNode *p;
	if ((p = (AStarNode*)malloc(sizeof(AStarNode))) == NULL)
	{
		MessageBox(NULL, TEXT("メモリの確保に失敗しました。\nアプリケーションを終了します。"), TEXT("SystemError"), MB_OK);
		PostQuitMessage(0);
	}

	*p = setData;			// データを登録
	p->parent = parentNode;	// 親ノードを登録
	p->next = head;
	head = p;
	return S_OK;
}


//=============================================================================
// リストの解放
//=============================================================================
void AStar::releaseAStarNode()
{
	// 確保していた全メモリを解放する
	AStarNode *p;
	while (head != NULL)
	{
		p = head->next;
		free(head);
		head = p;
	}
}


//=============================================================================
// リストから最も低スコアなノードを取得
//=============================================================================
AStarNode* AStar::getLeastCostNode()
{
	AStarNode *p = head;			// リストの先頭を指す
	AStarNode *leastScoreNode = p;	// 最低スコアのノード
	float leastScore = FLT_MAX;		// 最低スコア
	bool isEnterLoop = false;		// whileループに入っていない

	// リスト末尾までノードを探索
	while (p->next != NULL)
	{
		isEnterLoop = true;			// whileループに入った

		if (p->score < leastScore
			&& p->status == OPEN)
		{// スコアがより小さくかつ一度も基準ノードにしていない条件で探す

			leastScore = p->score;
			leastScoreNode = p;
		}

		p = p->next;				// 次の要素に捜査対象を変更
	}

	if (isEnterLoop == true && leastScore == FLT_MAX)
	{// リスト中にオ―プンノードがない場合（袋小路状態）
		return NULL;
	}

	return leastScoreNode;
}


//=============================================================================
// リストから最も推定コストの低いノードを取得
//=============================================================================
AStarNode* AStar::getLeastHeuristicNode()
{
	AStarNode *p = head;					// リストの先頭を指す
	AStarNode *leastHeuristicNode = p;	// 最低推定コストのノード
	float leastHeuristic = FLT_MAX;		// 最低推定コスト

	// リスト末尾までノードを探索
	while (p->next != NULL)
	{
		if (p->heuristic <= leastHeuristic)
		{// 推定コストが今の最低コスト以下であるなら
			leastHeuristic = p->heuristic;

			if (p->cost <= leastHeuristicNode->cost)
			{// 移動コストも今の最低コストノード以下であれば// ●
				leastHeuristicNode = p;
			}// これで道順も違和感なくなる
		}

		p = p->next;					// 次の要素に捜査対象を変更
	}

	return leastHeuristicNode;
}


//=============================================================================
// リストからノードを探す
//=============================================================================
AStarNode* AStar::searchNodeByFaceIndex(DWORD searchIndex)
{
	AStarNode* p = head;		// リストの先頭を指す

	// リストの先頭でsearchIndexを見つけた場合
	if (p != NULL && p->faceIndex == searchIndex)
	{
		return p;
	}

	// リストの先頭がNULLなら終了
	if (p == NULL)
	{
		return NULL;
	}

	// searchIndexを見つける
	while (p->next != NULL && p->next->faceIndex != searchIndex)
	{
		p = p->next;
	}

	if (p->next == NULL)
	{
		return NULL;
	}

	return p->next;
}


//=============================================================================
// リストから経路を取得
//=============================================================================
LinkedList<meshDataNS::Index2>* AStar::createEdgeList(
	LinkedList<meshDataNS::Index2>* edgeList, aStarNS::AStarNode *work)
{
	if (work->parent != NULL)
	{
		edgeList->insertFront(work->edge);
		createEdgeList(edgeList, work->parent);
	}

	return edgeList;
}


//=============================================================================
// 座標から2つのポリゴンの境界線までの距離を求める
//=============================================================================
float AStar::culcDistanceToEdge(float* outLen, D3DXVECTOR3* outPos, D3DXVECTOR3 inPos, DWORD faceInd1, DWORD faceInd2)
{
	// 共有している頂点インデックスを取得
	Index2 sharing = meshData->getSharingVertexIndexFromTwoFace(faceInd1, faceInd2);
	if (sharing.index[0] == -1 || sharing.index[1] == -1) return -1;// 面が接していない

	// インデックスの頂点座標を取得
	D3DXVECTOR3* pos1 = (D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(sharing.index[0]));
	D3DXVECTOR3* pos2 = (D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(sharing.index[1]));

	// 入力座標と面境界線の交点を求める
	D3DXVECTOR3 nearestPoint = nearestPointOnLine(*pos1, *pos2, inPos);
	if(outPos != NULL) *outPos = nearestPoint;

	// 交点までの距離を求める
	float ans;
	if (nearestPoint == *pos1)		ans = D3DXVec3Length(&(*pos1 - inPos));
	else if(nearestPoint == *pos2)	ans = D3DXVec3Length(&(*pos2 - inPos));
	else							ans = D3DXVec3Length(&(nearestPoint - inPos));
	if(outLen != NULL) *outLen = ans;
	return ans;
}
