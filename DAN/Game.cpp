//===================================================================================================================================
//【Game.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/10/18
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Game.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace gameNS;
//using namespace playerNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Game::Game()
{
	sceneName = "Scene -Game-";

	nextScene = SceneList::RESULT;

	//シーンの更新
	SoundInterface::SwitchAudioBuffer(SceneList::GAME);

	//再生パラメータ
	memset(playParameters, 0, sizeof(playParameters));
	XAUDIO2_FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, GAME_SE_LIST::GAME_SE_01, false ,NULL,true, filterParameters };
	playParameters[1] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, GAME_SE_LIST::GAME_SE_02, false ,NULL,true, filterParameters };
	playParameters[2] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, GAME_BGM_LIST::GAME_BGM_01, true,1.0f,true, filterParameters };
	playParameters[3] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, GAME_SE_LIST::GAME_SE_01, false ,NULL,true, filterParameters };		//アイテム取得音

	//再生
	SoundInterface::playSound(playParameters[0]);
	SoundInterface::playSound(playParameters[1]);
	SoundInterface::playSound(playParameters[2]);

	//BGMManager::startTime = frameTime;
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

	//player
	player = new Player;

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

	//camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH/2, WINDOW_HEIGHT);
	camera->setTarget(player->getPosition());
	camera->setTargetX(&player->getAxisX()->direction);
	camera->setTargetY(&player->getAxisY()->direction);
	camera->setTargetZ(&player->getAxisZ()->direction);
	camera->setRelative(CAMERA_RELATIVE_QUATERNION);
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setRelativeGaze(CAMERA_RELATIVE_GAZE);
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView( (D3DX_PI/18) * 9 );

	//light
	light = new Light;
	light->initialize();

	//テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::YAMADA_TEST_ZONE));
	testFieldRenderer->generateObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//プレイヤーの初期化
	player->initialize(inputNS::DINPUT_1P, 0);
	player->setCamera(camera);	//カメラポインタのセット
	playerRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2));
	playerRenderer->generateObject(player);
	player->configurationGravityWithRay(testField->getPosition(), testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());	//重力を設定

	//枯木の初期化
	deadTree = new DeadTree();
	//木Aの初期化
	treeA = new TreeTypeA();
	//木Bの初期化
	treeB = new TreeTypeB();
	//石の初期化
	stone = new Stone();

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

	// ツリー
	treeManager = new TreeManager;
	treeManager->initialize();

	// アイテム
	itemManager = new ItemManager;
	itemManager->initialize();

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
	SAFE_DELETE(camera);
	SAFE_DELETE(light);
	SAFE_DELETE(testField);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(player);
	SAFE_DELETE(playerRenderer);
	SAFE_DELETE(deadTree);
	SAFE_DELETE(treeA);
	SAFE_DELETE(treeB);
	SAFE_DELETE(stone);
	SAFE_DELETE(testEffect);
	SAFE_DELETE(treeManager);
	SAFE_DELETE(itemManager);
	SAFE_DELETE(telop);
	SAFE_DELETE(aiDirector);
	SAFE_DELETE(spriteGauge);
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
	if (frameTime > 0.10)return;

	//エフェクト（インスタンシング）テスト
	testEffect->update(frameTime);

	//テストフィールドの更新
	testField->update();
	testFieldRenderer->update();

	//プレイヤーの更新
	player->update(frameTime);

	// エネミーの更新
	//enemy->update(frameTime);

	// ツリーの更新
	treeManager->update(frameTime);

	// アイテムの更新
	if (input->wasKeyPressed('0'))
	{
		itemNS::ItemData unko = { 0, itemNS::BATTERY, *player->getPosition() };
		itemManager->createItem(&unko);
		
	}
	if (input->wasKeyPressed('9'))
	{
		itemManager->destroyAllItem();
	}
	// 3Dモデル表示確認用（アイテムの更新）
	if (input->wasKeyPressed('8'))
	{
		itemNS::ItemData abc = { 1, itemNS::EXAMPLE, *player->getPosition() };
		itemManager->createItem(&abc);
	}
	itemManager->update(frameTime);

	// テロップの更新
	telop->update(frameTime);
	
	////カメラの更新
	//camera->update();
	playerRenderer->update();

	//枯木の更新
	deadTree->update();
	//木Aの更新
	treeA->update();
	//木Bの更新
	treeB->update();
	//石の更新
	stone->update();

	//カメラの更新
	camera->update();

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

}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Game::render() {	
		
	//1Pカメラ・ウィンドウ
	camera->renderReady();
	direct3D9->changeViewport1PWindow();
	render3D(*camera);

	//2Pカメラ・ウィンドウ
	camera->renderReady();
	direct3D9->changeViewport2PWindow();
	render3D(*camera);

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

	//エフェクト（インスタンシング）テスト
	testEffect->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// プレイヤーの描画
	playerRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	// プレイヤーの他のオブジェクトの描画
	player->otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//木の描画
	deadTree->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//木Aの描画
	treeA->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//木Bの描画
	treeB->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//石の描画
	stone->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// エネミーの描画
	//enemy->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// ツリーの描画
	treeManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// アイテムの描画
	itemManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);
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
	std::vector<Item*> itemList = itemManager->getList();
	for (size_t i = 0; i < itemList.size(); i++)
	{	
		if (itemList[i]->sphereCollider.collide(player->getBodyCollide()->getCenter(),
			player->getRadius(), *itemList[i]->getMatrixWorld(), *player->getMatrixWorld()))
		{
			//itemManager->destroyItem();
			player->addSpeed(D3DXVECTOR3(0, 10, 0));
			player->addpower(playerNS::RECOVERY_POWER);				//電力加算
			SoundInterface::playSound(playParameters[3]);	//SE再生
			itemManager->destroyAllItem();					//デリート(今は全消し)
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
