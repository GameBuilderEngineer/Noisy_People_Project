//-----------------------------------------------------------------------------
// 頂点アクセスクラス [VertexAccessor.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/13
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshLoader.h"

namespace vtxAccess
{
	// アクセス要素
	enum VERTEX_ELEMENT
	{
		POSITION,
		NORMAL,
		COLOR,
		UV,
		// バンプマップとかする場合は
		// ここを増やす！
		ELEMENT_MAX
	};
}

// 頂点アクセスクラス
struct VertexAccessor
{
	WORD offset[vtxAccess::ELEMENT_MAX];		// オフセットサイズ配列

	// 頂点から取得したい情報へのポインタを返す
	void* getPointer(int target, BYTE* begin);

	// 頂点情報を解析して頂点アクセスクラスを返す
	void initialize(StaticMesh* staticMesh);
};

