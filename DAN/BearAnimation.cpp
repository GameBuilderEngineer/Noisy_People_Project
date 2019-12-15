//-----------------------------------------------------------------------------
// BEARアニメーション[BearAnimation.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/15
//-----------------------------------------------------------------------------
#include "Bear.h"
#include "BearAnimation.h"
using namespace::bearNS;
using namespace::bearAnimNS;


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
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_ARM_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_ARM_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_WAIST[] =
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
	rot[ARM_L] = updateRotation(DEFAULT_ANIMATION_ARM_L);
	rot[ARM_R] = updateRotation(DEFAULT_ANIMATION_ARM_R);
	rot[WAIST] = updateRotation(DEFAULT_ANIMATION_WAIST);
	rot[LEG_L] = updateRotation(DEFAULT_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(DEFAULT_ANIMATION_LEG_R);
}


//=============================================================================
// 移動アニメーション
//=============================================================================
static const D3DXVECTOR3 MOVE_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, -0.01f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, 0.01f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 MOVE_ANIMATION_ARM_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
	D3DXVECTOR3(0.0f, 0.2f, -0.5f),
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
	D3DXVECTOR3(0.0f, -0.2f, -0.5f),
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
};
static const D3DXVECTOR3 MOVE_ANIMATION_ARM_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
	D3DXVECTOR3(0.0f, 0.2f, 0.5f),
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
	D3DXVECTOR3(0.0f, -0.2f, 0.5f),
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
};
static const D3DXVECTOR3 MOVE_ANIMATION_WAIST[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),

};
static const D3DXVECTOR3 MOVE_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 MOVE_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.5f, 0.0f, 0.0f),
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
	rot[ARM_L] = updateRotation(MOVE_ANIMATION_ARM_L);
	rot[ARM_R] = updateRotation(MOVE_ANIMATION_ARM_R);
	rot[WAIST] = updateRotation(MOVE_ANIMATION_WAIST);
	rot[LEG_L] = updateRotation(MOVE_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(MOVE_ANIMATION_LEG_R);
}


//=============================================================================
// 攻撃アニメーション
//=============================================================================
static const D3DXVECTOR3 ATTACK1_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, -0.01f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, 0.01f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 ATTACK1_ANIMATION_ARM_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
};
static const D3DXVECTOR3 ATTACK1_ANIMATION_ARM_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
};
static const D3DXVECTOR3 ATTACK1_ANIMATION_WAIST[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, -0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.005f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),

};
static const D3DXVECTOR3 ATTACK1_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.25f),
	D3DXVECTOR3(0.5f, 0.0f, 0.25f),
	D3DXVECTOR3(0.0f, 0.0f, 0.25f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.25f),
	D3DXVECTOR3(0.0f, 0.0f, 0.25f),
};
static const D3DXVECTOR3 ATTACK1_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.25f),
	D3DXVECTOR3(-0.5f, 0.0f, -0.25),
	D3DXVECTOR3(0.0f, 0.0f, -0.25),
	D3DXVECTOR3(0.5f, 0.0f, -0.25),
	D3DXVECTOR3(0.0f, 0.0f, -0.25),
};

