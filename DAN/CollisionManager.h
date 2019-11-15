//===================================================================================================================================
//yCollisionManager.hz
// [ì¬Ò]HAL“Œ‹GP12A332 11 ›–ì ÷
// [ì¬“ú]2019/11/11
// [XV“ú]2019/11/12
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//yƒCƒ“ƒNƒ‹[ƒhz
//===================================================================================================================================
#include "Base.h"
#include "ObjectTypeList.h"
#include "Object.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Tree.h"

//===================================================================================================================================
//y–¼‘O‹óŠÔz
//===================================================================================================================================
namespace CollisionManagerNS{

}

//===================================================================================================================================
//yÕ“ËŠÇ—ƒNƒ‰ƒXz
//===================================================================================================================================
class CollisionManager :public Base
{
public:
	CollisionManager();
	~CollisionManager();

	//Õ“Ë”»’èŠÖ”
	static bool collision(Object* obj1, Object* obj2);

	//‹…Õ“Ë”»’è
	static bool collisionSphere(Object* obj1, Object* obj2);
	//‰~’ŒÕ“Ë”»’èi”ñ‰ñ“]j
	static bool collisionCylinder(Object* obj1, Object* obj2);
	//…•½•â³(ratio1 = obj1‚ª•â³‚³‚ê‚éŠ„‡(0.0‚Í“®‚©‚È‚¢/1.0‚Í‚·‚×‚Ä‚Ì•â³‚ğ‚¤‚¯‚é)j
	static void horizontalCorrection(Object* obj1, Object* obj2,float ratio1);


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
