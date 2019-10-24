//-----------------------------------------------------------------------------
// A*アルゴリズムによるパス検索クラス [Astar.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/20
//-----------------------------------------------------------------------------
#include "Astar.h"
using namespace aStarNS;


//*****************************************************************************
// マクロ・列挙型定義
//*****************************************************************************
#define INTERVAL_ROUTE_SEARCH	(5)		// 経路探索の更新フレーム間隔（必ず奇数）
#define  SURROUNDING_NODE_MAX	(8)		// 周囲のノードの数 

// マップノードの描画カラー
#define ROUTE_R					(1.0f)	// 赤
#define ROUTE_G					(0.7f)	// 緑
#define ROUTE_B					(0.0f)	// 青


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// オープンリストにノードを追加
HRESULT AddOpenList(OPENLIST **head, OPENLIST *parentNode, OPENLIST setData);

// オープンリストの解放
void ReleaseOpenList(OPENLIST **head);

// オープンリストからノードを探す
OPENLIST *SearchNodeFromOpenList(OPENLIST **head, int searchKey);

// オープンリストから最も低スコアなノードを取得
OPENLIST *GetLeastCostNode(OPENLIST *head);

// オープンリストから最も推定コストの低いノードを取得
OPENLIST *GetLeastHeuristicNode(OPENLIST *head);

// オープンリストから経路を取得
void GetPathFromOpenList(OPENLIST *work, int numRouteArray);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
MAPVECTOR2		*route;					// 経路情報へのポインタ	
int				cntRoute;				// 経路の移動数をカウント
static int		mapX, mapY;				// マップ座標XY最大値


//=============================================================================
// 初期化処理
//=============================================================================
void AStar::initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor)
{
	meshData = _meshData;
	vtxAccessor = _vtxAccessor;

	// ポリゴン面ノード配列を作成する
	faceNode = new FaceNode[meshData->getNumFaces()];
	for (int i = 0; i < meshData->getNumFaces(); i++)
	{
		for (int k = 0; k < 3; k++)
		{
			faceNode->vtx[k].index = meshData->getVertexIndexPointerFromFaceIndex(
				meshData->getIndexArray(), i)[k];
			faceNode->vtx[k].ptr = meshData->getVertexPointerFromVertexIndex(
				meshData->getVtxArray(), faceNode->vtx[k].index);
		}
		for (int k = 0; k < 3; k++)
		{
			faceNode->adjacency[k] = *meshData->getAdjacentFaceIndexPointerFromFaceIndex(
				meshData->getAdjacencyArray(), i + k);
		}
		faceNode[i].resetStatus();
	}

#if 1
	setDataDirectory();
	FILE* fp;
	fp = fopen("FaceNodeArray.txt", "w");
	fprintf(fp, "ポリゴン面ノード配列の中身\n出力元：AStar.cpp\n");

	for (int i = 0; i < meshData->getNumFaces(); i++)
	{
		fprintf(fp, "【ポリゴンIndex%d】\n" ,i);
		for (int k = 0; k < 3; k++)
		{
			D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
			//D3DXVECTOR3* position = (D3DXVECTOR3*)vtxAccessor->getPointer(
			//	vtxAccess::POSITION, &meshData->getVtxArray()[i * 3 * meshData->getStride() + k * meshData->getStride()]);
			fprintf(fp, "頂点%d Ind：%d　Pos(%f, %f, %f)\n", k, faceNode[i].vtx[k].index, position.x, position.y, position.z);
		}
	}

	fclose(fp);
#endif

#if 0
	int enemyMax = GetNumberEnemy();

	// 経路・目的地情報の初期化
	for (int i = 0; i < enemyMax; i++)
	{
		AI_MEMORY *memory = GetAiMemory(i);
		for (int routeType = 0; routeType < ROUTE_TYPE_MAX; routeType++)
		{
			memory->route[routeType].ptr = NULL;
			memory->route[routeType].numRouteStep = 0;
			memory->route[routeType].referenceRec = 0;
			memory->destination[routeType] = NULL;
		}
	}

	// マップサイズを設定
	GAMEDATA *gameData = GetGameData();	
	mapX = gameData->mapX;
	mapY = gameData->mapY;
#endif
}


//=============================================================================
// 終了処理
//=============================================================================
void AStar::uninitialize(void)
{
	SAFE_DELETE_ARRAY(faceNode);

#if 0
	int enemyMax = GetNumberEnemy();

	for (int i = 0; i < enemyMax; i++)
	{
		AI_MEMORY *memory = GetAiMemory(i);

		// 経路情報メモリの解放
		for (int routeType = 0; routeType < ROUTE_TYPE_MAX; routeType++)
		{
			free(memory->route[routeType].ptr);
			memory->route[routeType].ptr = NULL;
		}
	}
#endif
}


