//=============================================================================
//
// アニメーションヘッダー [D3DXAnimation.h]
// Author : HAL東京 周子皓 
//
//=============================================================================

#ifndef _D3DXANIMATION_H_
#define _D3DXANIMATION_H_

#include "AllocateHierarchy.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// アニメーションセットデータ構造体
typedef struct
{
	LPCSTR				SetName;				// アニメーションセットの名前
	ID3DXAnimationSet	*AnimSet;				// アニメーションセット
	float				ShiftTime;              // シフトするのにかかる時間
	float				CurWeightTime;          // 現在のウェイト時間
	float				Damage;					// 与えるダメージ
}ANIMATIONDATA;

// アニメーション構造体
typedef struct D3DXANIMATION D3DXANIMATION;
struct D3DXANIMATION
{
	// メンバー変数
	LPD3DXANIMATIONCONTROLLER	AnimController;	// アニメーションコントローラー
	UINT						PreviousAnimID;	// 前再生していたアニメーションの番号
	UINT						CurrentAnimID;	// 現在再生しているアニメーションの番号
	UINT						NextAnimID;		// 次再生するアニメーションの番号
	ANIMATIONDATA				*AnimData;		// アニメーションセットのデータ
	CAllocateHierarchy			*AllocateHier;	// x fileの各情報を保存する
	LPD3DXFRAME					FrameRoot;		// ルートフレイム	
	int							AnimeSetNum;		// アニメーションセットの数
	const char					*Owner;			// 所有者の名前
	bool						MotionEnd;		// 今再生しているアニメーションは最後かどうか
	bool						StartMove;		// キャラの移動を停止かどうか
	bool						MotionBlendOver;// モーションブレンド終了
	bool						Cancelable;		// このアニメーションは中断できるかフラグ
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitAnimation(D3DXANIMATION* Animation, int Set_No);
// 終了処理
void UninitAnimation(D3DXANIMATION* Animation);
// 更新処理
void UpdateAnimation(D3DXANIMATION* Animation, float Time);
// 描画処理
void DrawAnimation(D3DXANIMATION* Animation, LPD3DXMATRIX WorldMatrix, bool DrawShadowMap);
// アニメーションを切り替える
void ChangeAnimation(D3DXANIMATION* Animation, UINT AnimID, float PlaySpeed, bool ResetMotion);
// モデルのx Fileを読み込む
HRESULT Load_xFile(D3DXANIMATION* D3DXAnimation, LPCTSTR filename, const char* ErrorSrc);
// 特定なボーンのマトリックスを取得
D3DXMATRIX GetBoneMatrix(D3DXANIMATION* Animation, const char* BoneName);

// メッシュを描画する
void DrawMeshContainer(D3DXANIMATION* Animation, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, bool DrawShadowMap);
// フレイム(ボーン)を描画する
void DrawFrame(D3DXANIMATION* Animation, LPD3DXFRAME pFrame, bool DrawShadowMap);
// ボーンマトリクス間の関係を設置する
HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot);
// フレイムのマトリクスを更新する
void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
// 特定なボーンのマトリックスを取得
D3DXFRAME_DERIVED* SearchBoneFrame(D3DXANIMATION* Animation, const char* BoneName, D3DXFRAME* Frame);



#endif
