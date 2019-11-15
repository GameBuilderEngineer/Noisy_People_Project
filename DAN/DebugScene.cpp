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

	player = new Player();
	//player->initialize();

#pragma region Memory Test
	//メモリテスト
	i = 0;
	int j;
	while (i < 10000)
	{
		enemyManager = new EnemyManager();
		enemyManager->initialize(*getSceneName(), testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld(), player);
		//itemManager->update(1.0/60.0);
		//itemManager->uninitialize();
		//SAFE_DELETE(itemManager);
		D3DXVECTOR3 pos = D3DXVECTOR3(rand() % 400, 150, rand() % 480);
		pos -= D3DXVECTOR3(200, 0, 240);
		enemyNS::ENEMYSET tmp =
		{
			enemyManager->issueNewEnemyID(),
			enemyNS::WOLF,
			stateMachineNS::PATROL,
			pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		};
		enemyNS::EnemyData* p = enemyManager->createEnemyData(tmp);
		enemyManager->createEnemy(p);
		i++;
		enemyManager->destroyEnemy(i);
	}
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
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("LoopCount  = %d", i);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("stopTime = %f", stopTimer);
}
#endif