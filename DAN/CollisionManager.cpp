//===================================================================================================================================
//【CollisionManager.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/11
// [更新日]2019/11/14
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "CollisionManager.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace CollisionManagerNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
CollisionManager::CollisionManager()
{

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
CollisionManager::~CollisionManager()
{

}

//===================================================================================================================================
//【衝突処理判定】
//===================================================================================================================================
bool CollisionManager::collision(Object* obj1, Object* obj2)
{
	using namespace ObjectType;
	if (obj1->treeCell.type == NONE || obj2->treeCell.type == NONE)return false;

	int type1	= obj1->treeCell.type;
	int type2	= obj2->treeCell.type;

	switch (type1)
	{
	case PLAYER:
		switch (type2){
		case PLAYER:		return playerAndPlayer((Player*)obj1, (Player*)obj2);				break;
		case BULLET:		return false;														break;
		case ENEMY:			return playerAndEnemy((Player*)obj1, (Enemy*)obj2);					break;
		case TREE:			return playerAndTree((Player*)obj1, (Tree*)obj2);					break;
		case GREENING_AREA:	return false;														break;
		}break;
	case BULLET:
		switch (type2) {
		case PLAYER:			return false;													break;
		case BULLET:			return false;													break;
		case ENEMY:				return bulletAndEnemy((Bullet*)obj1, (Enemy*)obj2);				break;
		case TREE:				return bulletAndTree((Bullet*)obj1, (Tree*)obj2);				break;
		case GREENING_AREA:		return false;													break;
		}break;
	case ENEMY:
		switch (type2) {
		case PLAYER:			return playerAndEnemy((Player*)obj2, (Enemy*)obj1);				break;
		case BULLET:			return bulletAndEnemy((Bullet*)obj2, (Enemy*)obj1);				break;
		case ENEMY:				return enemyAndEnemy((Enemy*)obj1, (Enemy*)obj2);				break;
		case TREE:				return enemyAndTree((Enemy*)obj1, (Tree*)obj2);					break;
		case GREENING_AREA:		return false;													break;
		}break;
	case TREE:
		switch (type2) {
		case PLAYER:			return playerAndTree((Player*)obj2, (Tree*)obj1);				break;
		case BULLET:			return bulletAndTree((Bullet*)obj2, (Tree*)obj1);				break;
		case ENEMY:				return enemyAndTree((Enemy*)obj2, (Tree*)obj1);					break;
		case TREE:				return false;													break;
		case GREENING_AREA:		return greeningAreaAndTree((GreeningArea*)obj2, (Tree*)obj1);	break;
		}break;
	case GREENING_AREA:
		switch (type2) {
		case PLAYER:			return false;													break;
		case BULLET:			return false;													break;
		case ENEMY:				return false;													break;
		case TREE:				return greeningAreaAndTree((GreeningArea*)obj1,(Tree*)obj2);	break;
		case GREENING_AREA:		return false;													break;
		}break;
	default:return false; break;
	}

	return false;

}



#pragma region CollisionFunction
//===================================================================================================================================
//【球による衝突判定】
//===================================================================================================================================
bool CollisionManager::collisionSphere(Object* obj1, Object* obj2)
{
	return 	obj1->sphere->collide(	
		obj2->center, 		
		obj2->radius, 		
		obj1->matrixCenter, 
		obj2->matrixCenter);
}

//===================================================================================================================================
//【球と点による衝突判定】
//	obj1:球<->obj2:点
//===================================================================================================================================
bool CollisionManager::collisionSphereAndPoint(Object* obj1, Object* obj2)
{
	bool hit = false;

	float distance = Base::between2VectorLength(obj1->center, obj2->center);
	if (obj1->sphere->getRadius() > distance)
	{
		hit = true;
	}

	return hit;
}

//===================================================================================================================================
//【円柱による衝突判定】
//===================================================================================================================================
bool CollisionManager::collisionCylinder(Object* obj1, Object* obj2)
{
	bool hit = false;

	//中央位置準拠
	//垂直位置
	D3DXVECTOR3 vertical1 = D3DXVECTOR3(0.0f, obj1->center.y, 0.0f);
	D3DXVECTOR3 vertical2 = D3DXVECTOR3(0.0f, obj2->center.y, 0.0f);
	//垂直距離
	float verticalDistance = Base::between2VectorLength(vertical1, vertical2);
	//正垂直距離(非衝突時の距離)
	float verticalCorrect = obj1->size.y / 2 + obj2->size.y / 2;

	//水平位置
	D3DXVECTOR3 horizontal1	= D3DXVECTOR3(obj1->center.x, 0.0f, obj1->center.z);
	D3DXVECTOR3 horizontal2	= D3DXVECTOR3(obj2->center.x, 0.0f, obj2->center.z);
	//水平距離
	float horizontalDistance = Base::between2VectorLength(horizontal1, horizontal2);
	//正水平距離(非衝突時の距離)
	float horizontalCorrect = obj1->size.x / 2 + obj2->size.x / 2;

	//衝突判定
	if (verticalCorrect >= verticalDistance &&
		horizontalCorrect >= horizontalDistance)
	{
		hit = true;
	}

	return hit;
}
#pragma endregion

#pragma region ActionFunction
void CollisionManager::horizontalCorrection(Object* obj1, Object* obj2, float ratio1)
{
	//水平位置
	D3DXVECTOR3 horizontal1 = D3DXVECTOR3(obj1->center.x, 0.0f, obj1->center.z);
	D3DXVECTOR3 horizontal2 = D3DXVECTOR3(obj2->center.x, 0.0f, obj2->center.z);
	//水平距離
	float horizontalDistance = Base::between2VectorLength(horizontal1, horizontal2);
	//正水平距離(非衝突時の距離)
	float horizontalCorrect = obj1->size.x / 2 + obj2->size.x / 2;

	D3DXVECTOR3 repulsion;//反発ベクトル
	Base::between2VectorDirection(&repulsion, horizontal1, horizontal2);
	float length = (horizontalCorrect - horizontalDistance);
	if (ratio1 > 0.0f) {
		obj1->position -= repulsion * (length*ratio1);
		obj1->Object::update();
	}
	if (ratio1 <= 1.0f)
	{
		obj2->position += repulsion * (length*(1.0f-ratio1));
		obj2->Object::update();
	}
}
#pragma endregion

#pragma region PLAYER
//===================================================================================================================================
//【プレイヤー <-> プレイヤー】
//===================================================================================================================================
bool CollisionManager::playerAndPlayer(Player* player1, Player* player2)
{
	if (collisionCylinder(player1, player2))
	{
		horizontalCorrection(player1, player2, 0.5f);
		return true;
	}
	return false;
}

//===================================================================================================================================
//【プレイヤー <-> バレット】
//===================================================================================================================================
bool CollisionManager::playerAndBullet(Player* player, Bullet* bullet)
{
	//bool hit = false;

	//float distance = Base::between2VectorLength(player->position, bullet->position);
	//float correct = player->radius + bullet->radius;

	if(collisionSphere(player,bullet))
	{
		if (bullet->collide(player->getMesh(), player->matrixWorld))
		{
			bullet->destroy();
			return true;
		}
	}
	return false;
}

//===================================================================================================================================
//【プレイヤー <-> エネミー】
//===================================================================================================================================
bool CollisionManager::playerAndEnemy(Player* player, Enemy* enemy)
{
	if (collisionCylinder(player, enemy))
	{
		horizontalCorrection(player, enemy, 0.5f);
		if (enemy->getIsAttacking())
		{
			player->damage(enemyNS::ATTACK_DAMAGE[enemy->getEnemyData()->type]);
			if (player->getOnGround())
			{// 接地時（ノックバックの強さをここで調整）
				player->speed += enemy->speed * 0.45f;
			}
			else
			{// 空中時（ノックバックの強さをここで調整）
				player->speed += enemy->speed * 0.35f;
			}
			enemy->stopAttacking();	// 減速処理が入っているためプレイヤーに速度を移した後に呼ぶ
		}
		return true;
	}
	return false;
}

//===================================================================================================================================
//【プレイヤー <-> ツリー】
//===================================================================================================================================
bool CollisionManager::playerAndTree(Player* player, Tree* tree)
{
	if (collisionCylinder(player, tree))
	{
		horizontalCorrection(player, tree, 1.0);
		return true;
	}
	return false;
}

#pragma endregion

#pragma region BULLET
//===================================================================================================================================
//【バレット<-> バレット】
//===================================================================================================================================
bool CollisionManager::bulletAndBullet(Bullet* bullet1, Bullet* bullet2)
{
	return false;
}

//===================================================================================================================================
//【バレット<-> エネミー】
//===================================================================================================================================
bool CollisionManager::bulletAndEnemy(Bullet* bullet, Enemy* enemy)
{
	bool hit = false;

	if (collisionCylinder(bullet, enemy))
	{
		hit = bullet->collide(enemy->getMesh(), enemy->matrixCenter);
	}

	if (hit)
	{
		enemy->damage(bullet->getDigitalPower());
		enemy->setAttention(-bullet->getBulletSpeed());
		bullet->destroy();
	}

	return hit;
}

//===================================================================================================================================
//【バレット<-> ツリー】
//===================================================================================================================================
bool CollisionManager::bulletAndTree(Bullet* bullet, Tree* tree)
{
	bool hit = false;

	if (collisionCylinder(bullet, tree))
	{
		hit = true;
		//hit = bullet->collide(tree->getMesh(), tree->matrixWorld);
	}

	if (hit)
	{
		tree->addHp(bullet->getDigitalPower());
		bullet->destroy();
	}

	return hit;
}

#pragma endregion

#pragma region ENEMY
//===================================================================================================================================
//【エネミー<->エネミー】
//===================================================================================================================================
bool CollisionManager::enemyAndEnemy(Enemy* enemy1, Enemy* enemy2)
{
	if (collisionCylinder(enemy1, enemy2))
	{
		horizontalCorrection(enemy1, enemy2, 0.5f);
		return true;
	}
	return false;
}

//===================================================================================================================================
//【エネミー<-> ツリー】
//===================================================================================================================================
bool CollisionManager::enemyAndTree(Enemy* enemy, Tree* tree)
{
	if (collisionCylinder(enemy, tree))
	{
		horizontalCorrection(enemy, tree, 1.0f);
		return true;
	}

	return false;
}
#pragma endregion

#pragma region Tree
//===================================================================================================================================
//【ツリー<-> ツリー】
//===================================================================================================================================
bool CollisionManager::treeAndTree(Tree* tree1, Tree* tree2)
{
	return false;
}
#pragma endregion

#pragma region GreeningArea
//===================================================================================================================================
//【緑化エリア<-> ツリー】
//===================================================================================================================================
bool CollisionManager::greeningAreaAndTree(GreeningArea* area, Tree* tree)
{
	bool hit = false;

	hit = collisionSphereAndPoint(area, tree);

	if (hit)
	{
		if(tree->getTreeData()->greenState == treeNS::DEAD)
			tree->transState();//状態遷移
	}

	return hit;
}
#pragma endregion

#pragma region newRegion
#pragma endregion

