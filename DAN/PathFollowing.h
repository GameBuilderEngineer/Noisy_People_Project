//-----------------------------------------------------------------------------
// パスフォローイングクラス [PathFollowing.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/30
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"
#include "LinkedList.h"
#include "MeshData.h"
#include "VertexAccessor.h"


//=============================================================================
// クラス定義
//=============================================================================
class PathFollowing: public Base
{
private:
	MeshData* meshData;							// メッシュ情報
	VertexAccessor* vtxAccessor;				// 頂点アクセスオブジェクト

	// createVector()の引数としてもらう
	D3DXVECTOR3* out;							// 最終的な出力ベクトル
	DWORD* faceIndex;							// 現在位置しているポリゴン面インデックス
	D3DXVECTOR3 position;						// 現在位置している座標
	LinkedList<meshDataNS::Index2>* edgeList;	// エッジリスト

	// createVector()の中で使われる
	D3DXVECTOR3 pos1, pos2;
	D3DXVECTOR3 pos3, pos4;
	D3DXVECTOR3 edgeIntersection;				// 第1エッジ（次に通過するエッジ）との交点
	D3DXVECTOR3 nextEdgeIntersection;			// 第2エッジ（次の次に通過するエッジ）との交点
	D3DXVECTOR3 vecToNextEdge;					// 第2エッジへのベクトル（正規化済）
	D3DXVECTOR3 nor;							// 現在地ポリゴンの法線
	bool isProtrude;							// はみ出し判定
	int nearPos;								//
	D3DXVECTOR3 subGoal;						// サブゴール

	// Method
	void zeroClear()
	{
		MeshData* tempMeshData = meshData;
		VertexAccessor* tempVtxAccessor = vtxAccessor;
		ZeroMemory(this, sizeof(PathFollowing));
		meshData = tempMeshData;
		vtxAccessor = tempVtxAccessor;
	}
	void step1();
	void step2();
	void step3();
	void step4();

public:
	// 初期化
	void initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor);
	// 経路に沿う移動ベクトルを作成
	void createVector(D3DXVECTOR3* _out, D3DXVECTOR3 _position,
		DWORD* _faceIndex, LinkedList<meshDataNS::Index2>* _edgeList);
};
