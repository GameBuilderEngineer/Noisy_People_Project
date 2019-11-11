//===================================================================================================================================
//【CollisionManager.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/11
// [更新日]2019/11/11
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "ObjectTypeList.h"
#include "Object.h"
#include "Player.h"
#include "Bullet.h"


//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace CollisionManagerNS{
	//組合せリスト
	enum COLLISION_COMBI{
		//NONE
		NONE_NONE,
		NONE_PLAYER,
		NONE_BULLET,

		//PLAYER
		PLAYER_NONE,
		PLAYER_PLAYER,
		PLAYER_BULLET,

		//BULLET
		BULLET_NONE,
		BULLET_PLAYER,
		BULLET_BULLET,

		COMBI_NUM
	};

	const int COMBI[ObjectType::TYPE_NUM][ObjectType::TYPE_NUM]=
	{
		//NONE
		NONE_NONE,
		NONE_PLAYER,
		NONE_BULLET,

		//PLAYER
		PLAYER_NONE,
		PLAYER_PLAYER,
		PLAYER_BULLET,

		//BULLET
		BULLET_NONE,
		BULLET_PLAYER,
		BULLET_BULLET,

	};




}

//===================================================================================================================================
//【衝突管理クラス】
//===================================================================================================================================
class CollisionManager :public Base
{
public:
	CollisionManager();
	~CollisionManager();

	//衝突判定関数
	static bool collision(Object* obj1, Object* obj2);

	//PLAYER
	static bool playerAndPlayer(Player* player1, Player* player2);		//PLAYER<->PLAYER
	static bool playerAndBullet(Player* player, Bullet* bullet);		//PLAYER<->BULLET
	
	//BULLET
	static bool bulletAndPlayer(Player* player, Bullet* bullet);		//PLAYER<->BULLET
	//static bool bulletAndBullet(Player* player, Bullet* bullet);		//BULLET<->BULLET


};
