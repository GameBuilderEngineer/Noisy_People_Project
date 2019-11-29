#include "DebugScene.h"



DebugScene::DebugScene()
{
	sceneName = "Scene -Debug-";
	nextScene = SceneList::SPLASH;
}


DebugScene::~DebugScene()
{
}

void DebugScene::initialize()
{

	//テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//アイテム
	itemManager = new ItemManager();
	itemManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	
	//プレイヤー
	player = new Player();
	//player->initialize();

	//ツリー
	treeManager = new TreeManager();

#pragma region Memory Test
	//メモリテスト
	i = 0;
	int j;
	// ツリーの設定

	//while (i < 10000)
	//{
	//	//エネミー
	//	/*enemyManager = new EnemyManager();
	//	enemyManager->initialize(*getSceneName(), testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld(), player);
	//	
	//	
	//	D3DXVECTOR3 pos = D3DXVECTOR3(rand() % 400, 150, rand() % 480);
	//	pos -= D3DXVECTOR3(200, 0, 240);
	//	enemyNS::ENEMYSET tmp =
	//	{
	//		enemyManager->issueNewEnemyID(),
	//		enemyNS::WOLF,
	//		stateMachineNS::PATROL,
	//		pos,
	//		D3DXVECTOR3(0.0f, 0.0f, 0.0f)
	//	};
	//	enemyNS::EnemyData* p = enemyManager->createEnemyData(tmp);
	//	enemyManager->createEnemy(p);
	//	
	//	enemyManager->uninitialize();
	//	SAFE_DELETE(enemyManager);*/

	//	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	//	
	//	treeNS::TreeData treeData;
	//	treeData.treeID = treeManager->issueNewTreeID();
	//	treeData.hp = 0;
	//	treeData.type = treeNS::ANALOG_TREE;
	//	treeData.size = treeNS::STANDARD;
	//	treeData.greenState = treeNS::DEAD;
	//	treeData.model = treeNS::B_MODEL;
	//	
	//	treeManager->createTree(treeData);
	//	
	//	//treeManager->update(i / 60);
	//	treeManager->uninitialize();
	//	
	//	i++;
	//}
	//	SAFE_DELETE(treeManager);
		
#pragma endregion

}

void DebugScene::uninitialize()
{

}

void DebugScene::update(float _frameTime)
{

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	if (input->wasKeyPressed(VK_RETURN) ||
		input->wasKeyPressed(VK_SPACE) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::SPECIAL_MAIN)
		)
	{
		changeScene(nextScene);
	}
}

void DebugScene::render()
{

}

void DebugScene::collisions()
{

}

void DebugScene::AI()
{

}


#ifdef _DEBUG
void DebugScene::createGUI()
{
	//フラグ
	bool debugDestroyAllFlag = false;


	//enemyManager->outputGUI();
	treeManager->outputGUI();
	//itemManager->outputGUI();

	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	//ImGui::Text("enemyData = %d", Enemy::getNumOfEnemy());
	ImGui::Text("LoopCount  = %d", i);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("stopTime = %f", stopTimer);

	ImGui::Checkbox("Delete All Item", &debugDestroyAllFlag);

}
#endif

/*itemNS::ItemData unko = { itemManager->issueNewItemID(), itemNS::BATTERY, *player->getPosition() };
		itemManager->createItem(unko);*/
		////itemManager->update(1.0/60.0);
		////itemManager->uninitialize();
		////SAFE_DELETE(itemManager);
		//D3DXVECTOR3 pos = D3DXVECTOR3(rand() % 400, 150, rand() % 480);
		//pos -= D3DXVECTOR3(200, 0, 240);
		//itemNS::ITEMSET tmp =
		//{
		//	itemManager->issueNewItemID(),
		//	itemNS::BATTERY,
		//	pos,
		//	D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		//};
		//enemyNS::EnemyData* p = enemyManager->createEnemyData(tmp);
		//enemyManager->createEnemy(p);
		///*enemyManager->destroyEnemy(i);
		//enemyManager->destroyEnemyData(i);*/
		//itemManager->destroyItem(i);