// コンストラクタ
Attack1Animation::Attack1Animation(DWORD _flag) :PartsAnimation(_flag)
{
	numKeyFrame = sizeof(ATTACK1_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// 更新
void Attack1Animation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(ATTACK1_ANIMATION_BODY);
	rot[ARM_L] = updateRotation(ATTACK1_ANIMATION_ARM_L);
	rot[ARM_R] = updateRotation(ATTACK1_ANIMATION_ARM_R);
	rot[WAIST] = updateRotation(ATTACK1_ANIMATION_WAIST);
	rot[LEG_L] = updateRotation(ATTACK1_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(ATTACK1_ANIMATION_LEG_R);
}


//=============================================================================
// 死亡アニメーション
//=============================================================================
static const D3DXVECTOR3 DEAD_ANIMATION_BODY[] =
{
	D3DXVECTOR3(-0.3f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.3f, 0.2f, 0.0f),
	D3DXVECTOR3(-0.3f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.3f, -0.2f, 0.0f),
	D3DXVECTOR3(-0.3f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_ARM_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_ARM_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_WAIST[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.1f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -0.1f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
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
	rot[ARM_L] = updateRotation(DEAD_ANIMATION_ARM_L);
	rot[ARM_R] = updateRotation(DEAD_ANIMATION_ARM_R);
	rot[WAIST] = updateRotation(DEAD_ANIMATION_WAIST);
	rot[LEG_L] = updateRotation(DEAD_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(DEAD_ANIMATION_LEG_R);
}


//=============================================================================
#pragma endregion


#pragma region [Manager]
///////////////////////////////////////////////////////////////////////////////
/////////////////////////  アニメーションマネージャー /////////////////////////
///////////////////////////////////////////////////////////////////////////////
//=============================================================================
// コンストラクタ
//=============================================================================
BearAnimationManager::BearAnimationManager(int partsMax, Object* parent, Object** parts)
	: PartsAnimationManager(partsMax, parent, parts)
{
	animation = new PartsAnimation*[ANIMATION_TYPE_MAX];

	// アニメーションを作成
	animation[DEFAULT] = new DefaultAnimation(ANIMATION_FLAG[DEFAULT]);
	animation[MOVE] = new MoveAnimation(ANIMATION_FLAG[MOVE]);
	animation[ATTACK1] = new Attack1Animation(ANIMATION_FLAG[ATTACK1]);
	animation[ATTACK2] = NULL;
	animation[ATTACK3] = NULL;
	animation[DEAD] = new DeadAnimation(ANIMATION_FLAG[DEAD]);
}


//=============================================================================
// デストラクタ
//=============================================================================
BearAnimationManager::~BearAnimationManager()
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
void BearAnimationManager::update(float frameTime)
{
	// 回転の更新
	for (int i = 0; i < ANIMATION_TYPE_MAX; i++)
	{
		if (animation[i] == NULL) { continue; }

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
void BearAnimationManager::culcPartsMatrix()
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

		// 子パーツのワールドマトリクスに親パーツの回転マトリクスを掛ける
		if (i == ARM_L || i == ARM_R)
		{
			D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[BODY]->matrixRotation);
		}
		else if (i == LEG_L || i == LEG_R)
		{
			D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[WAIST]->matrixRotation);
		}

		// 位置マトリクスをワールドマトリクスに掛ける
		D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[i]->matrixPosition);

		// パーツのワールドマトリクスに親オブジェクトのワールドマトリクスを掛ける
		D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parent->matrixWorld);
	}
}


//=============================================================================
// デフォルトアニメーションに切り替え
//=============================================================================
void BearAnimationManager::switchDefault()
{
	// 攻撃アニメーションが終了していないと再生されない
	if (animation[ATTACK1]->wasPlayedToEnd)
	{
		inactivate(animation[ATTACK1]);
		activate(animation[DEFAULT]);
	}
}


//=============================================================================
// 移動アニメーションに切り替え
//=============================================================================
void BearAnimationManager::switchMove()
{
	// 攻撃アニメーションが終了していないと再生されない
	if (animation[ATTACK1]->wasPlayedToEnd)
	{
		inactivate(animation[ATTACK1]);
		activate(animation[MOVE]);
	}
}



//=============================================================================
// 攻撃アニメーションに切り替え
//=============================================================================
void BearAnimationManager::switchAttack()
{
	inactivateAll();
	activate(animation[ATTACK1]);
}


//=============================================================================
// 死亡アニメーションに切り替え
//=============================================================================
void BearAnimationManager::switchDead()
{
	inactivateAll();
	activate(animation[DEAD]);
}
#pragma endregion