//=============================================================================
// AI経路探索処理
//=============================================================================
void AStar::pathSearch(/*int characterNo*/DWORD start, DWORD dest)
{
#if 0
	// 経路探索の間隔調整
	static DWORD cntSearch = 0;
	cntSearch++;
	if (cntSearch % INTERVAL_ROUTE_SEARCH)
	{// INTERVA_ROUTE_SEARCHが奇数でないと、
	 // NPC_VS_NPCのとき片一方の経路探索が更新されなくなる
		return;
	}


	///////////////////////////////////////////////////////////////////////////
	//////////////////////////// 探索経路の決定 ///////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CHARACTER *enemy = GetCharacter(characterNo);	// エネミーを取得
	AI_MEMORY *memory = GetAiMemory(enemy->aiNo);	// 記憶領域を取得
	MAP map = GetMap();								// マップを取得

	// 自分のマップ座標を取得
	MAPVECTOR2 myMapPos = TransrateWorldPosToMapPos(enemy->transform->pos);

	/* 経路探索セレクター */
	// 各NPCAIごとに、ループ毎、目的地ノードを相手座標、オーブ座標、アイテム座標と変える
	// 各NPCAIはこの３つの経路情報を常時保持し、更新を続ける

	// 今フレームでどの経路を探索するかを決定
	memory->searchSelecter++;
	memory->searchSelecter = memory->searchSelecter % 3;
	ROUTE_TYPE routeType = (ROUTE_TYPE)memory->searchSelecter;

	// 目的地のノードを設定する（先に他のAIモジュールでポインタに中身を入れておく）
	MAPNODE* destinationNode = memory->destination[routeType];

	// 目的地が設定されていなければ終了
	if (destinationNode == NULL)
	{
		return;
	}

	if (routeType == ROUTE_TO_ORB)
	{
		routeType = ROUTE_TO_ORB;
	}

#endif

	///////////////////////////////////////////////////////////////////////////
	///////////////////////// Aスターアルゴリズム /////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	// 下ごしらえ
	OPENLIST *head = NULL;							// オープンリストの先頭
	BOOL isBlindAlley = FALSE;						// 袋小路ではない

	// 基準ノードに開始ノードを設定する
	OPENLIST *work = NULL;
	OPENLIST data;
	data.cost = 0;									// 開始地点だから0
	data.heuristic = 0;								// 開始地点だから0
	data.score = 0;									// 開始地点だから0
	data.faceIndex = start;
#if 0
	data.mapPos = myMapPos;							// 自分のマップ座標
#endif
	data.parent = NULL;								// 開始地点だからNULL
	data.next = NULL;								// 開始地点だからNULL

	// オープンリストに開始ノードを追加する
	AddOpenList(&head, work, data);

	//--------------
	// メインループ
	//--------------
	while (head != NULL)
	{
		// オープンリストの中で最も低コストであるノードを基準ノードに設定
		work = GetLeastCostNode(head);

		if (work == NULL)
		{// 袋小路に陥っているならループから出る
			isBlindAlley = TRUE;
			break;
		}

		// 基準ノードが目的地のノードである場合
		if (work->faceIndex == dest)
		{
			break;	// 経路の完成
		}

		// 基準ノードが目的地のノードでない場合
		else
		{
			// 基準ノードの周りをオープンしていく
			for (int i = 0; i < 3/*SURROUNDING_NODE_MAX*/; i++)
			{
				// オープンするノードのマップ座標を計算
				/*MAPVECTOR2*/DWORD ans = CalcSurroundingNodePos(work->faceIndex, i);

				if (/*pos.x*/ans == -1)
				{// 座標が存在しない
					continue;
				}
#if 0
				if ((map[pos.y][pos.x].status == NODE_OPEN
						|| map[pos.y][pos.x].status == NODE_CLOSED
						|| map[pos.y][pos.x].terrain == OBSTACLE)
					/*&& MapPosEqual(pos, destinationNode[routeType].mapPos) == FALSE*/)
				{// オープン対象でも目的地でもない
					continue;
				}
#endif
				if (faceNode[ans].status == (NODE_OPEN || NODE_CLOSED))
				{
					continue;
				}

				// ここでノードをオープンする
#if 0
				map[pos.y][pos.x].status = NODE_OPEN;					// マップ側
#endif
				faceNode[ans].status = NODE_OPEN;

				data.status = NODE_OPEN;								// オープンリスト側


				// オープンリストの他のデータ作成
				// 移動コスト
				data.cost = work->cost + 1;	
#if 0
				int differenceX, differenceY;
				differenceX = abs(destinationNode->mapPos.x - pos.x);
				differenceY = abs(destinationNode->mapPos.y - pos.y);
				// 推定コスト:縦横の最大値に縦横の合計を加えることで、毎フレーム正しく目的地方向に経路をとるようになる
				data.heuristic = max(differenceX, differenceY) + differenceX + differenceY;
				data.score = data.cost + data.heuristic;				// スコア
				data.mapPos = pos;										// 座標
#endif

				// 現状の三角形の重心を求めて
				// 各辺に垂線を下ろしその距離を保管　← コスト
				// その各辺と垂線の交点から、目的地の三角形重心までの距離を測り保管←ヒューリスティック
				data.heuristic = 0;
				data.faceIndex = ans;

				// 以上をオープンリストに追加
				AddOpenList(&head, work, data);
			}

#if 0			// 基準ノードをクローズ
			map[work->mapPos.y][work->mapPos.x].status = NODE_CLOSED;	// マップ側
			work->status = NODE_CLOSED;									// オープンリスト側
#endif
			faceNode[work->faceIndex].status = NODE_CLOSED;
			work->status = NODE_CLOSED;
		}
	}

#if 0
	// マップノードのステータスを初期化
	for (int y = 0; y < mapY; y++)
	{
		for (int x = 0; x < mapX; x++)
		{
			map[y][x].status = NODE_NONE;			// オープンやクローズドをすべてNONEに復帰
		}
	}
#endif
	for (int i = 0; i < 1000; i++)
	{
		faceNode[i].status = NODE_NONE;
	}


	// 袋小路の場合最も推定コストの低いノードを目的地として設定する
	if (isBlindAlley)
	{
		work = GetLeastHeuristicNode(head);
	}

#if 0
	// タイルカラーのリセット
	if (memory->searchSelecter == memory->moveRouteType)
	{
		ResetColorTile();
	}
#endif

	//// 経路情報（動的配列）のメモリを確保
	//if ((route = (MAPVECTOR2*)malloc(sizeof(MAPVECTOR2) * work->cost)) == NULL)
	//{
	//	MessageBox(NULL, TEXT("メモリの確保に失敗しました。\nアプリケーションを終了します。"), TEXT("SystemError"), MB_OK);
	//	PostQuitMessage(0);
	//}


	//// オープンリストから経路情報に経路を追加
	//GetPathFromOpenList(work, work->cost);


	//// AI記憶領域にストア
	//free(memory->route[routeType].ptr);					// 元のメモリを解放
	//memory->route[routeType].ptr = route;				// ポインタ再設定
	//memory->route[routeType].numRouteStep = work->cost;	// 経路情報の要素数を記録
	//memory->route[routeType].referenceRec = 0;			// 参照記録を初期化
	//route = NULL;

	//// オープンリストを解放
	//ReleaseOpenList(&head);
}


