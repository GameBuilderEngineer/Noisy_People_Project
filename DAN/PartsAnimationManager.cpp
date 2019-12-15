//-----------------------------------------------------------------------------
// パーツアニメーションマネージャ [PartsAnimationManager.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/12
//-----------------------------------------------------------------------------
#include "PartsAnimationManager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
PartsAnimationManager::PartsAnimationManager(int _partsMax, Object* _parent, Object** _parts)
{
	partsMax = _partsMax;
	parent = _parent;

	// パーツ数の配列を確保
	parts = new Object*[partsMax];
	for (int i = 0; i < partsMax; i++)
	{
		parts[i] = _parts[i];
	}
	rot = new D3DXVECTOR3[partsMax];
	for (int i = 0; i < partsMax; i++)
	{
		rot[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// フラグを初期化
	flagState = 0;
}


//=============================================================================
// デストラクタ
//=============================================================================
PartsAnimationManager::~PartsAnimationManager()
{
	SAFE_DELETE_ARRAY(parts);
	SAFE_DELETE_ARRAY(rot);
}


//=============================================================================
// アニメーション起動
//=============================================================================
void PartsAnimationManager::activate(PartsAnimation* animation)
{
	flagState |= animation->flag;
	animation->wasPlayedToEnd = false;
}


//=============================================================================
// アニメーション停止
//=============================================================================
void PartsAnimationManager::inactivate(PartsAnimation* animation)
{
	flagState &= ~animation->flag;
	animation->wasPlayedToEnd = true;
}


//=============================================================================
// アニメーション全停止
//=============================================================================
void PartsAnimationManager::inactivateAll()
{
	flagState = 0;
	for (int i = 0; i < animatinMax; i++)
	{
		animation[i]->wasPlayedToEnd = true;
	}
}


PartsAnimation* PartsAnimationManager::getAnimation(int type)
{
	return animation[type];
}