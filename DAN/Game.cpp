//===================================================================================================================================
//【Game.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/11/19
//===================================================================================================================================

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ※対応後に決してください（by 中込）
// プレイヤーの初期位置は、プレイヤー間で重複なくランダムに4か所から選定されます。
// ①南の平野(-43, 8, -236), ②西の岬(-265, 54, 26), ③北の平野(142, 13, 246), ④東の狭い所(311, 13, -65)
// 伊達山方向を正面に、各y座標より10m上にスポーンします。リスポーンのためにその回の初期位置を記録しておく
// 必要があります。
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Game.h"
#include "CollisionManager.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace gameNS;


//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Game::Game()
{
	//線形４分木空間分割管理クラス
	//linear4TreeManager = new Linear4TreeManager<Object>;
	//linear4TreeManager->initialize(5, -1000, 1000, 1000, -1000);	
	//線形８分木空間分割管理クラス
	linear8TreeManager = new Linear8TreeManager<Object>;
	linear8TreeManager->initialize(5, D3DXVECTOR3(-3000,-2000,-3000),D3DXVECTOR3(3000,3000,3000));	

	//オブジェクトカウンターのリセット
	objectNS::resetCounter();		

	sceneName = "Scene -Game-";

	nextScene = SceneList::RESULT;

	//再生パラメータ
	PLAY_PARAMETERS playParameters[2];
	memset(playParameters, 0, sizeof(playParameters));
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Decision, false ,NULL,false,NULL};
	playParameters[1] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Game, true,1.1f,false,NULL };
	
	//再生
	SoundInterface::SE->playSound(&playParameters[0]);
	SoundInterface::BGM->playSound(&playParameters[1]);
	
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Game::~Game()
{
	// サウンドの停止
	SoundInterface::BGM->uninitSoundStop();
}

//#define SAMPLE_NAVI	// ビルドスイッチ　このdefine周辺は近々で消しますが一旦残しておいてもらえると助かります中込
//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Game::initialize() {

	//テストフィールド
	testField = new Object();
#ifdef SAMPLE_NAVI
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_NAVMESH));
#else
	//testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_NAVIMESH));
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
#endif
	testFieldRenderer->registerObject(testField);
	testFieldRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//player
	player				= new Player[gameMasterNS::PLAYER_NUM];
	maleRenderer		= new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_MALE));
	femaleRenderer		= new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_FEMALE));

	//オープニングカメラ
	cameraOP = new Camera;
	//カメラの設定
	cameraOP->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	cameraOP->setTarget(&testField->position);
	cameraOP->setRelative(D3DXVECTOR3(1.5f,3.0f,-3.0f));
	cameraOP->setGazeDistance(500.0f);
	cameraOP->setGaze(D3DXVECTOR3(0, 0, 0));
	cameraOP->setUpVector(D3DXVECTOR3(0, 1, 0));
	cameraOP->setFieldOfView((D3DX_PI / 180) * 91);
	cameraOP->setLimitRotationTop(0.1f);
	cameraOP->setLimitRotationBottom(0.1f);


	//camera
	camera = new Camera[gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//カメラの設定
		camera[i].initialize(WINDOW_WIDTH / 2,  WINDOW_HEIGHT);
		camera[i].setTarget(player[i].getCameraGaze());
		camera[i].setTargetX(&player[i].getAxisX()->direction);
		camera[i].setTargetY(&player[i].getAxisY()->direction);
		camera[i].setTargetZ(&player[i].getAxisZ()->direction);
		camera[i].setRelative(CAMERA_RELATIVE_QUATERNION);
		camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
		camera[i].setRelativeGaze(CAMERA_RELATIVE_GAZE);
		camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
		camera[i].setFieldOfView( (D3DX_PI/180) * 91 );
		camera[i].setLimitRotationTop(0.1f);
		camera[i].setLimitRotationBottom(0.3f);

		//プレイヤーの設定
		PlayerTable infomation;
		switch (i)
		{
		case gameMasterNS::PLAYER_1P:
			infomation.playerType = gameMasterNS::PLAYER_1P;
			infomation.modelType = gameMasterNS::MODEL_FEMALE;
			player[i].initialize(infomation);
			break;
		case gameMasterNS::PLAYER_2P:
			infomation.playerType	= gameMasterNS::PLAYER_2P;
			infomation.modelType	= gameMasterNS::MODEL_FEMALE;
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

		//エフェクシアーの設定
		effekseerNS::setProjectionMatrix(i+1,
			camera[i].fieldOfView,
			(float)camera[i].windowWidth,
			(float)camera[i].windowHeight,
			camera[i].nearZ,
			camera[i].farZ);
	}

	//エフェクシアーの設定
	effekseerNS::setProjectionMatrix(0,
		camera[0].fieldOfView,
		(float)camera[0].windowWidth,
		(float)camera[0].windowHeight,
		camera[0].nearZ,
		camera[0].farZ);

	//light
	light = new Light;
	light->initialize();


	//スカイドームの初期化
	sky = new Sky();
	//海面の初期化
	ocean = new Ocean();

	//アニメションキャラの初期化
	InitMoveP(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.003f, 0.003f, 0.003f), true);
	InitMoveP1(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.003f, 0.003f, 0.003f), true);


	// サウンドの再生
	//sound->play(soundNS::TYPE::BGM_GAME, soundNS::METHOD::LOOP);

	//テキストの初期化
	//text.initialize(direct3D9->device,10,10, 0xff00ff00);
	//text2.initialize(direct3D9->device,11,11, 0xff0000ff);

	//エフェクト（インスタンシング）テスト
	//testEffect = new TestEffect();

	//ディスプレイ用プレーンサンプル
	samplePlane = new TestPlane();
	//開発中広告
	//ad = new Advertisement();

	// ナビゲーションAI（ナビゲーションAIはエネミー関係クラスより先に初期化する）
