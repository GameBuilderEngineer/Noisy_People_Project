//===================================================================================================================================
//ÅyCollisionManager.hÅz
// [çÏê¨é“]HALìåãûGP12A332 11 êõñÏ é˜
// [çÏê¨ì˙]2019/11/11
// [çXêVì˙]2019/11/11
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//ÅyÉCÉìÉNÉãÅ[ÉhÅz
//===================================================================================================================================
#include "Base.h"
#include "ObjectTypeList.h"
#include "Object.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Tree.h"

//===================================================================================================================================
//ÅyñºëOãÛä‘Åz
//===================================================================================================================================
namespace CollisionManagerNS{
	//ëgçáÇπÉäÉXÉg
	enum COLLISION_COMBI{
		//NONE
		NONE_NONE,
		NONE_PLAYER,
		NONE_BULLET,
		NONE_ENEMY,
		NONE_TREE,

		//PLAYER
		PLAYER_NONE,
		PLAYER_PLAYER,
		PLAYER_BULLET,
		PLAYER_ENEMY,
		PLAYER_TREE,

		//BULLET
		BULLET_NONE,
		BULLET_PLAYER,
		BULLET_BULLET,
		BULLET_ENEMY,
		BULLET_TREE,

		//ENEMY
		ENEMY_NONE,
		ENEMY_PLAYER,
		ENEMY_BULLET,
		ENEMY_ENEMY,
		ENEMY_TREE,

		//TREE
		TREE_NONE,
		TREE_PLAYER,
		TREE_BULLET,
		TREE_ENEMY,
		TREE_TREE,



		COMBI_NUM
	};

	const int COMBI[ObjectType::TYPE_NUM][ObjectType::TYPE_NUM]=
	{
		//NONE
		NONE_NONE,
		NONE_PLAYER,
		NONE_BULLET,
		NONE_ENEMY,
		NONE_TREE,

		//PLAYER
		PLAYER_NONE,
		PLAYER_PLAYER,
		PLAYER_BULLET,
		PLAYER_ENEMY,
		PLAYER_TREE,

		//BULLET
		BULLET_NONE,
		BULLET_PLAYER,
		BULLET_BULLET,
		BULLET_ENEMY,
		BULLET_TREE,

		//ENEMY
		ENEMY_NONE,
		ENEMY_PLAYER,
		ENEMY_BULLET,
		ENEMY_ENEMY,
		ENEMY_TREE,

		//TREE
		TREE_NONE,
		TREE_PLAYER,
		TREE_BULLET,
		TREE_ENEMY,
		TREE_TREE,
	};

}

//===================================================================================================================================
//Åyè’ìÀä«óùÉNÉâÉXÅz
//===================================================================================================================================
class CollisionManager :public Base
{
public:
	CollisionManager();
	~CollisionManager();

	//è’ìÀîªíËä÷êî
	static bool collision(Object* obj1, Object* obj2);

	//PLAYER
	static bool playerAndPlayer(Player* player1, Player* player2);		//PLAYER<->PLAYER
	static bool playerAndBullet(Player* player, Bullet* bullet);		//PLAYER<->BULLET
	static bool playerAndEnemy(Player* player, Enemy* enemy);			//PLAYER<->ENEMY
	static bool playerAndTree(Player* player, Tree* enemy);				//PLAYER<->TREE
	
	//BULLET
	static bool bulletAndBullet(Bullet* bullet1, Bullet* bullet2);		//BULLET<->BULLET
	static bool bulletAndEnemy(Bullet* player, Enemy* enemy);			//BULLET<->ENEMY
	static bool bulletAndTree(Bullet* player, Tree* tree);				//BULLET<->TREE

	//ENEMY
	static bool enemyAndEnemy(Enemy* enemy1, Enemy* enemy2);			//ENEMY<->ENEMY
	static bool enemyAndTree(Enemy* player, Tree* tree);				//ENEMY<->TREE
	
	//TREE
	static bool treeAndTree(Tree* tree1, Tree* tree2);				//ENEMY<->TREE

};
