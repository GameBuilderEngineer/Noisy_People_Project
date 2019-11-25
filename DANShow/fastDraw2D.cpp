//=============================================================================
// < SMASH STARS >
// 高速描画処理[2D] [fastDraw2D.cpp]
// Author : 蔡 友剛
//
//=============================================================================
#include "main.h"
#include "basic.h"
#include "fastDraw2D.h"

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void LoadTexture(LPDIRECT3DDEVICE9 device, TEXTURE *object, const char *path)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(device,	// デバイスへのポインタ
	path,								// ファイルの名前
	&object->texture);					// 読み込むメモリー
}

//=============================================================================
// 描画処理(一般的な描画処理)
//=============================================================================
void DrawNormalObject(LPDIRECT3DDEVICE9 device, TEXTURE object)
{
	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device->SetTexture(0, object.texture);

	// ポリゴンの描画
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &object.vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// 描画処理(移動用・中心座標)
//=============================================================================
void DrawMoveCenterObject(LPDIRECT3DDEVICE9 device, TEXTURE object)
{
	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device->SetTexture(0, object.texture);

	// 頂点座標の設定(中心座標)
	SetVtxCenterData(&object);

	// ポリゴンの描画
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &object.vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// 描画処理(回転用・中心座標)
//=============================================================================
void DrawRotCenterObject(LPDIRECT3DDEVICE9 device, TEXTURE object, float radian, float Length)
{
	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device->SetTexture(0, object.texture);

	// 頂点座標の設定(中心座標)
	SetVtxRotData(&object, radian, Length);

	// ポリゴンの描画
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &object.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// 描画処理(数字スクロール用)
//=============================================================================
void DrawNumDataObject(LPDIRECT3DDEVICE9 device, TEXTURE object, int no)
{
	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device->SetTexture(0, object.texture);

	// テクスチャ座標の設定(数字スクロール用)
	SetTextureNumData(&object, no);

	// ポリゴンの描画
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &object.vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// 描画処理(アニメーション用)
//=============================================================================
void DrawAnimationObject(LPDIRECT3DDEVICE9 device, TEXTURE object,
	int cntPattern, int PatternX, int PatternY)
{
	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device->SetTexture(0, object.texture);

	// 頂点座標の設定(中心座標)
	SetVtxCenterData(&object);

	// テクスチャ座標の設定(数字スクロール用)
	SetTextureAnimationData(&object, cntPattern, PatternX, PatternY);

	// ポリゴンの描画
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &object.vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// 描画処理(色の変更・中心座標)
//=============================================================================
void DrawColorCenterObject(LPDIRECT3DDEVICE9 device, TEXTURE object)
{
	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device->SetTexture(0, object.texture);

	// 頂点座標の設定(中心座標)
	SetVtxCenterData(&object);

	// 色の変更
	SetAllDiffuseData(&object);

	// ポリゴンの描画
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &object.vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// 描画処理(ゲージ用・色の変更)
//=============================================================================
void DrawGaugeObject(LPDIRECT3DDEVICE9 device, TEXTURE object, float length, int dir)
{
	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device->SetTexture(0, object.texture);

	// 色の変更
	SetAllDiffuseData(&object);

	// 頂点座標の設定(中心座標)
	SetVtx4PointGaugeDate(&object, length, dir);

	// ポリゴンの描画
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &object.vertexWk, sizeof(VERTEX_2D));
}
