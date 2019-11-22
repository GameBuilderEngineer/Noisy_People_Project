//=============================================================================
//
// 頂点処理 [basic.cpp]
// Author : 蔡 友剛
//
//=============================================================================
#include "basic.h"

//=============================================================================
// 頂点フォーマット(頂点情報)
// 引数:頂点情報格納ワーク	座標X,Y,Z	サイズX,Y
//=============================================================================
HRESULT MakeVtx4PointData(TEXTURE *object)
{
	// 頂点座標の設定
	object->vertexWk[0].vtx = D3DXVECTOR3(object->pos.x, object->pos.y, object->pos.z);
	object->vertexWk[1].vtx = D3DXVECTOR3(object->pos.x + object->size.x, object->pos.y, object->pos.z);
	object->vertexWk[2].vtx = D3DXVECTOR3(object->pos.x, object->pos.y + object->size.y, object->pos.z);
	object->vertexWk[3].vtx = D3DXVECTOR3(object->pos.x + object->size.x, object->pos.y + object->size.y, object->pos.z);

	// rhwの設定
	object->vertexWk[0].rhw =
		object->vertexWk[1].rhw =
		object->vertexWk[2].rhw =
		object->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	object->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	object->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	object->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	object->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	object->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	object->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	object->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	object->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 頂点フォーマット(中心座標)
//=============================================================================
HRESULT MakeVtxCenterData(TEXTURE *object)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	object->vertexWk[0].vtx.x = object->pos.x - object->size.x;
	object->vertexWk[0].vtx.y = object->pos.y - object->size.y;
	object->vertexWk[0].vtx.z = 0.0f;

	object->vertexWk[1].vtx.x = object->pos.x + object->size.x;
	object->vertexWk[1].vtx.y = object->pos.y - object->size.y;
	object->vertexWk[1].vtx.z = 0.0f;

	object->vertexWk[2].vtx.x = object->pos.x - object->size.x;
	object->vertexWk[2].vtx.y = object->pos.y + object->size.y;
	object->vertexWk[2].vtx.z = 0.0f;

	object->vertexWk[3].vtx.x = object->pos.x + object->size.x;
	object->vertexWk[3].vtx.y = object->pos.y + object->size.y;
	object->vertexWk[3].vtx.z = 0.0f;

	// rhwの設定
	object->vertexWk[0].rhw =
	object->vertexWk[1].rhw =
	object->vertexWk[2].rhw =
	object->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	object->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	object->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	object->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	object->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	object->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	object->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	object->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	object->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 頂点フォーマット(分割テクスチャ頂点情報)
// 引数:頂点情報格納ワーク	座標X,Y,Z	サイズX,Y
//=============================================================================
HRESULT MakeCutVtx4PointData(TEXTURE *object, int PatternX, int PatternY, int cntPattern)
{
	// 頂点座標の設定
	object->vertexWk[0].vtx = D3DXVECTOR3(object->pos.x, object->pos.y, object->pos.z);
	object->vertexWk[1].vtx = D3DXVECTOR3(object->pos.x + object->size.x, object->pos.y, object->pos.z);
	object->vertexWk[2].vtx = D3DXVECTOR3(object->pos.x, object->pos.y + object->size.y, object->pos.z);
	object->vertexWk[3].vtx = D3DXVECTOR3(object->pos.x + object->size.x, object->pos.y + object->size.y, object->pos.z);

	// rhwの設定
	object->vertexWk[0].rhw =
		object->vertexWk[1].rhw =
		object->vertexWk[2].rhw =
		object->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	object->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	object->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	object->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	object->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	int x = cntPattern % PatternX;
	int y = cntPattern / PatternX;
	float sizeX = 1.0f / PatternX;
	float sizeY = 1.0f / PatternY;

	object->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	object->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	object->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	object->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定(頂点情報)
// 引数:頂点情報格納ワーク	座標X,Y,Z	サイズX,Y
//=============================================================================
void SetVtx4PointData(TEXTURE *object)
{
	// 頂点座標の設定
	object->vertexWk[0].vtx.x = object->pos.x;
	object->vertexWk[0].vtx.y = object->pos.y;
	object->vertexWk[0].vtx.z = 0.0f;

	object->vertexWk[1].vtx.x = object->pos.x + object->size.x;
	object->vertexWk[1].vtx.y = object->pos.y;
	object->vertexWk[1].vtx.z = 0.0f;

	object->vertexWk[2].vtx.x = object->pos.x;
	object->vertexWk[2].vtx.y = object->pos.y + object->size.y;
	object->vertexWk[2].vtx.z = 0.0f;

	object->vertexWk[3].vtx.x = object->pos.x + object->size.x;
	object->vertexWk[3].vtx.y = object->pos.y + object->size.y;
	object->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 頂点座標の設定(中心座標)
//=============================================================================
void SetVtxCenterData(TEXTURE *object)
{
	// 頂点座標の設定
	object->vertexWk[0].vtx.x = object->pos.x - object->size.x;
	object->vertexWk[0].vtx.y = object->pos.y - object->size.y;
	object->vertexWk[0].vtx.z = 0.0f;

	object->vertexWk[1].vtx.x = object->pos.x + object->size.x;
	object->vertexWk[1].vtx.y = object->pos.y - object->size.y;
	object->vertexWk[1].vtx.z = 0.0f;

	object->vertexWk[2].vtx.x = object->pos.x - object->size.x;
	object->vertexWk[2].vtx.y = object->pos.y + object->size.y;
	object->vertexWk[2].vtx.z = 0.0f;

	object->vertexWk[3].vtx.x = object->pos.x + object->size.x;
	object->vertexWk[3].vtx.y = object->pos.y + object->size.y;
	object->vertexWk[3].vtx.z = 0.0f;
}


//=============================================================================
// 頂点座標の設定(回転)
//=============================================================================
void SetVtxRotData(TEXTURE *object, float angle, float Length)
{
	// 頂点座標の設定
	object->vertexWk[0].vtx.x = object->pos.x - cosf(D3DXToDegree(angle) + object->rot.z) * Length;
	object->vertexWk[0].vtx.y = object->pos.y - sinf(D3DXToDegree(angle) + object->rot.z) * Length;
	object->vertexWk[0].vtx.z = 0.0f;

	object->vertexWk[1].vtx.x = object->pos.x + cosf(D3DXToDegree(angle) - object->rot.z) * Length;
	object->vertexWk[1].vtx.y = object->pos.y - sinf(D3DXToDegree(angle) - object->rot.z) * Length;
	object->vertexWk[1].vtx.z = 0.0f;

	object->vertexWk[2].vtx.x = object->pos.x - cosf(D3DXToDegree(angle) - object->rot.z) * Length;
	object->vertexWk[2].vtx.y = object->pos.y + sinf(D3DXToDegree(angle) - object->rot.z) * Length;
	object->vertexWk[2].vtx.z = 0.0f;

	object->vertexWk[3].vtx.x = object->pos.x + cosf(D3DXToDegree(angle) + object->rot.z) * Length;
	object->vertexWk[3].vtx.y = object->pos.y + sinf(D3DXToDegree(angle) + object->rot.z) * Length;
	object->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 頂点座標の設定(ゲージ用)
//=============================================================================
void SetVtx4PointGaugeDate(TEXTURE *object, float Length, int dir)
{
	// 上向き
	if (dir == GAUGE_UP)
	{
		// 頂点座標の設定
		object->vertexWk[0].vtx = D3DXVECTOR3(object->pos.x, object->pos.y + Length, object->pos.z);
		object->vertexWk[1].vtx = D3DXVECTOR3(object->pos.x + object->size.x, object->pos.y + Length, object->pos.z);
		object->vertexWk[2].vtx = D3DXVECTOR3(object->pos.x, object->pos.y + object->size.y, object->pos.z);
		object->vertexWk[3].vtx = D3DXVECTOR3(object->pos.x + object->size.x, object->pos.y + object->size.y, object->pos.z);
	}

	// 右向き
	else if (dir == GAUGE_RIGHT)
	{
		// 頂点座標の設定
		object->vertexWk[0].vtx = D3DXVECTOR3(object->pos.x, object->pos.y, object->pos.z);
		object->vertexWk[1].vtx = D3DXVECTOR3(object->pos.x + (object->size.x * Length), object->pos.y, object->pos.z);
		object->vertexWk[2].vtx = D3DXVECTOR3(object->pos.x, object->pos.y + object->size.y, object->pos.z);
		object->vertexWk[3].vtx = D3DXVECTOR3(object->pos.x + (object->size.x * Length), object->pos.y + object->size.y, object->pos.z);
	}
}

//=============================================================================
// テクスチャ座標の設定(数字スクロール用)
//=============================================================================
void SetTextureNumData(TEXTURE *object, int number)
{
	// テクスチャ座標の設定
	object->vertexWk[0].tex = D3DXVECTOR2(0.1f * number, 0.0f);
	object->vertexWk[1].tex = D3DXVECTOR2(0.1f * (number + 1), 0.0f);
	object->vertexWk[2].tex = D3DXVECTOR2(0.1f * number, 1.0f);
	object->vertexWk[3].tex = D3DXVECTOR2(0.1f * (number + 1), 1.0f);
}

//=============================================================================
// テクスチャ座標の設定(背景スクロール用)
//=============================================================================
void SetTextureScrollData(TEXTURE *object, float distance)
{
	// テクスチャ座標の設定
	object->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f + distance);
	object->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f + distance);
	object->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f + distance);
	object->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f + distance);
}

//=============================================================================
// テクスチャ座標の設定(アニメーション用)
//=============================================================================
void SetTextureAnimationData(TEXTURE *object, int cntPattern, int PatternX, int PatternY)
{
	// テクスチャ座標の設定
	int x = cntPattern % PatternX;
	int y = cntPattern / PatternX;
	float sizeX = 1.0f / PatternX;
	float sizeY = 1.0f / PatternY;

	object->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	object->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	object->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	object->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 反射光の設定(一頂点ずつ)
//=============================================================================
void SetSingleDiffuseData(TEXTURE *object, int no)
{
	// 反射光の設定
	object->vertexWk[no].diffuse = object->col;
}

//=============================================================================
// 反射光の設定(全頂点)
//=============================================================================
void SetAllDiffuseData(TEXTURE *object)
{
	// 反射光の設定
	object->vertexWk[0].diffuse = object->col;
	object->vertexWk[1].diffuse = object->col;
	object->vertexWk[2].diffuse = object->col;
	object->vertexWk[3].diffuse = object->col;
}
