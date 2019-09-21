//===================================================================================================================================
//【Game.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
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

	// サウンドの再生
	//sound->play(soundNS::TYPE::BGM_GAME, soundNS::METHOD::LOOP);

	//player
	//player[0] = new Player;
	//player[1] = new Player;

	//camera
	//camera = new Camera[NUM_PLAYER];
	//for (int i = 0; i < NUM_PLAYER; i++)
	//{
	//	camera[i].initialize(WINDOW_WIDTH/2, WINDOW_HEIGHT);
	//	camera[i].setTarget(player[i]->getPosition());
	//	camera[i].setTargetX(&player[i]->getAxisX()->direction);
	//	camera[i].setTargetY(&player[i]->getAxisY()->direction);
	//	camera[i].setTargetZ(&player[i]->getAxisZ()->direction);
	//	camera[i].setRelative(CAMERA_RELATIVE_QUATERNION[i]);
	//	camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
	//	camera[i].setRelativeGaze(D3DXVECTOR3(0, 10, 0));
	//	camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
	//	camera[i].setFieldOfView(D3DX_PI / 2.5);
	//}

	//light
	//light = new Light;
	//light->initialize(direct3D9);

	//for (int i = 0; i < NUM_PLAYER; i++)
	//{//プレイヤーの初期化
		//player[i]->initialize(i, gameMaster->getPlayerInfomation()[i].modelType, direct3D9->device, staticMeshLoader, textureLoader, shaderLoader);
		//player[i]->setInput(input);			//入力クラスのセット
		//player[i]->setAnimationModel(animationLoader->getAnimationModel(i, gameMaster->getPlayerInfomation()[i].modelType));
		//player[i]->setCamera(&camera[i]);	//カメラのセット
		//player[i]->setSound(sound);			//サウンドのセット
		//player[i]->configurationGravity(field.getPosition(),field.getRadius());	//重力を作成
		//player[i]->animationPlayer->setAnimationConfiguration(animationPlayerNS::SCENE_TYPE::GAME);
		//player[i]->animationPlayer->resetAnimation();
	//}

	//テキストの初期化
	//text.initialize(direct3D9->device,10,10, 0xff00ff00);
	//text2.initialize(direct3D9->device,11,11, 0xff0000ff);

	//ポイントスプライト
	//pointSprite.initilaize(direct3D9->device);

	//インスタンスプレーン
	//plane.createPositionSpherical(direct3D9->device, 3000, 250.0f);
	//plane.initialize(direct3D9->device, *shaderLoader->getEffect(shaderNS::INSTANCE_BILLBOARD), *textureLoader->getTexture(textureLoaderNS::BACKGROUND_DUST));

	//xFile読込meshのインスタンシング描画のテスト
	//D3DXVECTOR3 positionList[] =
	//{
	//	D3DXVECTOR3(100,50,50),
	//	D3DXVECTOR3(-100,-100,-50),
	//	D3DXVECTOR3(100,-50,10),
	//	D3DXVECTOR3(100,50,10),
	//	D3DXVECTOR3(100,0,100),
	//	D3DXVECTOR3(0,100,0),
	//	D3DXVECTOR3(-100,100,100),
	//	D3DXVECTOR3(-100,-100,100),
	//	D3DXVECTOR3(-100,100,0),
	//	D3DXVECTOR3(-100,100,-100),
	//	D3DXVECTOR3(100,50,50),
	//	D3DXVECTOR3(-100,-100,-50),
	//	D3DXVECTOR3(100,-50,10),
	//	D3DXVECTOR3(100,50,10),
	//	D3DXVECTOR3(100,0,100),
	//	D3DXVECTOR3(0,100,0),
	//	D3DXVECTOR3(-100,100,100),
	//	D3DXVECTOR3(-100,-100,100),
	//	D3DXVECTOR3(-100,100,0),
	//	D3DXVECTOR3(-100,100,-100),
	//};
	//testObject.initialize(direct3D9->device, &staticMeshLoader->staticMesh[staticMeshNS::STAR_REGULAR_POLYHEDRON_X10], &D3DXVECTOR3(0, 0, 0));
	//testObject.setNumOfRender(direct3D9->device, 20, positionList);
	//testObject.activation();

	//D3DXVECTOR3 cubeList[NUM_CUBE];
	//for (int i = 0; i < NUM_CUBE; i++)
	//{
	//	cubeList[i] = D3DXVECTOR3((float)(rand() % 1000 - 500), (float)(rand() % 1000 - 500), (float)(rand() % 1000 - 500));
	//}
	//testCube.initialize(direct3D9->device, &staticMeshLoader->staticMesh[staticMeshNS::CUBE], &D3DXVECTOR3(0, 0, 0));
	//testCube.setNumOfRender(direct3D9->device, NUM_CUBE, cubeList);
	//testCube.activation();

	//ゲームマスター
	//gameMaster->gameStart();//ゲーム開始時処理
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Game::uninitialize() {
	//SAFE_DELETE(light);
	//SAFE_DELETE_ARRAY(camera);
	//SAFE_DELETE(player[0])
	//SAFE_DELETE(player[1])
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Game::update(float _frameTime) {

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//ゲームが開始した場合ポーズが有効
	//if (gameMaster->whetherAlreadyStart())
	//{
	//	if (uiPause.getRenderFlag())
	//	{
	//		if (input->wasKeyPressed('P') ||
	//			input->getController()[PLAYER1]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
	//			input->getController()[PLAYER2]->wasButton(virtualControllerNS::SPECIAL_MAIN)
	//			)// ポーズ解除
	//		{
	//			// サウンドの再生
	//			sound->play(soundNS::TYPE::SE_PAUSE, soundNS::METHOD::PLAY);
	//			uiPause.setRenderFlag(false);
	//		}
	//	}
	//	else if (!uiPause.getRenderFlag())
	//	{
	//		if (input->wasKeyPressed('P') ||
	//			input->getController()[PLAYER1]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
	//			input->getController()[PLAYER2]->wasButton(virtualControllerNS::SPECIAL_MAIN)
	//			)// ポーズ解除
	//		{
	//			// サウンドの再生
	//			sound->play(soundNS::TYPE::SE_PAUSE, soundNS::METHOD::PLAY);
	//			uiPause.setRenderFlag(true);
	//		}
	//	}
	//	if (uiPause.getRenderFlag())return;// ポーズしてたら更新しない
	//}

	//【処理落ち】
	//フレーム時間が約10FPS時の時の時間より長い場合は、処理落ち（更新しない）
	//※フレーム時間に準拠している処理が正常に機能しないため
	if (frameTime > 0.10)return;

	//【ゲームマスターの更新】
	//gameMaster->update(frameTime);
	//ゲームオーバー時処理
	//if (gameMaster->whetherGameOver())
	//{//シーン切替
	//	for (int i = 0; i < playerNS::NUM_PLAYER; i++)
	//	{
	//		gameMaster->setWage(i, player[i]->getWage());
	//	}
	//	changeScene(SceneList::RESULT);
	//}

	//【プレイヤーの更新】
	//for (int i = 0; i < NUM_PLAYER; i++)
	//{
	//	player[i]->update(frameTime);
	//}

	//D3DXCOLOR* colorList = new D3DXCOLOR[3000];
	//float sinValue = (sinf(gameMaster->getGameTime() * 4)+1.0f)/2.0f;
	//float cosValue = cosf(gameMaster->getGameTime() * 4);
	//for (int i = 0; i < 3000; i++)
	//{
	//	colorList[i] = D3DXCOLOR(sinValue,sinValue,sinValue,sinValue);
	//}
	//plane.setColorBuffer(direct3D9->device, 3000, colorList);
	//SAFE_DELETE_ARRAY(colorList);
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Game::render() {

	//1Pカメラ・ウィンドウ
	//device->SetTransform(D3DTS_VIEW, &camera[PLAYER1].view);
	//device->SetTransform(D3DTS_PROJECTION, &camera[PLAYER1].projection);
	direct3D9->changeViewport1PWindow();
	//render3D(camera[PLAYER1]);

	//2Pカメラ・ウィンドウ
	//device->SetTransform(D3DTS_VIEW, &camera[PLAYER2].view);
	//device->SetTransform(D3DTS_PROJECTION, &camera[PLAYER2].projection);
	direct3D9->changeViewport2PWindow();
	//render3D(camera[PLAYER2]);

	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();

}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
//void Game::render3D(Camera currentCamera) {

	//（サンプル）ポイントスプライトの描画
	//pointSprite.render(direct3D9->device, currentCamera.position);

	//（サンプル）プレーンの描画(インスタンシング)
	//plane.render(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position);

	//xFileStaticMeshテスト描画
	//testObject.multipleRender(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position,
	//	*shaderLoader->getEffect(shaderNS::INSTANCE_STATIC_MESH));
	//testCube.multipleRender(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position,
	//	*shaderLoader->getEffect(shaderNS::INSTANCE_STATIC_MESH));


	//for (int i = 0; i < NUM_PLAYER; i++)
	//{
	//	// プレイヤーの描画
	//	player[i]->toonRender(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position, *shaderLoader->getEffect(shaderNS::TOON), *textureLoader->getTexture(textureLoaderNS::TOON_SHADE), *textureLoader->getTexture(textureLoaderNS::TOON_OUT_LINE));
	//	// プレイヤーの他のオブジェクトの描画
	//	player[i]->otherRender(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position);
	//}
//}

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
}

//===================================================================================================================================
//【衝突判定処理】
//===================================================================================================================================
void Game::collisions() 
{

}

//===================================================================================================================================
//【AI処理】
//===================================================================================================================================
void Game::AI() {
#ifdef USING_AI

#endif// USING_AI
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
}
#endif // _DEBUG
