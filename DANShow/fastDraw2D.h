//=============================================================================
//
// 高速描画処理[2D] [fastDraw2D.cpp]
// Author : 蔡 友剛
//
//=============================================================================
#ifndef _FAST_DRAW_2D_H
#define _FAST_DRAW_2D_H

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// テクスチャの読み込み
void LoadTexture
(LPDIRECT3DDEVICE9 device, TEXTURE *object, const char *path);

// 描画処理(一般的な描画処理)
void DrawNormalObject							
(LPDIRECT3DDEVICE9 device, TEXTURE object);

// 描画処理(移動用・中心座標)
void DrawMoveCenterObject
(LPDIRECT3DDEVICE9 device, TEXTURE object);

// 描画処理(回転用・中心座標)
void DrawRotCenterObject
(LPDIRECT3DDEVICE9 device, TEXTURE object, float angle, float Length);

// 描画処理(数字スクロール用)
void DrawNumDataObject
(LPDIRECT3DDEVICE9 device, TEXTURE object, int no);

// 描画処理(アニメーション用)
void DrawAnimationObject
(LPDIRECT3DDEVICE9 device, TEXTURE object, int cntPattern,
	int PatternX, int PatternY);

// 描画処理(色の変更・中心座標)
void DrawColorCenterObject
(LPDIRECT3DDEVICE9 device, TEXTURE object);

// 描画処理(ゲージ・色の変更)
void DrawGaugeObject
(LPDIRECT3DDEVICE9 device, TEXTURE object, float length, int dir);

#endif 
