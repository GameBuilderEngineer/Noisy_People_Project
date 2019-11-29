#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"
//#include "Splash2D.h"
#include "TreeManager.h"
#include "StaticMeshRenderer.h"
#include "ItemManager.h"
#include "EnemyManager.h"
#include "Player.h"
#include "ItemManager.h"

class DebugScene :public AbstractScene
{
private: 

	int i;				//ループカウント
	std::vector<Tree*> treeList;
	Object* testField;
	StaticMeshRenderer* testFieldRenderer;
	TreeManager* treeManager;
	ItemManager* itemManager;
	EnemyManager* enemyManager;
	Player* player; 
public:
	DebugScene();
	~DebugScene();

	virtual void initialize()  override;
	virtual void uninitialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};

