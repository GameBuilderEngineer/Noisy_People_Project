//===================================================================================================================================
//【Create.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/20
// [更新日]2019/10/18
//===================================================================================================================================
#pragma once

#ifdef _DEBUG
//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include <vector>
#include "AbstractScene.h"
#include "TmpObject.h"
#include "StaticMeshRenderer.h"
#include "Stone.h"
#include "DeadTree.h"
#include "TreeTypeA.h"
#include "TreeTypeB.h"
#include "TestEffect.h"
#include "EnemyManager.h"
#include "AIDirector.h"
#include "NavigationMesh.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Telop.h"
#include "Sound.h"

#include "EnemyTools.h"
#include "ItemTools.h"
#include "TreeTools.h"
#include "MapObjectTool.h"

//===================================================================================================================================
//【列挙型定数】
//===================================================================================================================================
enum TOOLS_TYPE			//ツールの種類
{
	TOOLS_NONE,
	TOOLS_ENEMY,		//エネミー
	TOOLS_ITEM,			//アイテム
	TOOLS_TREE,			//ツリー
	TOOLS_MAP_OBJECT,	//マップオブジェクト
	TOOLS_MAX
};

//===================================================================================================================================
//【ゲームシーンクラス】
//===================================================================================================================================
class Create : public AbstractScene
{
private:

	//hukankamera
	Camera* topView;
	bool onTopView;
	//仮オブジェクト
	TmpObject *tmpObject;
	StaticMeshRenderer* tmpObjRenderer;
	//フィールド
	Object* testField;
	StaticMeshRenderer* testFieldRenderer;

	//インスタンシングビルボードテスト
	TestEffect* testEffect;

	//エネミーツール
	ENEMY_TOOLS *enemyTools;
	//アイテムツール
	ITEM_TOOLS *itemTools;
	//ツリーツール
	TREE_TOOLS *treeTools;
	//マップオブジェクトツール
	MPOJ_TOOLS *mapObjTools;

	//枯木
	DeadTree* deadTree;
	//木Ａ
	TreeTypeA* treeA;
	//木B
	TreeTypeB* treeB;
	//石
	Stone* stone;

public:
	Create();
	~Create();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	void render3D(Camera currentCamera);
	void renderUI();

	int ToolsListboxType;
	int meshId;

	virtual void createGUI() override;
	void toolsGUI();
	void collideGUI();
};
#endif