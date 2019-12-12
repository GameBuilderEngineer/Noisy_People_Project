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
	enum ANIMATION_TYPE
	{
		DEFAULT,
		MOVE,
		ATTACK,
		SHOT,
		ANIMATION_TYPE_MAX
	};

	const DWORD ANIMATION_FLAG[ANIMATION_TYPE_MAX] =
	{
		0x00000001, // DEFAULT
		0x00000002,	// MOVE
		0x00000004,	// ATTACK
		0x00000008,	// SHOT
	};

	const float ANIMATION_SPEED[ANIMATION_TYPE_MAX] =
	{
		1.0f,		// DEFAULT
		1.0f,		// MOVE
		1.0f,		// ATTACK
		1.0f,		// SHOT
	};

	class DefaultAnimation : public PartsAnimation { public: void update(float t); };
	class MoveAnimation : public PartsAnimation { public: void update(float t); };
	class AttackAnimation : public PartsAnimation { public: void update(float t); };
	class ShotAnimation : public PartsAnimation { public: void update(float t); };
}


//=============================================================================
// アニメーションマネージャー
//=============================================================================
class TigerAnimationManager: public PartsAnimationManager
{
private:
	Object* parts[tigerAnimNS::ANIMATION_TYPE_MAX];
	tigerAnimNS::DefaultAnimation defaultAnimation;
	tigerAnimNS::MoveAnimation moveAnimation;
	tigerAnimNS::AttackAnimation attackAnimation;
	tigerAnimNS::ShotAnimation shotAnimation;

public:
	TigerAnimationManager(int partsMax, Object* parent, Object** parts);
	~TigerAnimationManager();
	void initialize();
	void update(float frameTime) override;
};