#ifdef SAMPLE_NAVI
	naviMesh = new NavigationMesh(staticMeshNS::reference(staticMeshNS::SAMPLE_NAVMESH));
#else
	naviMesh = new NavigationMesh(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_NAVIMESH));
#endif
	naviMesh->initialize();

	// エネミー
	enemyManager = new EnemyManager;
	enemyManager->initialize(*getSceneName(),testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld(), gameMaster, player);
	
	// ツリー
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	treeManager->setGameMaster(gameMaster);

	// アイテム
	itemManager = new ItemManager;
	itemManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// 風
	windManager = new WindManager;
	windManager->initialize(*getSceneName(), testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// マップオブジェクト
	mapObjectManager = new MapObjectManager;
	mapObjectManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());


	//テロップマネージャー
	telopManager = new TelopManager;
	telopManager->initialize();

	//固定されたUI
	fixedUI = new FixedUI;
	fixedUI->initialize();

	//プレイヤー1周りのUI
	player1UI = new Player1UI;
	player1UI->initialize(&player[gameMasterNS::PLAYER_1P]);

	//プレイヤー２周りのUI
	player2UI = new Player2UI;
	player2UI->initialize(&player[gameMasterNS::PLAYER_2P]);

	//カウントUI
	countUI = new CountUI();

	//レティクル
	reticle = new Reticle();
	reticle->setAimingPosition1(player[gameMasterNS::PLAYER_1P].getAiming());
	reticle->setAimingPosition2(player[gameMasterNS::PLAYER_2P].getAiming());

	//Sprite実験
	spriteGauge = new SpriteGauge;
	spriteGauge->initialize();

	//Network
	networkClient = new NETWORK_CLIENT;

#pragma region Memory Test
	////メモリテスト

	//treeManager->uninitialize();
	//SAFE_DELETE(treeManager);

	//int i = 100000;
	//while (i >= 0)
	//{
	//	/*testFieldRenderer2 = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
	//	testFieldRenderer2->registerObject(testField);
	//	testFieldRenderer2->update();
	//	SAFE_DELETE(testFieldRenderer2);*/
	//	treeManager = new TreeManager();
	//	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	//	//treeManager->update(1.0/60.0);
	//	treeManager->uninitialize();

	//	SAFE_DELETE(treeManager);
	//	i--;
	//}
	//i++;
	//treeManager = new TreeManager();
	//treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
#pragma endregion


#ifdef _DEBUG
	// デバッグエネミーモードにするための準備
	enemyManager->setDebugEnvironment(camera, &player[gameMasterNS::PLAYER_1P]);
