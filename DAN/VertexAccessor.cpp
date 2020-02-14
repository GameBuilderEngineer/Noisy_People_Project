//-----------------------------------------------------------------------------
// 頂点アクセスクラス [VertexAccessor.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/13
//-----------------------------------------------------------------------------
#include "VertexAccessor.h"
using namespace vtxAccess;

//=============================================================================
// 頂点情報を解析
//=============================================================================
void VertexAccessor::initialize(StaticMesh* staticMesh)
{
	ZeroMemory(offset, sizeof(WORD) * ELEMENT_MAX);
	BYTE sumOffset = 0;
	int i = 0;

	// 参照対象ごとに頂点アドレスからのオフセット値を計算する
	while (staticMesh->vertexElement[i].Type != MAXD3DDECLTYPE)
	//while (i < 65)
	{
		switch (staticMesh->vertexElement[i].Usage)
		{
		case D3DDECLUSAGE_POSITION:
			offset[VERTEX_ELEMENT::POSITION] = sumOffset;
			break;

		case D3DDECLUSAGE_NORMAL:
			offset[VERTEX_ELEMENT::NORMAL] = sumOffset;
			break;

		case D3DDECLUSAGE_COLOR:
			offset[VERTEX_ELEMENT::COLOR] = sumOffset;
			break;

		case D3DDECLUSAGE_TEXCOORD:
			offset[VERTEX_ELEMENT::UV] = sumOffset;
			break;

		case D3DDECLUSAGE_BINORMAL:
			offset[VERTEX_ELEMENT::BINORMAL] = sumOffset;
			break;

		default:
			break;
		}

		switch (staticMesh->vertexElement[i].Type)
		{
		case D3DDECLTYPE_FLOAT1:
			sumOffset += sizeof(float);
			break;

		case D3DDECLTYPE_FLOAT2:
			sumOffset += sizeof(float) * 2;
			break;

		case D3DDECLTYPE_FLOAT3:
			sumOffset += sizeof(float) * 3;
			break;

		case D3DDECLTYPE_FLOAT4:
			sumOffset += sizeof(float) * 4;
			break;

		case D3DDECLTYPE_D3DCOLOR:
			sumOffset += sizeof(D3DCOLOR);
			break;

		case D3DDECLTYPE_UNUSED:// 未使用
			sumOffset += 0;
			break;

		default:
			sumOffset += 0;
			break;
		}

		i++;
	}
}


//=============================================================================
// 頂点から取得したい情報へのポインタを返す
//=============================================================================
void* VertexAccessor::getPointer(int target, BYTE* begin)
{
	if (target < VERTEX_ELEMENT::ELEMENT_MAX)
	{
		return (void*)(begin + offset[target]);
	}
	else
	{
		return NULL;
	}
}

