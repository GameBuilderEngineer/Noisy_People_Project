//=============================================================================
//
// キャラクタアニメ処理 [movep.h]
// Author : HAL東京 周子皓
//
//=============================================================================
#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "AllocateHierarchy.h"										
#include <vector>
#include "D3DXAnimation.h"


#define	MOVEP1_MODEL		"07 Model File/FeMale.X"

#define TimePerFrame		(0.0167f)				// 1 / 60秒

enum MoveP1Action
{
	MoveP1_Idle,
	MoveP1_Run,
	MoveP1_FireIdle,
	MoveP1_FireRun,
	MoveP1_JumpFire,
	MoveP1_Die,
	MoveP1_WalkRight,
	MoveP1_WalkLeft,
	MoveP1_WalkBackwards,
	MoveP1_Max
};



typedef struct
{
	CAllocateHierarchy*		pAllocateHier = NULL;

	D3DXMATRIX			WorldMatrix;		// ワールドマトリックス
	D3DXVECTOR3			Pos;				// 座標
	D3DXVECTOR3			Scl;				// 大きさ(スケール)
	D3DXVECTOR3			Rot;				// 回転
	D3DXQUATERNION		Quaternion;				// 回転
	D3DXANIMATION		*Animation;			// アニメーション
	float				ActionSpeed;		// 全体アクションの速度
	float				fTimeDelta = 0.0f;
	D3DXMATRIX			RHand;
	D3DXVECTOR3			RHandPos;
	D3DXVECTOR3			PrePos;				// 1フレイム前の座標
	D3DXVECTOR3			CenterPos;			// プレイヤーの中心座標
	D3DXVECTOR3			NextPos;			// 次の座標
	D3DXVECTOR3			Move;				// 移動量
	D3DXVECTOR3			DestAngle;			// 向き
	float				MoveY;				// 目標角度
	float				GroundDistance;		// 地面との距離
	float				facing;				// 目標角度
	float				facingTarget;		// 次のアニメーションの目標角度
	int					HP;					// プレイヤーのHP
	float				RunSpeedPl;
	float				WalkSpeedPl;

	//判断用変数
	bool				IsRun = false;
	bool				IsIdle = true;
	bool				IsWalk = false;
	bool				IsRoll = false;
	bool				IsJump = false;
	bool				IsFireJump = false;
	bool				IsJumpEnd = false;
	bool				IsGround = false;
	bool				IsDefend = false;
	bool				AnimeChange = true;
	bool				IsChange = false;
	bool				IsChangeRun = false;


	//死亡の動作判断
	bool				IsDie = false;
	//死亡した後判断
	bool				IsDeath = false;
	//攻撃三段の判断
	bool				IsAttack = false;
	bool				IsAttack1 = false;
	bool				IsAttack2 = false;
	//攻撃三段の移動判断
	bool				AttackMove = false;
	bool				AttackMove1 = false;
	bool				AttackMove2 = false;
	//攻撃三段の攻撃判断
	bool				IsDamage1 = false;
	bool				flag1 = false;
	bool				IsDamage2 = false;
	bool				flag2 = false;
	bool				IsDamage3 = false;
	bool				flag3 = false;
	bool				IsDefendDamage = false;
	bool				IsNoDefendDamage = false;

	//現在使用していない
	bool				IsLock = false;

}MOVEP1;


HRESULT InitMoveP1(D3DXVECTOR3 Rot, D3DXVECTOR3 Scl, bool FirstInit);
void UpdateMoveP1(float f_TimeDelta);
void DrawMoveP1();
void UninitMoveP1(void);

void MoveP1Move(void);
void MoveP1AnimeCur(void);
void MoveP1AnimeNext(void);


MOVEP1 *GetMoveP1Adr(void);
// アニメーション中断イベントのKeyframesを設置する
HRESULT InitCallbackKeys_MoveP1(void);

