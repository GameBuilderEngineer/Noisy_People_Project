//===================================================================================================================================
//【Game.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/10/25
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Game.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace gameNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Game::Game()
{
	objectNS::resetCounter();		//オブジェクトカウンターのリセット

	sceneName = "Scene -Game-";

	nextScene = SceneList::RESULT;

	//シーンの更新
	SoundInterface::SwitchAudioBuffer(SceneList::GAME);

	//再生パラメータ
	memset(playParameters, 0, sizeof(playParameters));
	FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, GAME_SE_LIST::GAME_SE_01, false ,NULL,true, filterParameters };
	playParameters[1] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, GAME_SE_LIST::GAME_SE_02, false ,NULL,true, filterParameters };
	playParameters[2] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, GAME_BGM_LIST::GAME_BGM_01, true,1.0f,true, filterParameters };
	playParameters[3] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, GAME_SE_LIST::GAME_SE_01, false ,NULL,true, filterParameters };		//アイテム取得音

	//再生
	SoundInterface::playSound(playParameters[0]);
	SoundInterface::playSound(playParameters[1]);
	SoundInterface::playSound(playParameters[2]);
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Game::~Game()
{
	// サウンドの停止
	SoundInterface::stopSound(playParameters[0]);
	SoundInterface::stopSound(playParameters[1]);
	SoundInterface::stopSound(playParameters[2]);
	SoundInterface::stopSound(playParameters[3]);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Game::initialize() {

	//-----中込テストゾーンその１---------------
	//enemy = new Enemy;
	//camera = new Camera;

	//camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//camera->setTarget(enemy->getPosition());
	//camera->setTargetX(&enemy->getAxisX()->direction);
	//camera->setTargetY(&enemy->getAxisY()->direction);
	//camera->setTargetZ(&enemy->getAxisZ()->direction);
	//camera->setRelative(CAMERA_RELATIVE_QUATERNION);
	//camera->setGaze(D3DXVECTOR3(0, 0, 0));
	//camera->setRelativeGaze(CAMERA_RELATIVE_GAZE);
	//camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	//camera->setFieldOfView((D3DX_PI / 18) * 10);
	//-------------------------------------------

	//テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::YAMADA_TEST_ZONE));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//player
	player			= new Player[gameMasterNS::PLAYER_NUM];
	maleRenderer	= new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_MALE));
	femaleRenderer	= new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_FEMALE));

	//camera
	camera = new Camera[gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//カメラの設定
		camera[i].initialize(WINDOW_WIDTH/2, WINDOW_HEIGHT);
		camera[i].setTarget(player[i].getPosition());
		camera[i].setTargetX(&player[i].getAxisX()->direction);
		camera[i].setTargetY(&player[i].getAxisY()->direction);
		camera[i].setTargetZ(&player[i].getAxisZ()->direction);
		camera[i].setRelative(CAMERA_RELATIVE_QUATERNION);
		camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
		camera[i].setRelativeGaze(CAMERA_RELATIVE_GAZE);
		camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
		camera[i].setFieldOfView( (D3DX_PI/180) * 90 );
		
		//プレイヤーの設定
		PlayerTable infomation;
		switch (i)
		{
		case gameMasterNS::PLAYER_1P:
			infomation.playerType	= gameMasterNS::PLAYER_1P;
			infomation.modelType	= gameMasterNS::MODEL_FEMALE;
			player[i].initialize(infomation);
			break;
		case gameMasterNS::PLAYER_2P:
			infomation.playerType	= gameMasterNS::PLAYER_2P;
			infomation.modelType	= gameMasterNS::MODEL_MALE;
			player[i].initialize(infomation);
			break;
		}
		//カメラポインタのセット
		player[i].setCamera(&camera[i]);	

		//モデルの設定
		switch (player[i].getInfomation()->modelType)
		{
		case gameMasterNS::MODEL_MALE:
			maleRenderer->registerObject(&player[i]);
			break;
		case gameMasterNS::MODEL_FEMALE:
			femaleRenderer->registerObject(&player[i]);
			break;
		}

		//重力を設定
		player[i].configurationGravityWithRay(
			testField->getPosition(), 
			testFieldRenderer->getStaticMesh()->mesh, 
			testField->getMatrixWorld());	
	}

	//エフェクシアーの設定
	effekseerNS::setProjectionMatrix(
		camera[0].fieldOfView, 
		camera[0].windowWidth, 
		camera[0].windowHeight, 
		camera[0].nearZ, 
		camera[0].farZ);

	//light
	light = new Light;
	light->initialize();


	//枯木の初期化
	deadTree = new DeadTree();
	//木Aの初期化
	treeA = new TreeTypeA();
	//木Bの初期化
	treeB = new TreeTypeB();
	//石の初期化
	stone = new Stone();
	//スカイドームの初期化
	sky = new Sky();

	//アニメションキャラの初期化
	InitMoveP(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.04f, 0.04f, 0.04f), true);


	// サウンドの再生
	//sound->play(soundNS::TYPE::BGM_GAME, soundNS::METHOD::LOOP);

	//テキストの初期化
	//text.initialize(direct3D9->device,10,10, 0xff00ff00);
	//text2.initialize(direct3D9->device,11,11, 0xff0000ff);

	//エフェクト（インスタンシング）テスト
	testEffect = new TestEffect();

	//-----中込テストゾーンその2---------------
	//enemy->setDebugEnvironment();
	//enemy->setCamera(camera);	//カメラのセット
	//enemy->configurationGravityWithRay(testField->getPosition(), testField->getStaticMesh()->mesh, testField->getMatrixWorld());	//重力を設定
	//-----------------------------------------

	// エネミー
	enemyManager = new EnemyManager;
	enemyManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// ツリー
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// アイテム
	itemManager = new ItemManager;
	itemManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// テロップ
	telop = new Telop;
	telop->initialize();

	// AI
	aiDirector = new AIDirector;
	aiDirector->initialize();
	naviAI = new NavigationMesh(staticMeshNS::reference(staticMeshNS::SAMPLE_NAVMESH));
	naviAI->initialize();

	//Sprite実験
	spriteGauge = new SpriteGauge;
	spriteGauge->initialize();

}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Game::uninitialize() {
	SAFE_DELETE_ARRAY(player);
	SAFE_DELETE_ARRAY(camera);
	SAFE_DELETE(light);
	SAFE_DELETE(testField);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(maleRenderer);
	SAFE_DELETE(femaleRenderer);
	SAFE_DELETE(deadTree);
	SAFE_DELETE(treeA);
	SAFE_DELETE(treeB);
	SAFE_DELETE(stone);
	SAFE_DELETE(sky);
	SAFE_DELETE(testEffect);
	SAFE_DELETE(enemyManager);
	SAFE_DELETE(treeManager);
	SAFE_DELETE(itemManager);
	SAFE_DELETE(telop);
	SAFE_DELETE(aiDirector);
	SAFE_DELETE(spriteGauge);

	UninitMoveP();

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Game::update(float _frameTime) {

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//【処理落ち】
	//フレーム時間が約10FPS時の時の時間より長い場合は、処理落ち（更新しない）
	//※フレーム時間に準拠している処理が正常に機能しないため
	//if (frameTime > 0.10)return;


	//テストフィールドの更新
	testField->update();			//オブジェクト
	testFieldRenderer->update();	//レンダラー

	//プレイヤーの更新
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		player[i].update(frameTime);		//オブジェクト
	maleRenderer->update();					//レンダラー
	femaleRenderer->update();				//レンダラー

	// エネミーの更新
	enemyManager->update(frameTime);

	// ツリーの更新
	treeManager->update(frameTime);

	// アイテムの更新
	itemManager->update(frameTime);

	UpdateMoveP(0.01f);

	//エフェクシアーのテスト
#pragma region EffekseerTest
	//エフェクトの再生
	if (input->wasKeyPressed('1'))
	{
		effekseerNS::Instance* instance = new effekseerNS::Instance();
		instance->position = *player[gameMasterNS::PLAYER_1P].getPosition();
		effekseerNS::play(instance);
	}
	if (input->wasKeyPressed('2'))
	{
		class Fire :public effekseerNS::Instance
		{
		public:
			D3DXVECTOR3* syncPosition;
			Fire() { 
				effectNo = effekseerNS::TEST0;
				deltaRadian = D3DXVECTOR3(0, 0.3, 0);
			}
			virtual void update() {
				position = *syncPosition;

				Instance::update();
			};
		};
		Fire* instance = new Fire;
		instance->position = *player[gameMasterNS::PLAYER_1P].getPosition();
		instance->syncPosition = player[gameMasterNS::PLAYER_1P].getPosition();
		effekseerNS::play(instance);
	}
	//エフェクトの一時停止：再生
	if (input->wasKeyPressed('3'))
	{
		effekseerNS::pause(false);
	}
	//エフェクトの一時停止：停止
	if (input->isKeyDown('4'))
	{
		effekseerNS::pause(true);
	}
	//エフェクトの停止
	if (input->wasKeyPressed('G'))
	{
		//effekseerNS::stop((*getEffekseerManager()->instanceList->getValue(0))->handle);
	}
#pragma endregion


	// テロップの更新
	telop->update(frameTime);
	
	//枯木の更新
	deadTree->update();
	//木Aの更新
	treeA->update();
	//木Bの更新
	treeB->update();
	//石の更新
	stone->update();
	//スカイドームの更新
	sky->update();

	//エフェクト（インスタンシング）テスト
	testEffect->update(frameTime);

	//電力減少（電力回復確認用）
	player->pullpower(1);

	//カメラの更新
	for(int i = 0;i<gameMasterNS::PLAYER_NUM;i++)
		camera[i].update();

	//sound->updateSound(*player->getPosition(), player->getAxisZ()->direction);

	// Enterまたは〇ボタンでリザルトへ
	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		// サウンドの再生
		//sound->play(soundNS::TYPE::SE_DECISION, soundNS::METHOD::PLAY);
		// シーン遷移
		changeScene(nextScene);
	}

#ifdef _DEBUG
	test();
#endif
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Game::render() {	
		
	//1Pカメラ・ウィンドウ・エフェクシアーマネージャー
	camera[gameMasterNS::PLAYER_1P].renderReady();
	direct3D9->changeViewport1PWindow();
	render3D(camera[gameMasterNS::PLAYER_1P]);
	effekseerNS::setCameraMatrix(
		camera[gameMasterNS::PLAYER_1P].position, 
		camera[gameMasterNS::PLAYER_1P].gazePosition, 
		camera[gameMasterNS::PLAYER_1P].upVector);
	effekseerNS::render();

	//2Pカメラ・ウィンドウ・エフェクシアーマネージャー
	camera[gameMasterNS::PLAYER_2P].renderReady();
	direct3D9->changeViewport2PWindow();
	render3D(camera[gameMasterNS::PLAYER_2P]);
	effekseerNS::setCameraMatrix(
		camera[gameMasterNS::PLAYER_2P].position,
		camera[gameMasterNS::PLAYER_2P].gazePosition,
		camera[gameMasterNS::PLAYER_2P].upVector);
	effekseerNS::render();

	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();
}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
void Game::render3D(Camera currentCamera) {

	//テストフィールドの描画
	testField->setAlpha(0.1f); 
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	// プレイヤーの描画
	maleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	femaleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	// プレイヤーの他のオブジェクトの描画
	for(int i = 0;i<gameMasterNS::PLAYER_NUM;i++)
		player[i].otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//木の描画
	deadTree->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//木Aの描画
	treeA->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//木Bの描画
	treeB->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//石の描画
	stone->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//スカイドームの描画
	sky->render(currentCamera.view, currentCamera.projection, currentCamera.position);


	
	DrawMoveP();

	// エネミーの描画
	enemyManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//enemy->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// ツリーの描画
	treeManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// アイテムの描画
	itemManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	//エフェクト（インスタンシング）テスト
	testEffect->render(currentCamera.view, currentCamera.projection, currentCamera.position);

#ifdef DEBUG_NAVIMESH
	// ナビゲーションメッシュの描画
	naviAI->debugRender(currentCamera.view, currentCamera.projection, currentCamera.position);
#endif
}

//===================================================================================================================================
//【UI/2D描画】
//===================================================================================================================================
void Game::renderUI() {

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定

	//device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//Sprite実験
	spriteGauge->render();
	
	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	telop->render();	// テロップの描画
}

//===================================================================================================================================
//【衝突判定処理】
//===================================================================================================================================
void Game::collisions() 
{
	// プレイヤーとアイテム
	std::vector<Item*> itemList = itemManager->getItemList();
	for (size_t i = 0; i < itemList.size(); i++)
	{	
		for (int j = 0; j < gameMasterNS::PLAYER_NUM; j++)
		{
			if (itemList[i]->sphereCollider.collide(player[j].getBodyCollide()->getCenter(),
				player[j].getRadius(), *itemList[i]->getMatrixWorld(), *player[j].getMatrixWorld()))
			{
				player[j].addSpeed(D3DXVECTOR3(0, 10, 0));
				player[j].addpower(batteryNS::RECOVERY_POWER);	//電力加算
				SoundInterface::playSound(playParameters[3]);	//SE再生
				itemManager->destroyAllItem();					//デリート(今は全消し)
			}
		}
	}
}

//===================================================================================================================================
//【AI処理】
//===================================================================================================================================
void Game::AI() {
	aiDirector->run();		// メタAI実行
}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
#ifdef _DEBUG
void Game::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("node:%d", testEffect->getList().nodeNum);

	player->outputGUI();			//プレイヤー
	//enemy->outputGUI();			//エネミー
	itemManager->outputGUI();		// アイテムマネージャ
	testField->outputGUI();			//テストフィールド
	camera->outputGUI();			//カメラ
	naviAI->outputGUI();			//ナビゲーションAI

}
#endif // _DEBUG


