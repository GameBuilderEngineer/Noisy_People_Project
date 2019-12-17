#pragma once

#include "movep.h"
#include "D3DXAnimation.h"
#include "Input.h"
#include "Direct3D9.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 読み込むモデル名
#define	Model_MovePSword	"07 Model File/Gun.X"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 剣構造体
typedef struct
{
	D3DXVECTOR3			Pos;				// モデルの位置
	D3DXVECTOR3			Rot;				// モデルの向き(回転)
	D3DXVECTOR3			Scale;				// モデルの大きさ(スケール)
	LPDIRECT3DTEXTURE9	*Texture;			// テクスチャへのポインタ
	LPD3DXMESH			Mesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER		MaterialBuffer;		// マテリアル情報へのポインタ
	DWORD				MaterialNum;		// マテリアル情報の数
	D3DXMATRIX			WorldMatrix;		// ワールドマトリックス
	int					EffectID;			// エフェクト番号
	D3DXQUATERNION		Quaternion;			// 回転

}SWORD;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitEquipment(bool FirstInit);
// 終了処理
void UninitEquipment(void);
// 更新処理
void UpdateEquipment(void);
// 描画処理
void DrawEquipment(void);
// 剣のポインタを取得する
SWORD *GetSword(const char* Owner);

HRESULT SafeLoadModelTexture(LPDIRECT3DTEXTURE9	*Texture, LPD3DXBUFFER MaterialBufferconst, DWORD MaterialNum, const char* ErrorSrc);

