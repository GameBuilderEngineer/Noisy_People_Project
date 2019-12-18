//-----------------------------------------------------------------------------
// TIGERアニメーション[TigerAnimation.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/12
//-----------------------------------------------------------------------------
#include "Tiger.h"
#include "TigerAnimation.h"
using namespace::tigerNS;
using namespace::tigerAnimNS;


#pragma region [Animation]
///////////////////////////////////////////////////////////////////////////////
////////////////////////////// アニメーション /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// 回転は製作の利便性を優先しグローバル変数として用意する
// .hと.cppの往復がなくなりコードも分かりやすくなるため

//=============================================================================
// デフォルトアニメーション
//=============================================================================
static const D3DXVECTOR3 DEFAULT_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_GUN[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

// コンストラクタ
DefaultAnimation::DefaultAnimation(DWORD _flag):PartsAnimation(_flag)
{
	numKeyFrame = sizeof(DEFAULT_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// 更新
void DefaultAnimation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(DEFAULT_ANIMATION_BODY);
	rot[GUN] = updateRotation(DEFAULT_ANIMATION_GUN);
	rot[LEG_L] = updateRotation(DEFAULT_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(DEFAULT_ANIMATION_LEG_R);
}


//=============================================================================
// 移動アニメーション
//=============================================================================
static const D3DXVECTOR3 MOVE_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.03f, 0.01f, -0.05f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.03f, -0.01f, 0.05f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

static const D3DXVECTOR3 MOVE_ANIMATION_GUN[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.01f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.01f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

static const D3DXVECTOR3 MOVE_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.6f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.45f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 MOVE_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.45f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.6f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

// コンストラクタ
MoveAnimation::MoveAnimation(DWORD _flag) :PartsAnimation(_flag)
{
	numKeyFrame = sizeof(MOVE_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// 更新
void MoveAnimation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(MOVE_ANIMATION_BODY);
	rot[GUN] = updateRotation(MOVE_ANIMATION_GUN);
	rot[LEG_L] = updateRotation(MOVE_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(MOVE_ANIMATION_LEG_R);
}


//=============================================================================
// 攻撃アニメーション
//=============================================================================
static const D3DXVECTOR3 ATTACK_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.02f, 0.0f, 0.0f),
	D3DXVECTOR3(0.02f, 0.0f, 0.0f),
	D3DXVECTOR3(0.02f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 ATTACK_ANIMATION_GUN[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 ATTACK_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 ATTACK_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

// コンストラクタ
AttackAnimation::AttackAnimation(DWORD _flag) :PartsAnimation(_flag)
{
	numKeyFrame = sizeof(ATTACK_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// 更新
void AttackAnimation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(ATTACK_ANIMATION_BODY);
	rot[GUN] = updateRotation(ATTACK_ANIMATION_GUN);
	rot[LEG_L] = updateRotation(ATTACK_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(ATTACK_ANIMATION_LEG_R);

}


//=============================================================================
// 死亡アニメーション
//=============================================================================
static const D3DXVECTOR3 DEAD_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.01f, 0.01f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.01f, -0.01f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_GUN[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.03f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, -0.03f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.05f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.05f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.05f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.05f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

// コンストラクタ
DeadAnimation::DeadAnimation(DWORD _flag) :PartsAnimation(_flag)
{
	numKeyFrame = sizeof(DEAD_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// 更新
void DeadAnimation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(DEAD_ANIMATION_BODY);
	rot[GUN] = updateRotation(DEAD_ANIMATION_GUN);
	rot[LEG_L] = updateRotation(DEAD_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(DEAD_ANIMATION_LEG_R);
}


//=============================================================================
// ショットアニメーション
//=============================================================================
static const D3DXVECTOR3 SHOT_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.1f, 0.0f, 0.0f),
	D3DXVECTOR3(0.1f, 0.0f, 0.0f),
	D3DXVECTOR3(0.1f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 SHOT_ANIMATION_GUN[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 SHOT_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 SHOT_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

// コンストラクタ
ShotAnimation::ShotAnimation(DWORD _flag) :PartsAnimation(_flag)
{
	numKeyFrame = sizeof(SHOT_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// 更新
void ShotAnimation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(SHOT_ANIMATION_BODY);
	rot[GUN] = updateRotation(SHOT_ANIMATION_GUN);
	rot[LEG_L] = updateRotation(SHOT_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(SHOT_ANIMATION_LEG_R);
}
#pragma endregion


#pragma region [Manager]
///////////////////////////////////////////////////////////////////////////////
/////////////////////////  アニメーションマネージャー /////////////////////////
///////////////////////////////////////////////////////////////////////////////
//=============================================================================
// コンストラクタ
//=============================================================================
TigerAnimationManager::TigerAnimationManager(int partsMax, Object* parent, Object** parts)
	: PartsAnimationManager(partsMax, parent, parts)
{
	animation = new PartsAnimation*[ANIMATION_TYPE_MAX];

	// アニメーションを作成
	animation[DEFAULT] = new DefaultAnimation(ANIMATION_FLAG[DEFAULT]);
	animation[MOVE] = new MoveAnimation(ANIMATION_FLAG[MOVE]);
	animation[ATTACK] = new AttackAnimation(ANIMATION_FLAG[ATTACK]);
	animation[DEAD] = new DeadAnimation(ANIMATION_FLAG[DEAD]);
	animation[SHOT] = new ShotAnimation(ANIMATION_FLAG[SHOT]);
}


//=============================================================================
// デストラクタ
//=============================================================================
TigerAnimationManager::~TigerAnimationManager()
{
	for (int i = 0; i < ANIMATION_TYPE_MAX; i++)
	{
		SAFE_DELETE(animation[i]);
	}
	SAFE_DELETE_ARRAY(animation);
}


//=============================================================================
// 更新処理
//=============================================================================
void TigerAnimationManager::update(float frameTime)
{
	// 回転の更新
	for (int i = 0; i < ANIMATION_TYPE_MAX; i++)
	{
		if (flagState & animation[i]->flag)
		{
			animation[i]->update(rot, parts, frameTime * ANIMATION_SPEED[i]);
		}
	}

	// 行列計算
	culcPartsMatrix();
}


//=============================================================================
// パーツのマトリクスを計算する
// Objectクラスのupdate()ではクオータニオンベースでの回転計算を行っているため,
// 回転処理に干渉が難しい. update()の代用としてこの関数でワールド変換を行う.
// また親オブジェクトや親パーツのワールド変換を子パーツに反映させている.
//=============================================================================
void TigerAnimationManager::culcPartsMatrix()
{
	for (int i = 0; i < partsMax; i++)
	{
		// パーツのワールドマトリクスを初期化する
		D3DXMatrixIdentity(&parts[i]->matrixWorld);
		// 回転ベクトルから回転マトリクスを作成する
		D3DXMatrixRotationYawPitchRoll(&parts[i]->matrixRotation, rot[i].y, rot[i].x, rot[i].z);
		// 位置から位置マトリクスを作成する
		D3DXMatrixTranslation(&parts[i]->matrixPosition, parts[i]->position.x, parts[i]->position.y, parts[i]->position.z);
		// 回転マトリクスをワールドマトリクスに掛ける
		D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[i]->matrixRotation);
		// 位置マトリクスをワールドマトリクスに掛ける
		D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[i]->matrixPosition);

		if (i == BODY)
		{
			// パーツのワールドマトリクスに親オブジェクトのワールドマトリクスを掛ける
			D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parent->matrixWorld);
		}
		else
		{
			// 子パーツのワールドマトリクスに親パーツのマトリクスを掛ける
			D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[0]->matrixWorld);
		}
	}
}


//=============================================================================
// デフォルトアニメーションに切り替え
//=============================================================================
void TigerAnimationManager::switchDefault()
{
	// 攻撃アニメーションが終了していないと再生されない
	if (animation[ATTACK]->wasPlayedToEnd)
	{
		inactivate(animation[ATTACK]);
		activate(animation[DEFAULT]);
	}
}


//=============================================================================
// 移動アニメーションに切り替え
//=============================================================================
void TigerAnimationManager::switchMove()
{
	// 攻撃アニメーションが終了していないと再生されない
	if (animation[ATTACK]->wasPlayedToEnd)
	{
		inactivate(animation[ATTACK]);
		activate(animation[MOVE]);
	}
}



//=============================================================================
// 攻撃アニメーションに切り替え
//=============================================================================
void TigerAnimationManager::switchAttack()
{
	inactivateAll();
	activate(animation[ATTACK]);
}


//=============================================================================
// 死亡アニメーションに切り替え
//=============================================================================
void TigerAnimationManager::switchDead()
{
	inactivateAll();
	activate(animation[DEAD]);
}
#pragma endregion