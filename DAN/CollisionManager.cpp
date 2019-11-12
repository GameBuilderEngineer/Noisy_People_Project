//===================================================================================================================================
//【CollisionManager.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/11
// [更新日]2019/11/11
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
	if (obj1->type == ObjectType::NONE || obj2->type == ObjectType::NONE)return false;

	int type1 = obj1->type;
	int type2 = obj2->type;

	switch (COMBI[type1][type2])
	{
	//PLAYER
	case PLAYER_PLAYER:	return playerAndPlayer((Player*)obj1, (Player*)obj2); break;
	case PLAYER_BULLET: return playerAndBullet((Player*)obj1, (Bullet*)obj2); break;
	case PLAYER_ENEMY:	return playerAndEnemy((Player*)obj1, (Enemy*)obj2); break;
	case PLAYER_TREE:	return playerAndTree((Player*)obj1, (Tree*)obj2); break;

	//BULLET
	case BULLET_PLAYER: return playerAndBullet((Player*)obj2, (Bullet*)obj1); break;
	case BULLET_BULLET: return bulletAndBullet((Bullet*)obj1,(Bullet*)obj2); break;
	case BULLET_ENEMY:	return bulletAndEnemy((Bullet*)obj1,(Enemy*)obj2); break;
	case BULLET_TREE:	return bulletAndTree((Bullet*)obj1,(Tree*)obj2); break;

	//ENEMY
	case ENEMY_PLAYER:	return playerAndEnemy((Player*)obj2, (Enemy*)obj1); break;
	case ENEMY_BULLET:	return bulletAndTree((Bullet*)obj2,(Tree*)obj1); break;
	case ENEMY_ENEMY:	return enemyAndEnemy((Enemy*)obj1,(Enemy*)obj2); break;
	case ENEMY_TREE:	return enemyAndTree((Enemy*)obj1,(Tree*)obj2); break;

	//TREE
	case TREE_PLAYER:	return playerAndTree((Player*)obj2, (Tree*)obj1); break;
	case TREE_BULLET:	return bulletAndTree((Bullet*)obj2,(Tree*)obj1); break;
	case TREE_ENEMY:	return enemyAndTree((Enemy*)obj2,(Tree*)obj1); break;
	case TREE_TREE:		return treeAndTree((Tree*)obj1,(Tree*)obj2); break;

	default:break;
	}

	return false;
}


//===================================================================================================================================
//【プレイヤー <-> プレイヤー】
//===================================================================================================================================
bool CollisionManager::playerAndPlayer(Player* player1, Player* player2)
{
	bool hit = false;
	hit = player1->getBodyCollide()->collide(
		player2->getBodyCollide()->getCenter(), player2->radius,
		player1->matrixWorld, player2->matrixWorld);

	if (hit)
	{
		D3DXVECTOR3 repulsion;//反発ベクトル
		float length = Base::between2VectorDirection(&repulsion, player1->position, player2->position);
		length = ((player1->radius+player2->radius)-length)/2.0f;
		player1->position -= repulsion * length;
		player2->position += repulsion * length;
		player1->Object::update();
		player2->Object::update();
	}

	return hit;
}


//===================================================================================================================================
//【プレイヤー <-> バレット】
//===================================================================================================================================
bool CollisionManager::playerAndBullet(Player* player, Bullet* bullet)
{
	bool hit = false;

	float distance = Base::between2VectorLength(player->position, bullet->position);
	float correct = player->radius + bullet->radius;

	if( correct > distance)
	{
		bullet->existenceTimer = 0.0f;
	}

	return hit;
}

//===================================================================================================================================
//【プレイヤー <-> エネミー】
//===================================================================================================================================
bool CollisionManager::playerAndEnemy(Player* player, Enemy* enemy)
{
	bool hit = false;
	//hit = enemy->getSphereCollider()->collide(
		//player->getBodyCollide()->getCenter(), player->radius,
		//*enemy->getCentralMatrixWorld(), *player->getcentralMatrixWorld());

	float distance = Base::between2VectorLength(player->position, enemy->position);
	if (enemy->radius + player->radius >= distance)
	{
		hit = true;
	}

	if (hit)
	{
		D3DXVECTOR3 repulsion;//反発ベクトル
		float length = Base::between2VectorDirection(&repulsion, player->position, enemy->position);
		length = ((player->radius + enemy->radius) - length) / 2.0f;
		player->position -= repulsion * length;
		enemy->position += repulsion * length;
		player->Object::update();
		enemy->Object::update();
	}

	return hit;
}

//===================================================================================================================================
//【プレイヤー <-> ツリー】
//===================================================================================================================================
bool CollisionManager::playerAndTree(Player* player, Tree* tree)
{
	bool hit = false;
	//hit = enemy->getSphereCollider()->collide(
		//player->getBodyCollide()->getCenter(), player->radius,
		//*enemy->getCentralMatrixWorld(), *player->getcentralMatrixWorld());



	//水平距離
	D3DXVECTOR3 horizontalPlayer	= D3DXVECTOR3(player->position.x, 0.0f, player->position.x);
	D3DXVECTOR3 horizontalTree		= D3DXVECTOR3(tree->position.x, 0.0f, tree->position.z);
	float horizontalDistance = Base::between2VectorLength(horizontalPlayer,horizontalTree);

	//垂直距離
	D3DXVECTOR3 verticalPlayer = D3DXVECTOR3(0.0f, player->position.y, 0.0f);
	D3DXVECTOR3 verticalTree = D3DXVECTOR3(0.0f, tree->position.y, 0.0f);
	float verticalDistance = Base::between2VectorLength(verticalPlayer, verticalTree);

	//適正水平距離
	float horizontalCorrect = tree->getRadius() + player->radius/2;
	//適正垂直距離
	float verticalCorrect = tree->getHight() + player->radius;

	//衝突判定
	if (horizontalCorrect >= horizontalDistance &&
		verticalCorrect >= verticalDistance)
	{
		hit = true;
	}

	if (hit)
	{
		D3DXVECTOR3 repulsion;//反発ベクトル
		Base::between2VectorDirection(&repulsion, horizontalPlayer, horizontalTree);
		float length = (horizontalCorrect - horizontalDistance);
		player->position -= repulsion * length;
		player->Object::update();
	}

	return hit;
}

//BULLET<->BULLET
bool CollisionManager::bulletAndBullet(Bullet* bullet1, Bullet* bullet2) 
{
	return false;
}

//BULLET<->ENEMY
bool CollisionManager::bulletAndEnemy(Bullet* player, Enemy* enemy)
{
	return false;
}
//BULLET<->TREE
bool CollisionManager::bulletAndTree(Bullet* player, Tree* tree)
{
	return false;
}

//ENEMY<->ENEMY
bool CollisionManager::enemyAndEnemy(Enemy* enemy1, Enemy* enemy2)
{
	return false;
}
//ENEMY<->TREE
bool CollisionManager::enemyAndTree(Enemy* player, Tree* tree)
{
	return false;
}

//TREE
//ENEMY<->TREE
bool CollisionManager::treeAndTree(Tree* tree1, Tree* tree2)
{
	return false;
}
