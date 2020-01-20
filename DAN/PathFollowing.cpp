//-----------------------------------------------------------------------------
// パスフォローイングクラス [PathFollowing.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/30
//-----------------------------------------------------------------------------
#include "PathFollowing.h"
using namespace meshDataNS;

#define SQUARE_FOLLOW

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
// ※[memo]残りのエッジが2つ以上ある場合にNavigationMesh::steering()からこの関数に入る
void PathFollowing::createVector(
	D3DXVECTOR3* _out,
	D3DXVECTOR3 _position,
	DWORD* _faceIndex,
	LinkedList<meshDataNS::Index2>* _edgeList)
{
#ifdef SQUARE_FOLLOW 

	// ポインタ以外を全てゼロクリア
	zeroClear();

	// 引数をセット
	out = _out;
	position = _position;
	faceIndex = _faceIndex;
	edgeList = _edgeList;

	step1();		// [1]第1エッジの頂点座標を取得してエッジと現在地座標の交点を求める
	if (edgeList->nodeNum < 4)
	{// 残りのエッジが3個以下の場合
		subGoal = edgeIntersection;
	}
	else
	{
		step2();	// [2]第2エッジの頂点座標を取得してエッジと現在地座標の交点を求める
		step3();	// [3]第2エッジへのベクトルを現在地ポリゴン面と並行になるようスリップさせておく
		step4();	// [4]サブゴール座標を求める
	}

	// ベクトルを作成して終了
	*out = (subGoal - _position);
	D3DXVec3Normalize(out, out);
#else
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
#endif
}
#ifdef SQUARE_FOLLOW 
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
	Index2 edge = *edgeList->getValue(2);
	pos3 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[0]));
	pos4 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[1]));
	nextEdgeIntersection = nearestPointOnLine(pos3, pos4, position);
}

// [3]ベクトルを現在地ポリゴン面と並行になるようスリップさせておく
void PathFollowing::step3()
{
	nor = meshData->getFaceArray()[*faceIndex].nor;
	vecToEdge = edgeIntersection - position;
	vecToNextEdge = nextEdgeIntersection - position;
	D3DXVec3Normalize(&vecToEdge, &vecToEdge);
	D3DXVec3Normalize(&vecToNextEdge, &vecToNextEdge);
	vecToEdge = slip(vecToEdge, nor);
	vecToEdge = slip(vecToEdge, -nor);
	vecToNextEdge = slip(vecToNextEdge, nor);
	vecToNextEdge = slip(vecToNextEdge, -nor);
}

// [4] サブゴール座標を求める
void PathFollowing::step4()
{
	subGoal = (pos1 + pos2) / 2;

	//D3DXVECTOR3 tempIntersection;

	////--------------------------------------------------------------------------
	//// 第二エッジへの最短ベクトルがポリゴンの外にはみ出す場合に、最短ベクトルと
	//// 近い側の第一エッジの頂点を、第一エッジと第二エッジの交点の代わりとする
	////--------------------------------------------------------------------------
	//if (nextEdgeIntersection == pos3 || nextEdgeIntersection == pos4)
	//{
	//	float len1 = between2VectorLength(pos1, nextEdgeIntersection);
	//	float len2 = between2VectorLength(pos2, nextEdgeIntersection);
	//	if (len1 < len2)
	//	{
	//		tempIntersection = pos1;
	//	}
	//	else
	//	{
	//		tempIntersection = pos2;
	//	}

	//	subGoal = (tempIntersection + edgeIntersection) / 2;
	//	return;	// ここで終了
	//}

	////-----------------------------------------------------------------------------
	//// ∠pos1,position,pos2の角度を求める→angleOutSide
	//// ∠edgeIntersection,position,nextEdgeIntersectionの角度を求める→angleInside
	//// 角度の比率から対辺の長さを求める
	////-----------------------------------------------------------------------------
	//float angleOutside, angleInside;
	//float percentage;
	//float oppositeLen;
	//formedRadianAngle(&angleOutside, pos1 - position, pos2 - position);
	//formedRadianAngle(&angleInside, vecToEdge, vecToNextEdge);
	//percentage = angleInside / angleOutside;
	//float tempLen = between2VectorLength(pos1, pos2);
	//if (isnan(percentage))
	//{
	//	oppositeLen = 0.0f;
	//}
	//else
	//{
	//	oppositeLen = tempLen * percentage;
	//}

	////------------------------------------------------------------------------------
	//// nextEdgeIntersectionがedgeIntersectionから見てpos1側にあるかpos2側にあるかを
	//// 判定するため平面の方程式を作る
	//// これは即ち、求めたいtempIntersectionがedgeIntersectionから見てpos1側にあるか
	//// pos2側にあるかを判定することになる
	////------------------------------------------------------------------------------
	//D3DXVECTOR3 cross;
	//D3DXVec3Cross(&cross, &nor, &(edgeIntersection - position));// ●このノーマルは二枚目で変わるかも
	//D3DXVec3Normalize(&cross, &cross);
	//// 平面の法線をいつでもpos1側に向かせる
	//D3DXVECTOR3 temp1 = edgeIntersection + cross;
	//D3DXVECTOR3 temp2 = edgeIntersection - cross;
	//if (D3DXVec3LengthSq(&(temp1 - pos1)) > D3DXVec3LengthSq(&(temp2 - pos1)))
	//{
	//	cross = -cross;
	//}
	//// 平面を求める
	//float d = -(cross.x * edgeIntersection.x
	//	+ cross.y * edgeIntersection.y
	//	+ cross.z * edgeIntersection.z);
	//float distance = cross.x * nextEdgeIntersection.x
	//	+ cross.y * nextEdgeIntersection.y
	//	+ cross.z * nextEdgeIntersection.z
	//	+ d;

	////----------------------------------------------------------------------
	//// tempIntersectionがpos1かpos2のどちらに近いかを参考にその座標を算出し
	//// サブゴールを求める
	////----------------------------------------------------------------------
	//D3DXVECTOR3 dir;
	//if (distance > 0)
	//{// tempIntersectionはpos1側
	//	between2VectorDirection(&dir, pos2, pos1);
	//}
	//else
	//{// tempIntersectionはpos2側
	//	between2VectorDirection(&dir, pos1, pos2);
	//}
	//tempIntersection = edgeIntersection + dir * oppositeLen;
	//subGoal = (tempIntersection + edgeIntersection) / 2;
}
#else
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
	if (cross == D3DXVECTOR3(0.0f, 0.0, 0.0f))
	{// position, pos1, pos2が同一直線上だった場合(超絶に低い可能性だが)外積が無効のため
		subGoal = position;	// このフレームでcreateVector()はベクトル(0, 0, 0)を返す
		return;
	}
	D3DXVec3Normalize(&cross, &cross);
	// 平面の法線をいつでもposition側に向かせる
	D3DXVECTOR3 temp1 = edgeIntersection + cross;
	D3DXVECTOR3 temp2 = edgeIntersection - cross;
	if (D3DXVec3LengthSq(&(temp1 - position)) > D3DXVec3LengthSq(&(temp2 - position)))
	{
		cross = -cross;
	}

	// (2)平面の方程式に代入して第一エッジまでの距離を求める
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
#endif