//===================================================================================================================================
//【Game.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/10/04
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
	sceneName = "Scene -Game-";

	nextScene = SceneList::RESULT;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Game::~Game()
{
	// サウンドの停止
	//sound->stop(soundNS::TYPE::BGM_GAME);
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
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera->setTarget(player->getPosition());
	camera->setTargetX(&player->getAxisX()->direction);
	camera->setTargetY(&player->getAxisY()->direction);
	camera->setTargetZ(&player->getAxisZ()->direction);
	camera->setRelative(CAMERA_RELATIVE_QUATERNION);
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setRelativeGaze(CAMERA_RELATIVE_GAZE);
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView( (D3DX_PI/18) * 10 );

	//light
	light = new Light;
	light->initialize();

	//テストフィールド
	testField = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::YAMADA_TEST_ZONE));
	testField->initialize(&D3DXVECTOR3(0, 0, 0));
	testField->activation();

	// サウンドの再生
	//sound->play(soundNS::TYPE::BGM_GAME, soundNS::METHOD::LOOP);

	//プレイヤーの初期化
	player->initialize(inputNS::DINPUT_1P, 0);
	player->setCamera(camera);	//カメラのセット
	player->configurationGravityWithRay(testField->getPosition(),testField->getStaticMesh()->mesh,testField->getMatrixWorld());	//重力を設定
	

	//テキストの初期化
	//text.initialize(direct3D9->device,10,10, 0xff00ff00);
	//text2.initialize(direct3D9->device,11,11, 0xff0000ff);

	//ポイントスプライト
	//pointSprite.initilaize(direct3D9->device);

	//インスタンシングビルボードテスト
	instancingBillboardTest.initialize(
		*shaderNS::reference(shaderNS::INSTANCE_BILLBOARD),
		*textureNS::reference(textureNS::SAMPLE_TREE));

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
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Game::uninitialize() {
	SAFE_DELETE(camera);
	SAFE_DELETE(light);
	SAFE_DELETE(testField);
	SAFE_DELETE(player);
	SAFE_DELETE(treeManager);
	SAFE_DELETE(itemManager);
	SAFE_DELETE(telop);
	SAFE_DELETE(aiDirector);
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

	//インスタンシングビルボードテスト
	instancingBillboardTest.update(frameTime);

	//テストフィールドの更新
	testField->update();

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
		itemManager->uninitialize();
	}
	itemManager->update(frameTime);

	// テロップの更新
	telop->update(frameTime);
	
	////カメラの更新
	//camera->update();

	//3Dサウンド
	//プレイヤーの位置と向き
	
	//sound->updateSound(*player->getPosition(), player->getAxisZ()->direction);
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Game::render() {

	//1Pカメラ・ウィンドウ
	camera->renderReady();
	direct3D9->changeViewportFullWindow();
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
	testField->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	//（サンプル）ポイントスプライトの描画
	//pointSprite.render(direct3D9->device, currentCamera.position);

	//（サンプル）プレーンの描画(インスタンシング)
	instancingBillboardTest.render(currentCamera.view, currentCamera.projection, currentCamera.position);

	//xFileStaticMeshテスト描画
	//testObject.multipleRender(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position,
	//	*shaderLoader->getEffect(shaderNS::INSTANCE_STATIC_MESH));
	//testCube.multipleRender(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position,
	//	*shaderLoader->getEffect(shaderNS::INSTANCE_STATIC_MESH));

	// プレイヤーの描画
	player->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	// プレイヤーの他のオブジェクトの描画
	player->otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	// エネミーの描画
	//enemy->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// ツリーの描画
	treeManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// アイテムの描画
	itemManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

#ifdef DEBUG_NAVIMESH
	// ナビゲーションメッシュの描画
	naviAI->debugRender();
#endif
}

//===================================================================================================================================
//【UI/2D描画】
//===================================================================================================================================
void Game::renderUI() {

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

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
			player->addSpeed(D3DXVECTOR3(0, 10, 0));
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
	ImGui::Text("node:%d", instancingBillboardTest.getList().nodeNum);

	player->outputGUI();			//プレイヤー
	//enemy->outputGUI();			//エネミー
	itemManager->outputGUI();		// アイテムマネージャ
	testField->outputGUI();			//テストフィールド
	camera->outputGUI();			//カメラ
	naviAI->outputGUI();			//ナビゲーションAI

}
#endif // _DEBUG