//=============================================================================
// オープンリストにノードを追加
//=============================================================================
HRESULT AddOpenList(OPENLIST **head, OPENLIST *parentNode, OPENLIST setData)
{
	OPENLIST *p;

	// 記憶領域の確保
	if ((p = (OPENLIST*)malloc(sizeof(OPENLIST))) == NULL)
	{
		MessageBox(NULL, TEXT("メモリの確保に失敗しました。\nアプリケーションを終了します。"), TEXT("SystemError"), MB_OK);
		PostQuitMessage(0);
	}

	*p = setData;			// データを登録
	p->parent = parentNode;	// 親ノードを登録

	// ポインタのつなぎ換え
	p->next = *head;		// 今までの先頭ポインタを親ポインタに設定
	*head = p;				// 新たな領域を先頭ポインタに設定

	return S_OK;
}


//=============================================================================
// オープンリストの解放
//=============================================================================
void ReleaseOpenList(OPENLIST **head)
{
	OPENLIST *p;

	// 確保していた全メモリを解放する
	// 次ポインタがNULLまで処理
	while (*head != NULL)
	{
		p = (*head)->next;
		free(*head);
		*head = p;
	}
}


//=============================================================================
// オープンリストからノードを探す
//=============================================================================
OPENLIST *SearchNodeFromOpenList(OPENLIST **head, int searchKey)
{
	OPENLIST *p = *head;		// リストの先頭を指す

	// リストの先頭でserchKeyを見つけた場合
	if (p != NULL && p->key == searchKey)
	{
		return p;
	}

	// リストの先頭がNULLなら終了
	if (p == NULL)
	{
		return NULL;
	}

	// searchKeyを見つける
	while (p->next != NULL && p->next->key != searchKey)
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
// オープンリストから最も低スコアなノードを取得
//=============================================================================
OPENLIST *GetLeastCostNode(OPENLIST *head)
{
	OPENLIST *p = head;				// リストの先頭を指す
	OPENLIST *leastScoreNode = p;	// 最低コストのノード
	int leastScore = INT_MAX;		// 最低コスト
	BOOL isEnterLoop = FALSE;		// whileループに入っていない

	// リスト末尾までノードを探索
	while (p->next != NULL)
	{
		isEnterLoop = TRUE;			// whileループに入った

		if (p->score < leastScore
			&& p->status == NODE_OPEN)
		{// コストがより小さくかつ一度も基準ノードにしていない条件で探す

			leastScore = p->score;
			leastScoreNode = p;
		}

		p = p->next;				// 次の要素に捜査対象を変更
	}

	if (isEnterLoop == TRUE && leastScore == INT_MAX)
	{// リスト中にオ―プンノードがない場合（袋小路状態）
		return NULL;
	}

	return leastScoreNode;
}


//=============================================================================
// オープンリストから最も推定コストの低いノードを取得
//=============================================================================
OPENLIST *GetLeastHeuristicNode(OPENLIST *head)
{
	OPENLIST *p = head;					// リストの先頭を指す
	OPENLIST *leastHeuristicNode = p;	// 最低コストのノード
	int leastHeuristic = INT_MAX;		// 最低コスト

	// リスト末尾までノードを探索
	while (p->next != NULL)
	{
		if (p->heuristic <= leastHeuristic)
		{// 推定コストが今の最低コスト以下であるなら
			leastHeuristic = p->heuristic;

			if (p->cost <= leastHeuristicNode->cost)
			{// 移動コストも今の最低コストノード以下であれば
				leastHeuristicNode = p;
			}// これで道順も違和感なくなる
		}

		p = p->next;					// 次の要素に捜査対象を変更
	}

	return leastHeuristicNode;
}



//=============================================================================
// オープンリストから経路を取得
//=============================================================================
void GetPathFromOpenList(OPENLIST *work, int numRouteArray)
{
	if (numRouteArray > 0)
	{// 経路情報配列の後ろから順に座標を代入していく
#if 0
		route[numRouteArray - 1] = work->mapPos;
#endif
		//route[numRouteArray - 1] = work->faceIndex;
		numRouteArray--;
#if 0
		// 移動先までのタイルに色を描画
		AI_MEMORY *memory = GetAiMemory(0);
		if (memory->searchSelecter == memory->moveRouteType)
		{
			SetColorTile(work->mapPos.x, work->mapPos.y, D3DXCOLOR(ROUTE_R, ROUTE_G, ROUTE_B, 1.0f));
		}
#endif

	}

	// 親ポインタがNULLまで再帰処理
	if (work->parent != NULL)
	{
		GetPathFromOpenList(work->parent, numRouteArray);
	}
}


//=============================================================================
// 周辺ノードのマップ座標を計算
//=============================================================================
DWORD CalcSurroundingNodePos(/*MAPVECTOR2 workPos, int position*/ DWORD faceIndex, int i)
{
#if 0
	MAPVECTOR2 ans;	
	GAMEDATA *gameData = GetGameData();

	switch (position)
	{
	case 0:	// 左上
		ans.x = workPos.x - 1;
		ans.y = workPos.y - 1;
		break;

	case 1:	// 上
		ans.x = workPos.x;
		ans.y = workPos.y - 1;
		break;

	case 2:	// 右上
		ans.x = workPos.x + 1;
		ans.y = workPos.y - 1;
		break;

	case 3:	// 左
		ans.x = workPos.x - 1;
		ans.y = workPos.y;
		break;

	case 4:	// 右
		ans.x = workPos.x + 1;
		ans.y = workPos.y;
		break;

	case 5:	// 左下
		ans.x = workPos.x - 1;
		ans.y = workPos.y + 1;
		break;

	case 6:	// 下
		ans.x = workPos.x;
		ans.y = workPos.y + 1;
		break;

	case 7:	// 右下
		ans.x = workPos.x + 1;
		ans.y = workPos.y + 1;
		break;
	}

	if ((ans.x < 0 || ans.x >= gameData->mapX)
		|| (ans.y < 0 || ans.y >= gameData->mapY))
	{// 存在しない座標の場合
		ans.x = -1;
		ans.y = -1;
	}
	
	return ans;
#endif
 
	return 0;//  faceNode[faceIndex].adjacency[i];
}