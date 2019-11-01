//-----------------------------------------------------------------------------
// パスフォローイングクラス [PathFollowing.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/30
//-----------------------------------------------------------------------------
#include "PathFollowing.h"
using namespace meshDataNS;


//=============================================================================
// 初期化
//=============================================================================
void PathFollowing::initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor)
{
	meshData = _meshData;
	vtxAccessor = _vtxAccessor;
}


//=============================================================================
// 経路に沿う移動ベクトルを作成
//=============================================================================
void PathFollowing::createVector(
	D3DXVECTOR3* _out,
	D3DXVECTOR3 _position,
	DWORD* _faceIndex,
	LinkedList<meshDataNS::Index2>* _edgeList)
{
	// ポインタ以外を全てゼロクリア
	zeroClear();

	// 引数をセット
	out = _out;
	position = _position;
	faceIndex = _faceIndex;
	edgeList = _edgeList;

	if ((edgeList->nodeNum == 0))
	{
		return;
	}

	step1();		// [1]第1エッジの頂点座標を取得してエッジと現在地座標の交点を求める

	if (edgeList->nodeNum == 1)
	{// 残りのエッジが1つだけの場合
		subGoal = edgeIntersection;
	}
	else
	{// 残りのエッジが2つ以上の場合
		step2();	// [2]第2エッジの頂点座標を取得してエッジと現在地座標の交点を求める
		step3();	// [3]第2エッジへのベクトルを現在地ポリゴン面と並行になるようスリップさせておく
		step4();	// [4]サブゴール座標を求める
	}

	// ベクトルを作成して終了
	*out = (subGoal - _position);
	D3DXVec3Normalize(out, out);
}

// [1]第1エッジの頂点座標を取得してエッジと現在地座標の交点を求める
void PathFollowing::step1()
{
	Index2 edge = *edgeList->getValue(0);
	pos1 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[0]));
	pos2 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[1]));
	edgeIntersection = nearestPointOnLine(pos1, pos2, position);
}

// [2]第2エッジの頂点座標を取得してエッジと現在地座標の交点を求める
void PathFollowing::step2()
{
	Index2 edge = *edgeList->getValue(1);
	pos3 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[0]));
	pos4 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[1]));
	nextEdgeIntersection = nearestPointOnLine(pos3, pos4, position);
}

// [3]第2エッジへのベクトルを現在地ポリゴン面と並行になるようスリップさせておく
void PathFollowing::step3()
{
	nor = meshData->getFaceArray()[*faceIndex].nor;
	vecToNextEdge = nextEdgeIntersection - position;
	D3DXVec3Normalize(&vecToNextEdge, &vecToNextEdge);
	slip(vecToNextEdge, nor);
	slip(vecToNextEdge, -nor);
}

// [4] サブゴール座標を求める
void PathFollowing::step4()
{
	// 平面の方程式で第1エッジ上に平面を求め
	// 第2エッジへのベクトルと第1エッジの交点を求めたい
	// (1)平面の法線を準備
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, &nor, &(pos2 - pos1));
	if (cross = D3DXVECTOR3(0.0f, 0.0, 0.0f))
	{// position, pos1, pos2が同一直線上だった場合(超絶に低い可能性だが)外積が無効のため
		subGoal = position;	// このフレームでcreateVector()はベクトル(0, 0, 0)を返す
	}
	D3DXVec3Normalize(&cross, &cross);
	D3DXVECTOR3 temp = edgeIntersection + cross;
	if (D3DXVec3LengthSq(&(temp - position)) > D3DXVec3LengthSq(&(edgeIntersection - position)))
	{// 平面の法線をいつでもposition側に向かせる
		cross = -cross;
	}

	// (2)平面の方程式に代入して交点までの距離を求める
	float d = -(cross.x * edgeIntersection.x
		+ cross.y * edgeIntersection.y
		+ cross.z * edgeIntersection.z);

	float distance = cross.x * position.x
		+ cross.y * position.y
		+ cross.z * position.z
		+ d;

	// (3) 交点を求める
	D3DXVECTOR3 intersectionOfEdgeAndNextEdge = position + vecToNextEdge * distance;

	// 基準座標と第1エッジとの交点の中間点をサブゴールとする
	subGoal = (intersectionOfEdgeAndNextEdge + edgeIntersection) / 2.0f;
}


// ゴミ

//float firstEdgeRadian;
//float radianContainingSecondEdge;
//
//// 第2エッジとの交点が第1エッジのどちらの頂点に近いかを求める
//float tempLen1 = D3DXVec3LengthSq(&(nextEdgeIntersection - pos1));
//float tempLen2 = D3DXVec3LengthSq(&(nextEdgeIntersection - pos2));
//if (tempLen1 < tempLen2)
//{
//	nearPos = 1;
//}
//else
//{
//	nearPos = 2;
//}
//
////「第1エッジの各頂点⇔現在地座標」のなす角度
//formedRadianAngle(&firstEdgeRadian, pos1 - position, pos2 - position);
//
////「第1エッジの現在地座標に近い頂点⇔現在地座標」と「nextEdgeIntersection⇔現在地座標」のなす角
//if (nearPos == 1)
//{
//	if (false == formedRadianAngle(&radianContainingSecondEdge, pos2 - position, vecToNextEdge))
//		return;
//}
//else// nearPos == 2
//{
//	if (false == formedRadianAngle(&radianContainingSecondEdge, pos1 - position, vecToNextEdge))
//		return;
//}
//
//// はみ出しを検知
//if (firstEdgeRadian < radianContainingSecondEdge)
//{
//	isProtrude = true;
//}


//// はみ出しがあれば第2エッジとの交点と近い第1エッジの頂点座標を基準とする
//if (isProtrude)
//{
//	if (nearPos == 1)		work = pos1;
//	else if (nearPos == 2)	work = pos2;
//}