#endif // _DEBUG

	////エネミーをランダムに設置する
	//for (int i = 0; i < enemyNS::ENEMY_OBJECT_MAX; i++)
	//{
	//	D3DXVECTOR3 pos = D3DXVECTOR3(rand() % 400, 150, rand() % 480);
	//	pos -= D3DXVECTOR3(200, 0, 240);
	//	enemyNS::ENEMYSET tmp =
	//	{
	//		enemyManager->issueNewEnemyID(),
	//		rand() % (enemyNS::ENEMY_TYPE::TYPE_MAX - 1),
	//		stateMachineNS::PATROL,
	//		pos,
	//		D3DXVECTOR3(0.0f, 0.0f, 0.0f)
	//	};
	//	enemyNS::EnemyData* p = enemyManager->createEnemyData(tmp);
	//	enemyManager->createEnemy(p);
	//}

	// ツリーをツール情報を元に設置する
	treeManager->createUsingTool();

	// メタAI（メタAIはツリーの数が確定した後に初期化する）
	aiDirector = new AIDirector;
	aiDirector->initialize(gameMaster, testFieldRenderer->getStaticMesh()->mesh,
		player, enemyManager, treeManager, itemManager, telopManager);
	
	//ゲーム開始時処理
	gameMaster->startGame();
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Game::uninitialize() {

	//SAFE_DELETE(linear4TreeManager);
	SAFE_DELETE(linear8TreeManager);
	SAFE_DELETE_ARRAY(player);
	SAFE_DELETE_ARRAY(camera);
	SAFE_DELETE(cameraOP);
	SAFE_DELETE(light);
	SAFE_DELETE(testField);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(maleRenderer);
	SAFE_DELETE(femaleRenderer);
	SAFE_DELETE(sky);
	SAFE_DELETE(ocean);
	//SAFE_DELETE(testEffect);
	SAFE_DELETE(samplePlane);
	SAFE_DELETE(enemyManager);
	SAFE_DELETE(treeManager);
	SAFE_DELETE(itemManager);
	SAFE_DELETE(windManager);
	SAFE_DELETE(mapObjectManager);
	/*SAFE_DELETE(telop);*/
	SAFE_DELETE(telopManager);
	SAFE_DELETE(aiDirector);
	SAFE_DELETE(spriteGauge);
	SAFE_DELETE(reticle);
	SAFE_DELETE(fixedUI);
	SAFE_DELETE(player1UI);
	SAFE_DELETE(player2UI);
	SAFE_DELETE(countUI);
	//SAFE_DELETE(ad);
	SAFE_DELETE(networkClient);
	UninitMoveP();
	UninitMoveP1();

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Game::update(float _frameTime) {

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//gameMaster処理落ちやポーズに関わらない更新
	gameMaster->update(frameTime);

	//ポーズ中ならリターン
	if (gameMaster->paused())return;

	//【処理落ち】
	//フレーム時間が10FPS時の時間より長い場合は、処理落ち（更新しない）
	//※フレーム時間に準拠している処理が正常に機能しないため
	if (frameTime > 10.0f/60.0f)return;

	//オープニング時間の更新
	gameMaster->updateOpeningTime(frameTime);

	//カウントダウンの更新
	gameMaster->updateStartCountDown(frameTime);	//開始カウント
	gameMaster->updateFinishCountDown(frameTime);	//終了カウント

	//開始カウントダウン
	if (gameMaster->playActionStartCount(3))	countUI->startCount(3);
	if (gameMaster->playActionStartCount(2))	countUI->startCount(2);
	if (gameMaster->playActionStartCount(1))	countUI->startCount(1);
	if (gameMaster->playActionStartCount(0))	countUI->startCount(0);		//ゲーム開始

	//ゲームタイムの更新
	gameMaster->updateGameTime(frameTime);

	//終了カウントダウン
	if (gameMaster->playActionFinishCount(10))	countUI->finishCount(10);
	if (gameMaster->playActionFinishCount(9))	countUI->finishCount(9);
	if (gameMaster->playActionFinishCount(8))	countUI->finishCount(8);
	if (gameMaster->playActionFinishCount(7))	countUI->finishCount(7);
	if (gameMaster->playActionFinishCount(6))	countUI->finishCount(6);
	if (gameMaster->playActionFinishCount(5))	countUI->finishCount(5);
	if (gameMaster->playActionFinishCount(4))	countUI->finishCount(4);
	if (gameMaster->playActionFinishCount(3))	countUI->finishCount(3);
	if (gameMaster->playActionFinishCount(2))	countUI->finishCount(2);
	if (gameMaster->playActionFinishCount(1))	countUI->finishCount(1);
	if (gameMaster->playActionFinishCount(0))	countUI->finishCount(0);	//ゲーム終了

	//テストフィールドの更新
	testField->update();			//オブジェクト
	testFieldRenderer->update();	//レンダラー

	//プレイヤーの更新
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		player[i].update(frameTime);		//オブジェクト
	maleRenderer->update();					//レンダラー
	femaleRenderer->update();				//レンダラー

	// エネミーの更新
	//enemyManager->update(frameTime);

	// ツリーの更新
	treeManager->update(frameTime);

	// アイテムの更新
	itemManager->update(frameTime);

	// 風の更新
	windManager->update(frameTime);

	// マップオブジェクトの更新
	mapObjectManager->update(frameTime);

	UpdateMoveP(frameTime);
	//キャラクターの場所と回転の連携
	MOVEP *mp = GetMovePAdr();
	mp->Pos = player[gameMasterNS::PLAYER_1P].position;
	D3DXQUATERNION q = player[gameMasterNS::PLAYER_1P].quaternion;
	Base::anyAxisRotation(&q,D3DXVECTOR3(0,1,0),180);
	mp->Quaternion = q;

	UpdateMoveP1(frameTime);
	//キャラクターの場所と回転の連携
	MOVEP1 *mp1 = GetMoveP1Adr();
	mp1->Pos = player[gameMasterNS::PLAYER_2P].position;
	D3DXQUATERNION q1 = player[gameMasterNS::PLAYER_2P].quaternion;
	Base::anyAxisRotation(&q1, D3DXVECTOR3(0, 1, 0), 180);
	mp1->Quaternion = q1;


	//エフェクシアーのテスト
#pragma region EffekseerTest
	//エフェクトの再生
	if (input->wasKeyPressed('1'))
	{
		effekseerNS::Instance* instance = new effekseerNS::Instance();
		instance->position = *player[gameMasterNS::PLAYER_1P].getPosition();
		effekseerNS::play(0,instance);
	}
	if (input->wasKeyPressed('2'))
	{
		class Fire :public effekseerNS::Instance
		{
		public:
			D3DXVECTOR3 * syncPosition;
			Fire() {
				effectNo = effekseerNS::BLOW;
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
		effekseerNS::play(0,instance);
	}
	//エフェクトの一時停止：再生
	if (input->wasKeyPressed('3'))
	{
		effekseerNS::pause(0,false);
	}
	//エフェクトの一時停止：停止
	if (input->isKeyDown('4'))
	{
		effekseerNS::pause(0,true);
	}
	//エフェクトの停止
	if (input->wasKeyPressed('G'))
	{
		//effekseerNS::stop((*getEffekseerManager()->instanceList->getValue(0))->handle);
	}
#pragma endregion


	//テロップマネージャーの更新
	telopManager->update(frameTime);
	//テロップ発生フラグ
	//緑化状況10%
	if ((input->wasKeyPressed('M')) || treeManager->getGreeningRate() >= 0.1 &&
		!gameMaster->whetherAchieved(gameMasterNS::ACHIEVEMENT_GREENING_RATE_10 ))
	{
		telopManager->play(telopManagerNS::TELOP_TYPE0);
		gameMaster->setProgress(gameMasterNS::ACHIEVEMENT_GREENING_RATE_10);
	}
	//緑化状況30%
	if (treeManager->getGreeningRate() >= 0.3 &&
		!gameMaster->whetherAchieved(gameMasterNS::ACHIEVEMENT_GREENING_RATE_30))
	{
		telopManager->play(telopManagerNS::TELOP_TYPE1);
		gameMaster->setProgress(gameMasterNS::ACHIEVEMENT_GREENING_RATE_30);
	}
	//緑化状況50%
	if (treeManager->getGreeningRate() >= 0.5 &&
		!gameMaster->whetherAchieved(gameMasterNS::ACHIEVEMENT_GREENING_RATE_50))
	{
		telopManager->play(telopManagerNS::TELOP_TYPE2);
		gameMaster->setProgress(gameMasterNS::ACHIEVEMENT_GREENING_RATE_50);
	}
	

	//スカイドームの更新
	sky->update();
	//海面の更新
	ocean->update();

	//エフェクト（インスタンシング）テスト
	//testEffect->update(frameTime);

	//ディスプレイ用プレーンサンプル
	samplePlane->update(frameTime);
	// 開発中広告
	//ad->update(frameTime);
	
	//カメラの更新
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		camera[i].update();
	}

	//オープニングカメラの更新
	if (!gameMaster->whetherAchieved(gameMasterNS::PASSING_GAME_OPENING))
	{
		cameraOP->update();
	}

	//固定UIの更新
	fixedUI->update(gameMaster->getGameTime());

	//プレイヤー周りのUIの更新
	player1UI->update();
	player2UI->update();

	//カウントUIの更新
	countUI->update(frameTime);

	//レティクルの更新
	reticle->update(frameTime);


	// Enterまたは〇ボタンでリザルトへ
	//if (input->wasKeyPressed(VK_RETURN) ||
	//	input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
	//	input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	if(gameMaster->getGameTime() <= 0)
	{
		// サウンドの再生
		//sound->play(soundNS::TYPE::SE_DECISION, soundNS::METHOD::PLAY);
		// シーン遷移
		changeScene(nextScene);
	}

	

	//残り時間１分
	if (gameMaster->playActionRamaining1Min())
	{
		telopManager->play(telopManagerNS::TELOP_TYPE4);
		SoundInterface::BGM->SetSpeed();
	}

	networkClient->send(gameMaster->getGameTime());



	//フェーダーテスト
	if (input->wasKeyPressed('P'))
	{
		getFader()->setShader(faderNS::NORMAL);
		getFader()->start();
	}

#ifdef _DEBUG
	test();
#endif
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Game::render() 
{
	if (!gameMaster->whetherAchieved(gameMasterNS::PASSING_GAME_OPENING))
	{
		//OP中
		nowRenderingWindow = gameMasterNS::PLAYER_1P;
		direct3D9->changeViewportFullWindow();
		cameraOP->renderReady();
		render3D(cameraOP);
		return;
	}


	//1Pカメラ・ウィンドウ・エフェクシアーマネージャー
	nowRenderingWindow = gameMasterNS::PLAYER_1P;
	camera[gameMasterNS::PLAYER_1P].renderReady();
	direct3D9->changeViewport1PWindow();
	render3D(&camera[gameMasterNS::PLAYER_1P]);
	effekseerNS::setCameraMatrix(
		0,
		camera[gameMasterNS::PLAYER_1P].position,
		camera[gameMasterNS::PLAYER_1P].gazePosition,
		camera[gameMasterNS::PLAYER_1P].upVector);
	effekseerNS::render(0);
	effekseerNS::setCameraMatrix(
		1,
		camera[gameMasterNS::PLAYER_1P].position,
		camera[gameMasterNS::PLAYER_1P].gazePosition,
		camera[gameMasterNS::PLAYER_1P].upVector);
	effekseerNS::render(1);

	//2Pカメラ・ウィンドウ・エフェクシアーマネージャー
	nowRenderingWindow = gameMasterNS::PLAYER_2P;
	camera[gameMasterNS::PLAYER_2P].renderReady();
	direct3D9->changeViewport2PWindow();
	render3D(&camera[gameMasterNS::PLAYER_2P]);
	effekseerNS::setCameraMatrix(
		0,
		camera[gameMasterNS::PLAYER_2P].position,
		camera[gameMasterNS::PLAYER_2P].gazePosition,
		camera[gameMasterNS::PLAYER_2P].upVector);
	effekseerNS::render(0);

	effekseerNS::setCameraMatrix(
		2,
		camera[gameMasterNS::PLAYER_2P].position,
		camera[gameMasterNS::PLAYER_2P].gazePosition,
		camera[gameMasterNS::PLAYER_2P].upVector);
	effekseerNS::render(2);

	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();

}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
void Game::render3D(Camera* currentCamera) {

	//スカイドームの描画
	sky->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	//テストフィールドの描画
	if (player[nowRenderingWindow].getState() == playerNS::STATE::VISION ||
		player[nowRenderingWindow].getState() == playerNS::STATE::SKY_VISION)
	{
		testFieldRenderer->setStaticMesh(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_BLACK));
	}
	else {
		testFieldRenderer->setStaticMesh(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
	}
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);


	// プレイヤーの描画
	//maleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);
	//femaleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);

	// プレイヤーの他のオブジェクトの描画
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		player[i].otherRender(currentCamera->view, currentCamera->projection, currentCamera->position);

	//アニメーションモデルの描画
	DrawMoveP();
	DrawMoveP1();
	//スカイドームの描画
	//sky->render(currentCamera->view, currentCamera->projection, currentCamera->position);
	//海面の描画
	//ocean->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// エネミーの描画
	enemyManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// ツリーの描画
	if (player[nowRenderingWindow].getState() == playerNS::STATE::VISION ||
		player[nowRenderingWindow].getState() == playerNS::STATE::SKY_VISION)
	{
		treeManager->switchingVisionView(nowRenderingWindow);
	}
	else {
		treeManager->switchingNormalView(nowRenderingWindow);
	}
	treeManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// アイテムの描画
	itemManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// 風の描画
	windManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// マップオブジェクトの描画
	mapObjectManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	//エフェクト（インスタンシング）テスト
	//testEffect->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	//ディスプレイ用プレーンサンプル
	samplePlane->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// 開発中広告
	//ad->render(currentCamera->view, currentCamera->projection, currentCamera->position);


	//レティクル3D描画
	if(player[nowRenderingWindow].getState() == playerNS::STATE::NORMAL)
		reticle->render3D(nowRenderingWindow,currentCamera->view, currentCamera->projection, currentCamera->position);

#if _DEBUG
	//4分木空間分割のライン描画
	//linear4TreeManager->render();
	//8分木空間分割のライン描画
	//linear8TreeManager->render();
	//Ray ray;
	//ray.color = D3DXCOLOR(150, 150, 0, 255);
	//Object** root = collisionList->getRoot();
	//Object* tmp1 = NULL;
	//Object* tmp2 = NULL;
	//D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0);
	//float length = 0;
	//for (int i = 0; i < collisionNum; i++)
	//{
	//	tmp1 = root[i * 2];
	//	tmp2 = root[i * 2 + 1];
	//	length = Base::between2VectorDirection(&direction, tmp1->position, tmp2->position);
	//	ray.initialize(tmp1->position, direction);
	//	ray.render(length);
	//}
#endif

#ifdef _DEBUG
#if 0	// ナビゲーションメッシュのデバッグ描画
	naviMesh->debugRender(currentCamera->view, currentCamera->projection, currentCamera->position);
#endif
#endif //_DEBUG
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
	//spriteGauge->render();
	//telop->render();
	
	// テロップマネージャーの描画
	telopManager->render();	

	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//固定UIの描画
	fixedUI->render();

	//プレイヤー1周りのUIの描画
	player1UI->render();

	//プレイヤー2周りのUIの描画
	player2UI->render();

	//カウントUIの描画
	countUI->render();

	//レティクルの描画
	reticle->render2D(&player[gameMasterNS::PLAYER_1P]);
	reticle->render2D(&player[gameMasterNS::PLAYER_2P]);
}

//===================================================================================================================================
//【線形４分木空間分割マネージャー再登録処理】
//===================================================================================================================================
//void Game::tree4Reregister(Object* tmp)
//{
//	//一度リストから外れる
//	tmp->treeCell.remove();
//	//再登録
//	linear4TreeManager->registerObject(
//		tmp->getLeft(), tmp->getFront(), tmp->getRight(), tmp->getBack(), &tmp->treeCell);
//
//}

//===================================================================================================================================
//【線形８分木空間分割マネージャー再登録処理】
//===================================================================================================================================
void Game::tree8Reregister(Object* tmp)
{
	//一度リストから外れる
	tmp->treeCell.remove();
	//再登録
	linear8TreeManager->registerObject( &tmp->getMin(),&tmp->getMax(), &tmp->treeCell);
}

//===================================================================================================================================
//【衝突判定処理】
//===================================================================================================================================
void Game::collisions()
{
	//8分木への再登録
	//プレイヤーの登録
	tree8Reregister(&player[gameMasterNS::PLAYER_1P]);//1Pの登録
	tree8Reregister(&player[gameMasterNS::PLAYER_2P]);//2Pの登録
	//弾の登録
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		for (int num = 0; num < player[i].getShootingNum(); num++)
			tree8Reregister(player[i].getBullet(num));
	}
	//敵の登録
	for (int i = 0; i < enemyManager->getEnemyList().size(); i++)
	{
		tree8Reregister(enemyManager->getEnemyList()[i]);
	}
	//木の登録
	for (int i = 0; i < treeManager->getTreeList().size(); i++)
	{
		tree8Reregister(treeManager->getTreeList()[i]);
		if (treeManager->getTreeList()[i]->getTreeData()->type == treeNS::DIGITAL_TREE
			&&treeManager->getTreeList()[i]->isAroundGreening())
		{
			//緑化エリアオブジェクトの登録
			tree8Reregister(treeManager->getTreeList()[i]->getGreeningArea());
		}
	}
	// マップオブジェクトの登録
	for (size_t i = 0; i < mapObjectManager->getMapObjectList().size(); i++)
	{
		tree8Reregister(mapObjectManager->getMapObjectList()[i]);
	}

	//衝突対応リストを取得
	collisionNum = linear8TreeManager->getAllCollisionList(&collisionList);
	collisionNum /= 2;//2で割るのはペアになっているため

	//衝突判定
	Object** root = collisionList->getRoot();
	Object* tmp1 = NULL;
	Object* tmp2 = NULL;
	for (int i = 0; i < collisionNum; i++)
	{
		tmp1 = root[i * 2];
		tmp2 = root[i * 2 + 1];
		CollisionManager::collision(tmp1, tmp2);//衝突処理
	}

	// 風との当たり判定
	windManager->windCollision(player);


	//カメラとフィールド
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		LPD3DXMESH mesh = testFieldRenderer->getStaticMesh()->mesh;
		D3DXMATRIX matrix = testField->matrixWorld;
		//カメラのめり込み補正
		camera[i].insetCorrection(mesh, matrix);
	}

	//プレイヤーとフィールド
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		LPD3DXMESH mesh = testFieldRenderer->getStaticMesh()->mesh;
		D3DXMATRIX matrix = testField->matrixWorld;
		//地面方向補正処理
		player[i].grounding(mesh,matrix);
		//壁ずり処理
		player[i].insetCorrection(objectNS::AXIS_X, player[i].size.x / 2,mesh,matrix);
		player[i].insetCorrection(objectNS::AXIS_RX, player[i].size.x / 2, mesh, matrix);
		player[i].insetCorrection(objectNS::AXIS_Z, player[i].size.z / 2, mesh, matrix);
		player[i].insetCorrection(objectNS::AXIS_RZ, player[i].size.z / 2, mesh, matrix);
		//照準レイ更新/姿勢更新/狙撃レイ更新
		player[i].updateAiming(mesh, matrix);
		player[i].updatePostureByAiming();
		player[i].updateShooting(mesh, matrix);
	}

	//ビジョン|スカイビジョン状態の時
	//プレイヤー[シフトレイ]とデジタルツリー
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//シフト操作の無効化(初期値)
		player[i].disableOperation(playerNS::ENABLE_SHIFT);
		//選択ライトのOFF
		player[i].shownSelectLight(false);


		//選択されたデジタルツリー
		Tree* selectTree = NULL;
		//リストの取得
		std::vector<Tree*> list = treeManager->getTreeList();

		//ビジョン時スルー&選択ライトエフェクトをOFFにする
		if (player[i].getState() != playerNS::VISION &&
			player[i].getState() != playerNS::SKY_VISION)
		{
			//選択解除
			for (int num = 0; num < list.size(); num++)
			{
				Tree* tree = list[num];
				//デジタルツリーでない場合continue
				if (tree->getTreeData()->type != treeNS::DIGITAL_TREE)continue;
				//選択されていない状態にする
				tree->switchingSelected(false, i);
			}
			continue;
		}

		//衝突判定
		for (int num = 0; num < list.size(); num++)
		{
			Tree* tree = list[num];
			//デジタルツリーの場合
			if (tree->getTreeData()->type != treeNS::DIGITAL_TREE)continue;
			//選択されていない状態にする
			tree->switchingSelected(false, i);

			//カリング処理
			//カメラ視野角内の場合
			D3DXVECTOR3 center = tree->center;
			float radius = tree->radius;
			if (!UtilityFunction::culling(
				center, radius, camera[i].view, camera[i].fieldOfView,
				camera[i].nearZ, camera[i].farZ, camera[i].aspect))continue;

			//シフトレイの更新
			Cylinder treeCylinder;
			treeCylinder.centerLine.start	= tree->position;
			treeCylinder.centerLine.end		= tree->position + tree->getAxisY()->direction*tree->size.y;
			treeCylinder.height				= tree->size.y/2;
			treeCylinder.radius				= tree->size.x;
			//衝突時
			if (player[i].collideShiftRay(treeCylinder))
			{
				//選択候補として保存する。
				selectTree = tree;
			}
		}

		//選択されたデジタルツリーへの処理を行う
		if (selectTree)
		{
			selectTree->switchingSelected(true, i);
			player[i].shownSelectLight(true);
		}
	}

	

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
				//FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
				PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Getlem, false ,NULL,false,NULL};
				SoundInterface::SE->playSound(&playParameters);	//SE再生
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
	Object** root = collisionList->getRoot();
	Object* tmp1;
	Object* tmp2;
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("count = %d", count);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//ImGui::Text("node:%d", testEffect->getList().nodeNum);
	ImGui::Text("collisionNum:%d", collisionNum);
	if (ImGui::CollapsingHeader("CollisionList"))
	{
		for (int i = 0; i < collisionNum; i++)
		{
			tmp1 = root[i * 2];
			tmp2 = root[i * 2 +1];
			ImGui::Text("ID(%d):position(%.02f,%.02f,%.02f)<->ID(%d):position(%.02f,%.02f,%.02f)",
				tmp1->id, tmp1->position.x,tmp1->position.y,tmp1->position.z,
				tmp2->id, tmp2->position.x,tmp2->position.y,tmp2->position.z);
		}
	}
	networkClient->outputGUI();
	player->outputGUI();			//プレイヤー
	enemyManager->outputGUI();		//エネミー
	itemManager->outputGUI();		//アイテムマネージャ
	treeManager->outputGUI();		//ツリーマネージャー
	testField->outputGUI();			//テストフィールド
	camera->outputGUI();			//カメラ
	naviMesh->outputGUI();			//ナビゲーションAI
}
#endif // _DEBUG


