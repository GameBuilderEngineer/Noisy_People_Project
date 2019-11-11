//-----------------------------------------------------------------------------
// 衝突内容記述クラス [CollisionContent.cpp]
// 作成開始日 : 2019/11/9
//-----------------------------------------------------------------------------
#include <cassert>
#include "CollisionContent.h"


//=============================================================================
// プレイヤーとエネミーと当たり判定
//=============================================================================
void CollisionContent::playerCollideEnemy(Object* obj1, Object* obj2)
{
	Player* player = castPointerFromTwoObject<Player>(obj1, obj2, objectNS::PLAYER);
	Enemy* enemy = castPointerFromTwoObject<Enemy>(obj1, obj2, objectNS::ENEMY);
	assert(player && enemy);

	D3DXVECTOR3 vecToPlayer = *player->getCentralPosition() - *enemy->getCentralPosition();
	float len = D3DXVec3Length(&vecToPlayer);
	if (len <= (player->radius + enemy->radius) * 0.5f/*とりあえず*/)
	{
		// めり込み戻し
		if (player->getOnGround())
		{
			player->slip(vecToPlayer, *player->getGroundNormal());
		}
		D3DXVec3Normalize(&vecToPlayer, &vecToPlayer);
		*player->getPosition() += vecToPlayer * ((player->radius + enemy->radius) * 0.5f/*とりあえず*/ - len);

		// スピードベクトルのスリップ
		player->setSpeed(player->slip(player->speed, vecToPlayer));

		enemy->setIsHitPlayer(true);
	}
	else
	{
		enemy->setIsHitPlayer(false);
	}
}


//=============================================================================
// プレイヤーとエネミーの攻撃の当たり判定
//=============================================================================
void CollisionContent::enemyAttacksPlayer(Object* obj1, Object* obj2)
{
	Player* player = castPointerFromTwoObject<Player>(obj1, obj2, objectNS::PLAYER);
	Enemy* enemy = castPointerFromTwoObject<Enemy>(obj1, obj2, objectNS::ENEMY);
	assert(player && enemy);


	//dammy
}
