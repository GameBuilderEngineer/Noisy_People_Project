//-----------------------------------------------------------------------------
// A*アルゴリズムによるパス検索クラス [Astar.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/20
//-----------------------------------------------------------------------------
#pragma once

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Base.h"
#include "MeshData.h"
#include "VertexAccessor.h"

//*****************************************************************************
// マクロ・列挙型定義
//*****************************************************************************
#define NUM_SURROUND_NODE	(7)					// 周辺ノードの数

// 地形タイプ
enum TERRAIN_TYPE
{// ※この順番やナンバリングはDodgeOrbEditerに揃えなければいけない
	CARDINALE_TERRAIN,			// 赤　　　　※未使用
	CIEL_TERRAIN,				// 青　　　　※未使用
	OBSTACLE,					// 障害物
	NORMAL_TERRAIN = 9			// 何もなし
};


// マップ座標構造体
typedef struct {
	int x;						// マップノードのX座標
	int y;						// マップノードのY座標
}MAPVECTOR2;

//// マップノード構造体
//typedef struct {
//	NODE_STATUS		status;		// ステータス
//	int				terrain;	// 地形タイプ
//	MAPVECTOR2		mapPos;		// マップ座標
//	D3DXVECTOR3		worldPos;	// ワールド座標
//}MAPNODE, **MAP;


//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 経路構造体
typedef struct
{
	// A*アルゴリズムによるパス検索の結果を格納する
	MAPVECTOR2				*ptr;				// 経路情報へのポインタ
	int						numRouteStep;		// 経路情報動的配列の要素数（移動座標の数）
	int						referenceRec;		// 経路情報動的配列の参照している要素番号を保管
}ROUTE;




//meshAnalyzerNS::Face* face;


//=============================================================================
// 名前空間
//=============================================================================
namespace aStarNS
{
	enum NODE_STATUS
	{
		NODE_NONE,			// 一度も開いていない
		NODE_OPEN,			// オープン済
		NODE_CLOSED,		// 基準ノードとして使用済みでクローズ
	};
	struct Vertex
	{
		WORD index;
		BYTE* ptr;
	};

	struct FaceNode
	{
		Vertex vtx[3];					
		DWORD adjacency[3];
		int status;
		void resetStatus() { status = NODE_NONE; }
	};

	// オープンリスト構造体　A*アルゴリズムで使用
	typedef struct _OPENLIST
	{
		int				key;			// 主キー
		NODE_STATUS		status;			// ステータス
		int				cost;			// 移動コスト
		int				heuristic;		// 推定コスト
		int				score;			// スコア
		DWORD			faceIndex;		// 面インデックス
#if 0
		MAPVECTOR2		mapPos;			// マップ座標
#endif
		_OPENLIST		*parent;		// 親ノード
		_OPENLIST		*next;			// 次の要素へのポインタ
	}OPENLIST;

}


//=============================================================================
//クラス定義
//=============================================================================
class AStar
{
private:
	MeshData* meshData;
	VertexAccessor* vtxAccessor;
	aStarNS::FaceNode *faceNode;

public:
	void initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor);
	void uninitialize();
	// 経路検索
	void pathSearch(DWORD start, DWORD dest);
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitRouteSearch(int type);					// AI経路探索の初期化処理
void UninitRouteSearch(void);					// AI経路探索の終了処理
void RouteSearch(/*int characterNo*/DWORD start, DWORD dest);
// 周辺ノードのマップ座標を計算
/*MAPVECTOR2*/DWORD CalcSurroundingNodePos(/*MAPVECTOR2 workPos, int position*/ DWORD faceIndex, int i);