//===================================================================================================================================
//【なんかいろいろテストするところ】
//===================================================================================================================================
void Game::test()
{
	// アイテムマネージャのテスト
	if (input->wasKeyPressed('0'))
	{
		itemNS::ItemData unko = { itemManager->issueNewItemID(), itemNS::BATTERY, *player->getPosition() };
		itemManager->createItem(unko);
	}
	// 3Dモデル表示確認用（アイテムの更新）
	if (input->wasKeyPressed('P'))
	{
		itemNS::ItemData abc = { 1, itemNS::EXAMPLE, *player->getPosition() };
		itemManager->createItem(abc);
	}

	if (input->wasKeyPressed('9'))
	{
		itemManager->destroyAllItem();
		//itemManager->destroyItem(3);
	}

	//エネミーマネージャのテスト
	if (input->wasKeyPressed('8'))	// 作成
	{
		enemyNS::ENEMYSET tinko =
		{
			enemyManager->issueNewEnemyID(),
			enemyNS::WOLF,
			enemyNS::CHASE,
			*player->getPosition(),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		};
		enemyNS::EnemyData* p = enemyManager->createEnemyData(tinko);
		enemyManager->createEnemy(p);
	}

	if (input->wasKeyPressed('7'))	// 全破棄
	{
		enemyManager->destroyAllEnemy();
		enemyManager->destroyAllEnemyData();
	}
	if (input->wasKeyPressed('6'))	// 0-50（ID）までランダムに破棄
	{
		//enemyManager->destroyEnemy(5);
		//enemyManager->destroyEnemyData(5);

		static bool rec[50] = { false };
		for (int i = 0; i < 50; i++)
		{
			int n = rand() % 50;
			if (rec[n] == false)
			{
				rec[n] = true;
				enemyManager->destroyEnemy(n);
				enemyManager->destroyEnemyData(n);
				break;
			}
		}
	}

	//enemyNS::ENEMYSET temp =
	//{
	//	enemyManager->issueNewEnemyID(),
	//	enemyNS::WOLF,
	//	enemyNS::CHASE,
	//	D3DXVECTOR3(),
	//	D3DXVECTOR3(0.0f, 0.0f, 0.0f)
	//};




	// ツリーマネージャのテスト
	if (input->wasKeyPressed('5'))	// 作成
	{
		treeNS::TreeData treeData;
		treeData.geenState = treeNS::GREEN;
		treeData.model = treeNS::A_MODEL;
		treeData.initialPosition = *player->getPosition();
		treeManager->createTree(treeData);
	}
	if (input->wasKeyPressed('4'))	// リーフ3番のみ描画を切る
	{
		treeManager->unRegisterLeafRendering(treeManager->getTreeList()[3]->getLeaf(),
			treeManager->getTreeList()[3]->getTreeData()->model);
	}	
	if (input->wasKeyPressed('3'))	// 全破棄
	{
		treeManager->destroyAllTree();
	}
}