//===================================================================================================================================
//【なんかいろいろテストするところ】
//===================================================================================================================================
#ifdef _DEBUG
void Game::test()
{
	// アイテムマネージャのテスト
	if (input->wasKeyPressed('0'))
	{
		itemNS::ItemData unko = { itemManager->issueNewItemID(), itemNS::BATTERY, *player->getPosition() };
		itemManager->createItem(unko);
	}
	// 3Dモデル表示確認用（アイテムの更新）
	if (input->wasKeyPressed('N'))
	{
		itemNS::ItemData abc = { 1, itemNS::EXAMPLE, *player->getPosition() };
		itemManager->createItem(abc);
	}

	if (input->wasKeyPressed('9'))
	{
		itemManager->destroyAllItem();
		//itemManager->destroyItem(3);
	}

	// デバッグエネミーで'7','8'使用中
	//// マップオブジェクトマネージャーのテスト
	//if (input->wasKeyPressed('7'))	// 作成
	//{
	//	mapObjectNS::MapObjectData mapObjectData;
	//	mapObjectData.zeroClear();
	//	mapObjectData.mapObjectID = mapObjectManager->issueNewMapObjectID();
	//	mapObjectData.type = mapObjectNS::STONE_01;
	//	mapObjectData.defaultPosition = *player->getPosition();
	//	mapObjectManager->createMapObject(mapObjectData);
	//}


	if (input->wasKeyPressed('6'))
	{
		//aiDirector->eventMaker.makeEventSpawningEnemyAroundPlayer(0);

		aiDirector->eventMaker.makeEventBossEntry();
	}

	//if (input->wasKeyPressed('6'))
	//{
	//	aiDirector->eventMaker.makeEventSpawningEnemyAroundPlayer(0);

	//	//aiDirector->eventMaker.makeEventEnemyAttaksTree();
	//}
	if (input->wasKeyPressed('Z'))
	{
		player->position = D3DXVECTOR3(20, 5, 0);
	}

	// ツリーマネージャのテスト
	if (input->wasKeyPressed('5'))	// 作成
	{
		treeNS::TreeData treeData;
		treeData.treeID = treeManager->issueNewTreeID();
		treeData.hp = 100;
		treeData.type = treeNS::ANALOG_TREE;
		treeData.size = treeNS::STANDARD;
		treeData.greenState = treeNS::DEAD;
		treeData.model = treeNS::B_MODEL;
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

	//電力関係UIのテスト
	//電力減少（電力回復確認用）
	if (input->isKeyDown('U'))
	{
		player[gameMasterNS::PLAYER_1P].pullpower(1);
	}
	//電力増加（ゲージ、タンクテスト用）
	if (input->isKeyDown('I'))
	{
		player[gameMasterNS::PLAYER_1P].addpower(1);
	}
	//電力減少（電力回復確認用）
	if (input->isKeyDown('O'))
	{
		player[gameMasterNS::PLAYER_2P].pullpower(1);
	}
	//電力増加（ゲージ、タンクテスト用）
	if (input->isKeyDown('P'))
	{
		player[gameMasterNS::PLAYER_2P].addpower(1);
	}
}
#endif // _DEBUG
