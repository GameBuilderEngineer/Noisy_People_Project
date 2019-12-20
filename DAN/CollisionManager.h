//===================================================================================================================================
//【CollisionManager.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/11
// [更新日]2019/11/12
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
#include "Enemy.h"
#include "Tree.h"
#include "GreeningArea.h"
#include "MapObject.h"
#include "TigerBullet.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace CollisionManagerNS{

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

	//衝突判定
	//球衝突判定
	static bool collisionSphere(Object* obj1, Object* obj2);
	//球と点の衝突判定
	//obj1:球<->obj2:点
	static bool collisionSphereAndPoint(Object* obj1, Object* obj2);
	//円柱衝突判定（非回転）
	static bool collisionCylinder(Object* obj1, Object* obj2);

	//アクション
	//水平補正(ratio1 = obj1が補正される割合(0.0は動かない/1.0はすべての補正をうける)）
	static void horizontalCorrection(Object* obj1, Object* obj2,float ratio1);

	//PLAYER
	static bool playerAndPlayer(Player* player1, Player* player2);			//PLAYER<->PLAYER
	static bool playerAndBullet(Player* player, Bullet* bullet);			//PLAYER<->BULLET
	static bool playerAndEnemy(Player* player, Enemy* enemy);				//PLAYER<->ENEMY
	static bool playerAndTree(Player* player, Tree* enemy);					//PLAYER<->TREE
	static bool playerAndMapObject(Player* player, MapObject* mapObject);	//PLAYER<->MAPOBJECT
	static bool playerAndEnemyBullet(Player* player, TigerBullet* bullet);	//PLAYER<->ENEMY_BULLET
	
	//BULLET
	static bool bulletAndBullet(Bullet* bullet1, Bullet* bullet2);		//BULLET<->BULLET
	static bool bulletAndEnemy(Bullet* player, Enemy* enemy);			//BULLET<->ENEMY
	static bool bulletAndTree(Bullet* player, Tree* tree);				//BULLET<->TREE

	//ENEMY
	static bool enemyAndEnemy(Enemy* enemy1, Enemy* enemy2);			//ENEMY<->ENEMY
	static bool enemyAndTree(Enemy* player, Tree* tree);				//ENEMY<->TREE
	
	//TREE
	static bool treeAndTree(Tree* tree1, Tree* tree2);					//ENEMY<->TREE

	//GREENING_AREA
	static bool greeningAreaAndTree(GreeningArea* area, Tree* tree);	//GREENING_AREA<->TREE
};
