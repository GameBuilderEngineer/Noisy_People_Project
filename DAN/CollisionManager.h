//===================================================================================================================================
//yCollisionManager.hz
// [์ฌา]HALGP12A332 11 ์ ๗
// [์ฌ๚]2019/11/11
// [XV๚]2019/11/11
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//yCN[hz
//===================================================================================================================================
#include "Base.h"
#include "ObjectTypeList.h"
#include "Object.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

//===================================================================================================================================
//yผO๓ิz
//===================================================================================================================================
namespace CollisionManagerNS{
	//gนXg
	enum COLLISION_COMBI{
		//NONE
		NONE_NONE,
		NONE_PLAYER,
		NONE_BULLET,
		NONE_ENEMY,

		//PLAYER
		PLAYER_NONE,
		PLAYER_PLAYER,
		PLAYER_BULLET,
		PLAYER_ENEMY,

		//BULLET
		BULLET_NONE,
		BULLET_PLAYER,
		BULLET_BULLET,
		BULLET_ENEMY,

		//BULLET
		ENEMY_NONE,
		ENEMY_PLAYER,
		ENEMY_BULLET,
		ENEMY_ENEMY,

		COMBI_NUM
	};

	const int COMBI[ObjectType::TYPE_NUM][ObjectType::TYPE_NUM]=
	{
		//NONE
		NONE_NONE,
		NONE_PLAYER,
		NONE_BULLET,
		NONE_ENEMY,

		//PLAYER
		PLAYER_NONE,
		PLAYER_PLAYER,
		PLAYER_BULLET,
		PLAYER_ENEMY,

		//BULLET
		BULLET_NONE,
		BULLET_PLAYER,
		BULLET_BULLET,
		BULLET_ENEMY,

	};

}

//===================================================================================================================================
//yีหวNXz
//===================================================================================================================================
class CollisionManager :public Base
{
public:
	CollisionManager();
	~CollisionManager();

	//ีหป่ึ
	static bool collision(Object* obj1, Object* obj2);

	//PLAYER
	static bool playerAndPlayer(Player* player1, Player* player2);		//PLAYER<->PLAYER
	static bool playerAndBullet(Player* player, Bullet* bullet);		//PLAYER<->BULLET
	static bool playerAndEnemy(Player* player, Enemy* enemy);			//PLAYER<->BULLET
	
	//BULLET
	static bool bulletAndPlayer(Player* player, Bullet* bullet);		//PLAYER<->BULLET
	//static bool bulletAndBullet(Player* player, Bullet* bullet);		//BULLET<->BULLET

	//ENEMY

};
