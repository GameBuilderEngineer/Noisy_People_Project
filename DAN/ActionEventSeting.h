//=============================================================================
//
// アニメーションヘッダー [D3DXAnimation.h]
// Author : HAL東京 周子皓 
//
//=============================================================================
#ifndef _ACTIONEVENTSETING_H_
#define _ACTIONEVENTSETING_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Keydata_Max (50)
// デストラクタ
#define SAFE_FREE(object)			{if(object){free (object);			(object) = NULL;}}

#include "D3DXAnimation.h"

enum CallbackKeyType
{
	NoEvent = 0,
	MovePAttackStart,
	MovePAttackEnd,
	MovePJumpFireStart,
	MovePJumpFireEnd,
	MovePRollStart,
	MovePRollEnd,
	MovePDeath,
	MovePIsDamage,
	MovePIsDamageEnd,
	MoveP1AttackStart,
	MoveP1AttackEnd,
	MoveP1JumpFireStart,
	MoveP1JumpFireEnd,
	MoveP1RollStart,
	MoveP1RollEnd,
	MoveP1Death,
	MoveP1IsDamage,
	MoveP1IsDamageEnd,
	MovePMoveVoice,
	MoveP1MoveVoice,
	ResetTime,				// アニメーションのローカル時間をリセット 
	MotionEnd,				// モーション終了
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	float	Time;			// Keyframeの時間
	int		KeyType;		// Keyframeの種類
}KEYDATA;


//*****************************************************************************
// グローバル変数
//*****************************************************************************
typedef struct
{
	int				Keydata_No = 0;
	KEYDATA			Keydata[Keydata_Max];
}ACTIONEVENTSETING;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// アニメーション中断イベントのキーを設置する
HRESULT SetupCallbackKeys(KEYDATA *Keydata, LPCSTR SetName, int CallbacksNum, const char *Owner);
// キーデータを設置する
void AddKeydata(float Time, int KeyType);

ACTIONEVENTSETING *GetAESPAdr(void);

// アニメーションコールバック構造体
struct AnimCallBackHandler : public ID3DXAnimationCallbackHandler
{
	// メンバー変数
	D3DXANIMATION		*AnimPointer;

	// メンバー関数
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
};



#endif
