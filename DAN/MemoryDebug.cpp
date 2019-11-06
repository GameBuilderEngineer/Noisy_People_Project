//-----------------------------------------------------------------------------
// メモリデバッグ [MemoryDebug.cpp]
//-----------------------------------------------------------------------------
#include "MemoryDebug.h"
#include <vector>


#define LOOPTIME (100)

//=============================================================================
// エネミーマネージャ
//=============================================================================
void MemoryDebug::debugEnemyManager()
{
	int numAfterCreate, numAfterDestroy;
	int numDataAfterCreate, numDataAfterDestroy;
	EnemyManager* enemyManager = new EnemyManager;
	enemyManager->initialize(NULL, NULL, NULL, NULL);

	// enemyID
	for(int i = 0; i < LOOPTIME; i++)
	{
		enemyNS::ENEMYSET temp =
		{
			enemyManager->issueNewEnemyID(),
			enemyNS::WOLF,
			stateMachineNS::CHASE,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		};

		enemyNS::EnemyData* p = enemyManager->createEnemyData(temp);
		enemyManager->createEnemy(p);
	}
	numAfterCreate = enemyManager->getEnemyList().size();
	numDataAfterCreate = enemyManager->getEnemyDataList()->nodeNum;

#if 0
	// 全て破棄
	enemyManager->destroyAllEnemy();
	//enemyManager->destroyAllEnemyData();
#endif

#if 1
	// IDで破棄
	for (int i = 0; i < enemyManager->getNextID(); i++)
	{
		enemyManager->destroyEnemy(i);
		//enemyManager->destroyEnemyData(i);
	}
#endif

	numAfterDestroy = enemyManager->getEnemyList().size();
	numDataAfterDestroy = enemyManager->getEnemyDataList()->nodeNum;

	return;
}


//=============================================================================
// その他マネージャ
//=============================================================================
void MemoryDebug::debugSpawnManager()
{
	// ツリーマネージャ
	TreeManager* treeManager = new TreeManager;
	treeManager->initialize(NULL, NULL);

	for (int i = 0; i < LOOPTIME; i++)
	{
		treeNS::TreeData treeData;
		treeData.treeID = treeManager->issueNewTreeID();
		treeData.geenState = treeNS::GREEN;
		treeData.model = treeNS::A_MODEL;
		treeData.initialPosition = D3DXVECTOR3(0, 0, 0);

		treeManager->createTree(treeData);
	}
	
	int sizeTree = treeManager->getTreeList().size();
	treeManager->destroyAllTree();
	

	// アイテムマネージャ
	ItemManager* itemManager = new ItemManager;
	itemManager->initialize(NULL, NULL);

	for (int i = 0; i < LOOPTIME; i++)
	{
		itemNS::ItemData itemData;
		itemData.itemID = itemManager->issueNewItemID();
		itemData.type = itemNS::BATTERY;
		itemManager->createItem(itemData);
	}

	int sizeItem = itemManager->getItemList().size();

#if 1
	for (int i = 0; i < LOOPTIME; i++)
	{
		itemManager->destroyItem(i);
	}
#endif
#if 0
	itemManager->destroyAllItem();
#endif

	int sizeItemAfter = itemManager->getItemList().size();
	return;
}
