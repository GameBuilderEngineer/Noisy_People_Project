//-----------------------------------------------------------------------------
// TIGERアニメーション[TigerAnimation.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/12
//-----------------------------------------------------------------------------
#pragma once
#include "PartsAnimation.h"
#include "PartsAnimationManager.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace tigerAnimNS
{
	// アニメーションの種類
	enum ANIMATION_TYPE
	{
		DEFAULT,
		MOVE,
		ATTACK,
		DEAD,
		SHOT,
		ANIMATION_TYPE_MAX
	};

	// アニメーションのビット演算フラグ
	const DWORD ANIMATION_FLAG[ANIMATION_TYPE_MAX] =
	{
		0x00000001, // DEFAULT
		0x00000002,	// MOVE
		0x00000004,	// ATTACK
		0x00000008,	// DEAD
		0x00000010,	// SHOT
	};

	// アニメーションのスピード
	// キーフレームが「1進むのに1秒」かかるスピードに対しての倍率. ex)3.0f →　「1進むのに0.33秒」
	// また1つのアニメーション全体の長さは(キーフレームの数 - 1) * (1.0f / この倍率)で求められる.
	const float ANIMATION_SPEED[ANIMATION_TYPE_MAX] =
	{
		1.0f,		// DEFAULT
		4.0f,		// MOVE
		5.0f,		// ATTACK
		20.0f,		// DEAD
		1.0f,		// SHOT
	};

	//----------------------
	// アニメーションクラス
	//----------------------
	// デフォルト
	class DefaultAnimation : public PartsAnimation
	{
	public:
		DefaultAnimation(DWORD _flag);
		void update(D3DXVECTOR3* rot, Object** parts, float t);
	};

	// 移動
	class MoveAnimation : public PartsAnimation
	{
	public:
		MoveAnimation(DWORD _flag);
		void update(D3DXVECTOR3* rot, Object** parts, float t);
	};

	// 攻撃
	class AttackAnimation : public PartsAnimation
	{
	public:
		AttackAnimation(DWORD _flag);
		void update(D3DXVECTOR3* rot, Object** parts, float t);
	};

	// 死亡
	class DeadAnimation : public PartsAnimation
	{
	public:
		DeadAnimation(DWORD _flag);
		void update(D3DXVECTOR3* rot, Object** parts, float t);
	};

	// ショット
	class ShotAnimation : public PartsAnimation
	{
	public:
		ShotAnimation(DWORD _flag);
		void update(D3DXVECTOR3* rot, Object** parts, float t);
	};
}


//=============================================================================
// アニメーションマネージャークラス
//=============================================================================
class TigerAnimationManager: public PartsAnimationManager
{
public:
	// コンストラクタ
	TigerAnimationManager(int partsMax, Object* parent, Object** parts);
	// デストラクタ
	~TigerAnimationManager();
	// 更新
	void update(float frameTime) override;
	// パーツのマトリクスを計算する
	void culcPartsMatrix() override;
	// デフォルトアニメーションに切り替え
	void switchDefault() override;
	// 移動アニメーションに切り替え
	void switchMove() override;
	// 攻撃アニメーションに切り替え
	void switchAttack() override;
	// 死亡アニメーションに切り替え
	void switchDead() override;
};
