//===================================================================================================================================
//【Game.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/11/19
//===================================================================================================================================

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// TODO:2020/02/10：未実装
// ※対応後に消してください（by 中込）
// プレイヤーの初期位置は、プレイヤー間で重複なくランダムに4か所から選定されます。
// �@南の平野(-43, 8, -236), �A西の岬(-265, 54, 26), �B北の平野(142, 13, 246), �C東の狭い所(311, 13, -65)
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
//【グローバル変数】
//===================================================================================================================================
bool FirstInit = true;
bool reticleFlag = false;

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
	linear8TreeManager->initialize(5, D3DXVECTOR3(-3000, -2000, -3000), D3DXVECTOR3(3000, 3000, 3000));

	//オブジェクトカウンターのリセット
	objectNS::resetCounter();

	sceneName = "Scene -Game-";

	nextScene = SceneList::RESULT;

	//再生パラメータ
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Game, true,1.1f,false,NULL };
	playParameters[1] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,  SE_LIST::SE_CountDown, false,NULL,false,NULL };
	playParameters[2] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,  SE_LIST::SE_StartGame, false,NULL,false,NULL };
	playParameters[3] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,  SE_LIST::SE_TimeUp, false,NULL,false,NULL };
	playParameters[4] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Opening, false,1.0f,false,NULL };
	playParameters[5] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_HurryUp, false,1.0f,false,NULL };
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Game::~Game()
{
	// サウンドの停止
	SoundInterface::BGM->uninitSoundStop();
	SoundInterface::S3D->uninitSoundStop();
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Game::initialize() {

	//衝突判定フィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	// 装飾フィールド
	faceField = new Object();
	faceFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_FACE));
	faceFieldRenderer->registerObject(faceField);
	faceField->initialize(&D3DXVECTOR3(0, 0, 0));

	// 遠景
	distantView = new Object();
	viewRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DISTANT_VIEW));
	viewRenderer->registerObject(distantView);
	distantView->initialize(&D3DXVECTOR3(0, 0, 0));

	//player
	player				= new Player[gameMasterNS::PLAYER_NUM];

	//OPカメラターゲットオブジェクト
	target = new Object;
	target->initialize(&D3DXVECTOR3(247.0f, 38.0f, 181.0f));		//ターゲットの初期位置設定

	//オープニングカメラ
	cameraOP = new Camera;
	//カメラの設定
	cameraOP->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	cameraOP->setRelative(D3DXQUATERNION(0.0f, 0.0f, 1.0f, 0.0f));
	cameraOP->setTarget(&target->position);
	cameraOP->setGazeDistance(20.0f);
	cameraOP->setUpVector(D3DXVECTOR3(0, 1, 0));
	cameraOP->setFieldOfView((D3DX_PI) / 18 * 10);
	cameraOP->setViewProjection();
	cameraOP->setLimitRotationTop(0.1f);
	cameraOP->setLimitRotationBottom(0.1f);
	cameraOP->updateOrtho();
	//カメラ操作変数
	stateCamera = CAMERA0;		//初期フォトグラフ
	cameraAxisZ = D3DXVECTOR3(0, 0, 0);
	fixedAxisZ = D3DXVECTOR3(0, 0, 0);
	cameraAxisX = D3DXVECTOR3(0, 0, 0);
	cameraAxisY = D3DXVECTOR3(0, 0, 0);
	fixedAxisX = D3DXVECTOR3(0, 0, 0);

	//プレイヤーカメラ
	//camera
	camera = new Camera[gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//カメラの設定
		camera[i].initialize(WINDOW_WIDTH / 2, WINDOW_HEIGHT);
		camera[i].setTarget(player[i].getCameraGaze());
		camera[i].setTargetX(&player[i].getAxisX()->direction);
		camera[i].setTargetY(&player[i].getAxisY()->direction);
		camera[i].setTargetZ(&player[i].getAxisZ()->direction);
		camera[i].setRelative(CAMERA_RELATIVE_QUATERNION);
		camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
		camera[i].setRelativeGaze(CAMERA_RELATIVE_GAZE);
		camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
		camera[i].setFieldOfView( (D3DX_PI/180) * 90);
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
			infomation.playerType = gameMasterNS::PLAYER_2P;
			infomation.modelType = gameMasterNS::MODEL_FEMALE;
			player[i].initialize(infomation);
			break;
		}

		//カメラポインタのセット
		player[i].setCamera(&camera[i]);

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
	InitMoveP(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.003f, 0.003f, 0.003f), FirstInit);
	InitMoveP1(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.003f, 0.003f, 0.003f), FirstInit);

	FirstInit = false;

	// ナビゲーションAI（ナビゲーションAIはエネミー関係クラスより先に初期化する）
#ifdef SAMPLE_NAVI
	naviMesh = new NavigationMesh(staticMeshNS::reference(staticMeshNS::SAMPLE_NAVMESH));
#else
	naviMesh = new NavigationMesh(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
#endif
	naviMesh->initialize();

	//マーカー
	markerRenderer = new MarkerRenderer;
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		markerRenderer->playerPosition[i] = &player[i].center;
	}

	// エネミー
	enemyManager = new EnemyManager;
	enemyManager->initialize(*getSceneName(),testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld(), gameMaster, player, markerRenderer);

	// ツリー
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	treeManager->setGameMaster(gameMaster);

	// アイテム
	itemManager = new ItemManager;
	itemManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// 風
	windManager = new WindManager;
	windManager->initialize(player);

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

	//Network
	networkClient = new NETWORK_CLIENT;

	//OPアナウンス
	announcement = new Announcement;

	//ダメージUI
	damageUI = new DamageUI();


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

	// ツリーをツール情報を元に設置する
	treeManager->createUsingTool();

	// メタAI（メタAIはツリーの数が確定した後に初期化する）5
	aiDirector = new AIDirector;
	aiDirector->initialize(gameMaster, testFieldRenderer->getStaticMesh()->mesh,
		player, enemyManager, treeManager, itemManager, telopManager, markerRenderer);
	
	//ゲーム開始時処理
	gameMaster->startGame();
	gameMaster->setTreeNum(treeManager->getTreeNum());
	if (!getModeFlag(MODE_PRESENTATION))SoundInterface::BGM->playSound(&playParameters[4]);	//オープニングBGM再生

	reticleFlag = false;
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
	SAFE_DELETE(faceField);
	SAFE_DELETE(faceFieldRenderer);
	SAFE_DELETE(sky);
	SAFE_DELETE(ocean);
	SAFE_DELETE(enemyManager);
	SAFE_DELETE(treeManager);
	SAFE_DELETE(itemManager);
	SAFE_DELETE(windManager);
	SAFE_DELETE(mapObjectManager);
	SAFE_DELETE(telopManager);
	SAFE_DELETE(aiDirector);
	SAFE_DELETE(reticle);
	SAFE_DELETE(fixedUI);
	SAFE_DELETE(player1UI);
	SAFE_DELETE(player2UI);
	SAFE_DELETE(countUI);
	SAFE_DELETE(networkClient);
	SAFE_DELETE(announcement);
	SAFE_DELETE(target);
	SAFE_DELETE(damageUI);
	SAFE_DELETE(markerRenderer);
	SAFE_DELETE(distantView);
	SAFE_DELETE(viewRenderer);
}

//===================================================================================================================================
//【OP更新】
//===================================================================================================================================
void Game::updateOP()
{
	//プレゼンテーションモード時冒頭処理
	if (getModeFlag(MODE_PRESENTATION))
	{
		//OPのスキップ
		gameMaster->updateOpeningTime(gameMasterNS::OPENING_TIME);
		//開始カウントダウンのスキップ
		gameMaster->updateStartCountDown(gameMasterNS::COUNT_DOWN_TIME);
		if (gameMaster->playActionStartCount(0))
		{
			countUI->startCount(0);									//ゲーム開始
			SoundInterface::SE->playSound(&playParameters[2]);		//開始サウンド
			SoundInterface::BGM->playSound(&playParameters[0]);		//BGM再生
			telopManager->playOrder(telopManagerNS::TELOP_TYPE6);	//テロップ
			enemyManager->setUpdate(true);							//エネミー更新開始
			//プレイヤーを通常状態へ
			player[gameMasterNS::PLAYER_1P].transState(playerNS::NORMAL);
			player[gameMasterNS::PLAYER_1P].enableOperation(playerNS::ENABLE_CAMERA);
			player[gameMasterNS::PLAYER_2P].transState(playerNS::NORMAL);
			player[gameMasterNS::PLAYER_2P].enableOperation(playerNS::ENABLE_CAMERA);
		}
		return;
	}

	//オープニング時間の更新
	if (input->getController()[gameMasterNS::PLAYER_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[gameMasterNS::PLAYER_2P]->wasButton(virtualControllerNS::A) ||
		input->getController()[gameMasterNS::PLAYER_1P]->wasButton(virtualControllerNS::B) ||
		input->getController()[gameMasterNS::PLAYER_2P]->wasButton(virtualControllerNS::B) ||
		input->wasKeyPressed(VK_RETURN))
	{
		//スキップ
		gameMaster->updateOpeningTime(gameMasterNS::OPENING_TIME);
	}
	else {
		gameMaster->updateOpeningTime(frameTime);
	}

	//カウントダウンの更新
	gameMaster->updateStartCountDown(frameTime);	//開始カウント
	gameMaster->updateFinishCountDown(frameTime);	//終了カウント

	//開始カウントダウン
	if (gameMaster->playActionStartCount(3))
	{
		countUI->startCount(3);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionStartCount(2))
	{
		countUI->startCount(2);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionStartCount(1))
	{
		countUI->startCount(1);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionStartCount(0))
	{
		countUI->startCount(0);									//ゲーム開始
		SoundInterface::SE->playSound(&playParameters[2]);		//開始サウンド
		SoundInterface::BGM->playSound(&playParameters[0]);		//BGM再生
		telopManager->playOrder(telopManagerNS::TELOP_TYPE6);	//テロップ
		enemyManager->setUpdate(true);							//エネミー更新開始
		//プレイヤーを通常状態へ
		player[gameMasterNS::PLAYER_1P].transState(playerNS::NORMAL);
		player[gameMasterNS::PLAYER_1P].enableOperation(playerNS::ENABLE_CAMERA);
		player[gameMasterNS::PLAYER_2P].transState(playerNS::NORMAL);
		player[gameMasterNS::PLAYER_2P].enableOperation(playerNS::ENABLE_CAMERA);
		reticleFlag = true;
	}
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
	if (frameTime > 10.0f / 60.0f)return;

	//OPの更新処理
	updateOP();

	//ゲームタイムの更新
	gameMaster->updateGameTime(frameTime);

	//終了カウントダウン
	if (gameMaster->playActionFinishCount(10))
	{
		countUI->finishCount(10);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(9))
	{
		countUI->finishCount(9);
		SoundInterface::SE->playSound(&playParameters[1]);
	}

	if (gameMaster->playActionFinishCount(8))
	{
		countUI->finishCount(8);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(7))
	{
		countUI->finishCount(7);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(6))
	{
		countUI->finishCount(6);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(5))
	{
		countUI->finishCount(5);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(4))
	{
		countUI->finishCount(4);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(3))
	{
		countUI->finishCount(3);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(2))
	{
		countUI->finishCount(2);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(1))
	{
		countUI->finishCount(1);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(0))
	{
		countUI->finishCount(0);	//ゲーム終了
		SoundInterface::SE->playSound(&playParameters[3]);	// タイムアップサウンド
		for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		{
			player[i].disableOperation(
				playerNS::ENABLE_SHOT |
				playerNS::ENABLE_SKY_VISION |
				playerNS::ENABLE_VISION);
		}

	}

	//OPカメラのターゲットの更新
	target->update();

	//エンディング時間の更新
	gameMaster->updateEndingTime(frameTime);


	//ゲーム開始時ボイス
	if (gameMaster->getGameTime() < gameMasterNS::GAME_TIME && gameMaster->wasStartVoicePlayed[gameMasterNS::PLAYER_1P] == false)
	{
		PLAY_PARAMETERS voiceStart = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, S3D_LIST::Voice_Male_Start, false, NULL, true, gameMasterNS::PLAYER_1P };
		SoundInterface::S3D->playSound(&voiceStart);
		gameMaster->wasStartVoicePlayed[gameMasterNS::PLAYER_1P] = true;
	}
	else if (gameMaster->getGameTime() < gameMasterNS::GAME_TIME -1.5f && gameMaster->wasStartVoicePlayed[gameMasterNS::PLAYER_2P] == false)
	{
		PLAY_PARAMETERS voiceStart = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, S3D_LIST::Voice_Female_Start, false, NULL, true, gameMasterNS::PLAYER_2P };
		SoundInterface::S3D->playSound(&voiceStart);
		gameMaster->wasStartVoicePlayed[gameMasterNS::PLAYER_2P] = true;
	}

	//OPカメラの変数更新
	//注視オブジェクトとカメラの二点間ベクトル（カメラZ軸ベクトル）
	cameraAxisZ = cameraOP->getAxisZ();
	fixedAxisZ = Base::slip(cameraAxisZ, cameraOP->upVector);	//カメラの傾きに対応
	//カメラX軸ベクトル
	D3DXVec3Cross(&cameraAxisX, &cameraOP->upVector, &cameraAxisZ);
	//カメラY軸ベクトル（上方向の更新）
	D3DXVECTOR3 Y = D3DXVECTOR3(0, 0, 0);
	D3DXVec3Cross(&Y, &fixedAxisZ, &cameraAxisX);
	D3DXVec3Cross(&cameraAxisY, &cameraAxisZ, &cameraAxisX);
	D3DXVec3Cross(&fixedAxisX, &cameraAxisY, &cameraAxisZ);

	//OPカメラの状態別更新
	switch (stateCamera)
	{
	case CAMERA0:
		if (sceneTimer > 0.0f)
		{
			startPos = target->position;	//ラープ始点
			moveTime = 2.5f;				//終点までの時間
			moveTimer = moveTime;			//移動タイマー
			degreeTime = 2.5f;
			degreeTimer = degreeTime;
			stateCamera++;
			cameraOP->rotation(D3DXVECTOR3(0, 1, 0), 90.0f);
			cameraOP->rotation(fixedAxisX, -55.0f);
			//カメラの相対位置を一時保存
			tmpCameraQ = cameraOP->relativeQuaternion;
		}
		break;
	case CAMERA1:
		moveTimer -= frameTime;
		rate = moveTimer / moveTime;
		degreeTimer -= frameTime;
		rateX = degreeTimer / degreeTime;
		degreeX = UtilityFunction::lerp(0, 30, 1.0f - rateX);
		D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(247.0f, 122.0f, 181.0f), 1.0f - rate);
		cameraOP->relativeQuaternion = tmpCameraQ;
		//前のカメラの相対位置に補正する
		if (degreeTimer > 0)cameraOP->rotation(fixedAxisX, degreeX);
		if (moveTimer <= 0)
		{
			target->position = D3DXVECTOR3(105.0f, 73.0f, -250.0);
			startPos = target->position;
			moveTime = 2.5f;
			moveTimer = moveTime;
			degreeTime = 2.5f;
			degreeTimer = degreeTime;
			stateCamera++;
			//カメラの相対位置を一時保存
			cameraOP->rotation(D3DXVECTOR3(0, 1, 0), 90.0f);
			cameraOP->rotation(fixedAxisX, -30.0f);
			tmpCameraQ = cameraOP->relativeQuaternion;
		}	
		break;
	case CAMERA2:
		moveTimer -= frameTime;
		degreeTimer -= frameTime;
		rate = moveTimer / moveTime;
		rateY = degreeTimer / degreeTime;
		D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(-64.0f, 73.0f, -232.0f), 1.0f - rate);
		degreeY = UtilityFunction::lerp(0, 20.0f, 1.0f - rateY);
		if (degreeTimer > 0)
		{
			//前のカメラの相対位置に補正する
			cameraOP->relativeQuaternion = tmpCameraQ;
			cameraOP->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
		}
		if (moveTimer <= 0)
		{
			target->position = D3DXVECTOR3(-18.0f, 167.0f, 27.0f);
			startPos = target->position;
			moveTime = 5.0f;
			moveTimer = moveTime;
			degreeTimer = 5.0f;
			degreeTime = degreeTimer;
			cameraOP->setGazeDistance(100.0f);
			stateCamera++;
			//次ステート用に角度調整
			cameraOP->rotation(fixedAxisX, 35.0f);
			//カメラの相対位置を一時保存
			tmpCameraQ = cameraOP->relativeQuaternion;
		}
		break;
	case CAMERA3:
		moveTimer -= frameTime;
		cameraOP->rotation(D3DXVECTOR3(0, 1, 0), -frameDegree);
		if (moveTimer <= 0)	stateCamera++;
		break;
	case CAMERA4:break;
	default:break;
	}
	
	//テストフィールドの更新
	testField->update();			//オブジェクト
	testFieldRenderer->update();	//レンダラー

	//装飾フィールドの更新
	faceField->update();			//オブジェクト
	faceFieldRenderer->update();	//レンダラー

	//遠景の更新
	distantView->update();
	viewRenderer->update();

	//プレイヤーの更新
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		player[i].update(frameTime);		//オブジェクト
		if (player[i].position.y < -10.0f)
		{
			player[i].reset();
		}
		// 死亡時
		if (player[i].getHp() <= 0)
		{
			player[i].enableOperation(playerNS::DISABLE_OPERATION);
			if (player1UI->hpGuage->widthSize <= 0 && i == playerNS::PLAYER1)
			{
				// リスポーンとペナルティ処理
				player[i].enableOperation(playerNS::ALL_OPERATION);
				player[i].reset();
				player[i].setHp(playerNS::MAX_HP);
				player[i].setPower(playerNS::COST_SHIFT * 1.5f);
				player1UI->hpGuage->widthSize = hpGuageNS::WIDTH_HP_GUAGE;
				player1UI->hpGuage->hpGuage->setSize(hpGuageNS::WIDTH_HP_GUAGE, hpGuageNS::HEIGHT_HP_GUAGE);
				player1UI->hpGuage->hpGuage->setVertex();
				player1UI->electGuage->widthSize = electGuageNS::WIDTH_EN_GUAGE * (playerNS::COST_SHIFT * 1.5f / playerNS::MAX_POWER);
			}
			else if((player2UI->hpGuage->widthSize <= 0 && i == playerNS::PLAYER2))
			{
				// リスポーンとペナルティ処理
				player[i].enableOperation(playerNS::ALL_OPERATION);
				player[i].reset();
				player[i].setHp(playerNS::MAX_HP);
				player[i].setPower(playerNS::COST_SHIFT * 1.5f);
				player2UI->hpGuage->initialize();
				player2UI->electGuage->widthSize = electGuageNS::WIDTH_EN_GUAGE * (playerNS::COST_SHIFT * 1.5f / playerNS::MAX_POWER);
			}
		}
	}

	// エネミーの更新
	enemyManager->update(frameTime);
#ifdef CHEAT_PREZEN
	if (input->getController()[0]->wasButton(virtualControllerNS::RIGHT))
	{
		aiDirector->eventMaker.makeEventBossEntry();
	}
	if (input->wasKeyPressed('6') || input->getController()[0]->wasButton(virtualControllerNS::UP))
	{
		aiDirector->eventMaker.makeEventEnemyAttaksTree();
	}
	if (input->wasKeyPressed('7') || input->getController()[0]->wasButton(virtualControllerNS::DOWN))
	{
		aiDirector->eventMaker.makeEventPowerUpItem();
	}
#endif

	// ツリーの更新
	treeManager->update(frameTime);

	// アイテムの更新
	itemManager->update(frameTime);

	// 風の更新
	windManager->update(frameTime);

	// マップオブジェクトの更新
	mapObjectManager->update(frameTime);

	//(1P)アニメーションモデルとプレイヤーの場所と回転の連携
	UpdateMoveP(frameTime);
	MOVEP *mp = GetMovePAdr();
	mp->Pos = player[gameMasterNS::PLAYER_1P].position;
	D3DXQUATERNION q = player[gameMasterNS::PLAYER_1P].quaternion;
	Base::anyAxisRotation(&q, D3DXVECTOR3(0, 1, 0), 180);
	if (!mp->IsDie)
	{
		mp->Quaternion = q;
	}
	
	//(2P)アニメーションモデルとプレイヤーの場所と回転の連携
	UpdateMoveP1(frameTime);
	MOVEP1 *mp1 = GetMoveP1Adr();
	mp1->Pos = player[gameMasterNS::PLAYER_2P].position;
	D3DXQUATERNION q1 = player[gameMasterNS::PLAYER_2P].quaternion;
	Base::anyAxisRotation(&q1, D3DXVECTOR3(0, 1, 0), 180);
	if (!mp1->IsDie)
	{
		mp1->Quaternion = q1;
	}

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
			D3DXVECTOR3* syncPosition;
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

	bool passing1Min = (gameMaster->getGameTime() <= 60.0f);
	//テロップマネージャーの更新
	telopManager->update(frameTime);
	//テロップ発生フラグ
	//緑化状況10%
	if (treeManager->getGreeningRate() >= 0.1 &&
		!gameMaster->whetherAchieved(gameMasterNS::ACHIEVEMENT_GREENING_RATE_10 ))
	{
		if(!passing1Min)telopManager->playOrder(telopManagerNS::TELOP_TYPE0);
		gameMaster->setProgress(gameMasterNS::ACHIEVEMENT_GREENING_RATE_10);
		SerialCommunicationNS::send(SerialCommunicationNS::GREENING_10);
	}
	//緑化状況30%
	if (treeManager->getGreeningRate() >= 0.3 &&
		!gameMaster->whetherAchieved(gameMasterNS::ACHIEVEMENT_GREENING_RATE_30))
	{
		if (!passing1Min)telopManager->playOrder(telopManagerNS::TELOP_TYPE1);
		gameMaster->setProgress(gameMasterNS::ACHIEVEMENT_GREENING_RATE_30);
		SerialCommunicationNS::send(SerialCommunicationNS::GREENING_30);
	}
	//緑化状況50%
	if (treeManager->getGreeningRate() >= 0.5 &&
		!gameMaster->whetherAchieved(gameMasterNS::ACHIEVEMENT_GREENING_RATE_50))
	{
		if (!passing1Min)telopManager->playOrder(telopManagerNS::TELOP_TYPE2);
		gameMaster->setProgress(gameMasterNS::ACHIEVEMENT_GREENING_RATE_50);
		SerialCommunicationNS::send(SerialCommunicationNS::GREENING_50);
	}

	//スカイドームの更新
	sky->update();

	//海面の更新
	ocean->update();
	
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
	player1UI->update(treeManager->getGreeningRate()*100);
	player2UI->update(treeManager->getGreeningRate()*100);

	//カウントUIの更新
	countUI->update(frameTime);

	//レティクルの更新
	reticle->update(frameTime);

	//OPアナウンス
	announcement->update(frameTime);

	//マーカーの更新
	markerRenderer->update(frameTime);

	//ダメージUIの更新
	damageUI->update(frameTime);

	//エンディングが終了したらシーン遷移
	if(gameMaster->whetherAchieved(gameMasterNS::PASSING_GAME_ENDING))
	{
		// シーン遷移
		changeScene(nextScene);
	}

	//残り時間１分
	if (gameMaster->playActionRamaining1Min(gameMasterNS::PASSING_TELOP_ACTIVITY_LIMIT))
	{
		telopManager->playOrder(telopManagerNS::TELOP_TYPE4);	// 活動限界〜
	}
	if (gameMaster->playActionRamaining1Min(gameMasterNS::PASSING_TELOP_CANT_SENSE_GREEN))
	{
		telopManager->playOrder(telopManagerNS::TELOP_TYPE5);	// 緑化率が分からなくなってしまった〜
	}
	if (gameMaster->playActionRamaining1Min(gameMasterNS::PASSING_SE_HURRY_UP))
	{
		SoundInterface::SE->playSound(&playParameters[5]);	// ジングル
		SoundInterface::BGM->stopSound(playParameters[0]);
	}
	if (gameMaster->playActionRamaining1Min(gameMasterNS::PASSING_BGM_TEMP_REPLAY))
	{
		SoundInterface::BGM->playSound(&playParameters[0]);
	}
	if (gameMaster->playActionRamaining1Min(gameMasterNS::PASSING_SE_BGM_SPEED_UP))
	{
		SoundInterface::BGM->SetSpeed();					// BGM加速
	}

	//プレイヤー位置情報の送信
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		NETWORK_CLIENT::recordPlayerPosition(player[i].position, i);
	}

	//ディスプレイPCへ送信
	networkClient->send(gameMaster->getGameTime());

	//フェーダーテスト
	//if (input->wasKeyPressed('P'))
	//{
		//getFader()->setShader(faderNS::NORMAL);
		//getFader()->start();
	//}

	//リザルトへ強制遷移
	//後でプレゼンテーションフラグ判定を追加
	if (input->wasKeyPressed('1') || 
		input->getController()[0]->wasButton(virtualControllerNS::HOME)||
		input->getController()[1]->wasButton(virtualControllerNS::HOME))
	{
		changeScene(SceneList::RESULT);
	}

#ifdef _DEBUG
	if (input->wasKeyPressed('1'))
	{
		changeScene(SceneList::RESULT);
	}
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
		renderUI();
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

	//装飾フィールドの描画
	if (player[nowRenderingWindow].getState() == playerNS::STATE::VISION ||
		player[nowRenderingWindow].getState() == playerNS::STATE::SKY_VISION)
	{
		faceFieldRenderer->setStaticMesh(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_FACE_BLACK));
	}
	else {
		faceFieldRenderer->setStaticMesh(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_FACE));
	}
	faceFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);

	// プレイヤーの他のオブジェクトの描画
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		player[i].otherRender(currentCamera->view, currentCamera->projection, currentCamera->position);

	//アニメーションモデルの描画
	DrawMoveP();
	DrawMoveP1();

	//海面の描画
	ocean->render(currentCamera->view, currentCamera->projection, currentCamera->position);

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
	treeManager->render(currentCamera);

	// アイテムの描画
	itemManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// 風の描画
	windManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// マップオブジェクトの描画
	mapObjectManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	//3DUI
	if (gameMaster->whetherAchieved(gameMasterNS::PASSING_GAME_OPENING))
	{
		//レティクル3D描画
		if(player[nowRenderingWindow].getState() == playerNS::STATE::NORMAL)
			reticle->render3D(nowRenderingWindow,currentCamera);

		//マーカーの描画(2D/3D)両方とも描画
		markerRenderer->render(nowRenderingWindow, currentCamera);
	}

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
#if 1	// ナビゲーションメッシュのデバッグ描画
	naviMesh->debugRender(currentCamera->view, currentCamera->projection, currentCamera->position);
#endif
#endif //_DEBUG
}

//===================================================================================================================================
//【UI/2D描画】
//===================================================================================================================================
void Game::renderUI() 
{
	//OP中UI
	if (!gameMaster->whetherAchieved(gameMasterNS::PASSING_GAME_OPENING))
	{
		announcement->render();
		return;
	}

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定

	// テロップマネージャーの描画
	telopManager->render();

	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//固定UIの描画
	fixedUI->render();

	//プレイヤー1周りのUIの描画
	player1UI->render(gameMaster->getGameTime());

	//プレイヤー2周りのUIの描画
	player2UI->render(gameMaster->getGameTime());

	//カウントUIの描画
	countUI->render();
	if (reticleFlag)
	{
		//レティクルの描画
		reticle->render2D(&player[gameMasterNS::PLAYER_1P]);
		reticle->render2D(&player[gameMasterNS::PLAYER_2P]);
	}

	//ダメージUIの描画
	damageUI->render(gameMasterNS::PLAYER_1P);
	damageUI->render(gameMasterNS::PLAYER_2P);
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
	linear8TreeManager->registerObject(&tmp->getMin(), &tmp->getMax(), &tmp->treeCell);
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
		Enemy* enemy = enemyManager->getEnemyList()[i];
		// エネミー本体
		tree8Reregister(enemy);

		// TIGERのバレット
		if (enemy->getEnemyData()->type == enemyNS::TIGER)
		{
			Tiger* tiger = (Tiger*)enemy;
			for (int k = 0; k < tiger->getBulletMangaer()->getBulletList()->nodeNum; k++)
			{
				tree8Reregister(*tiger->getBulletMangaer()->getBulletList()->getValue(k));
			}
		}
		// BEARのパーツと枯れ木化範囲
		if (enemy->getEnemyData()->type == enemyNS::BEAR)
		{
			Bear* bear = (Bear*)enemy;
			if (bear->getIsMakingTreeDead())
			{
				tree8Reregister(bear->getDeadArea());
			}
			for (int k = 0; k < bearNS::PARTS_MAX; k++)
			{
				tree8Reregister(bear->getParts(k));
			}
		}
	}
	//木の登録
	for (int i = 0; i < treeManager->getTreeList().size(); i++)
	{
		tree8Reregister(treeManager->getTreeList()[i]);
		if (treeManager->getTreeList()[i]->isAroundGreening())
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

	//プレイヤーのダメージUIの処理
	for (int i = 0; i < gameMasterNS::PLAYER_NUM;i++)
	{
		if (player[i].getDamaged())
		{
			damageUI->damaged(i);
		}
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
		player[i].grounding(mesh, matrix);
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
			treeCylinder.centerLine.end		= tree->position + tree->getAxisY()->direction*10000.0f;//tree->size.y;
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
			// 描画の解除
			switch (itemList[i]->getItemData()->type)
			{
			case BATTERY:
				if (itemList[i]->sphereCollider.collide(player[j].getBodyCollide()->getCenter(),
					player[j].getRadius(), *itemList[i]->getMatrixWorld(), *player[j].getMatrixWorld()))
				{
					player[j].addpower(batteryNS::RECOVERY_POWER);	//電力加算
					PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Getlem, false ,NULL,false,NULL };
					SoundInterface::SE->playSound(&playParameters);	//SE再生
					itemManager->destroyItem(itemList[i]->getItemData()->itemID);					//デリート(今は全消し)
				}
				break;
			case EXAMPLE:break;
			case POWER_UP:
				if (itemList[i]->sphereCollider.collide(player[j].getBodyCollide()->getCenter(),
					player[j].getRadius(), *itemList[i]->getMatrixWorld(), *player[j].getMatrixWorld()))
				{
					player[j].powerup(2.0f);//パワーアップ
					PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_GetPowerUpItem, false ,NULL,false,NULL };
					SoundInterface::SE->playSound(&playParameters);	//SE再生
					// エフェクト再生 うまくいかん！うんちっち
					((Powerup*)itemList[i])->powUpEffect = new PowUpEffect(&player[j].position);
					effekseerNS::play(0, ((Powerup*)itemList[i])->powUpEffect);
					itemManager->destroyItem(itemList[i]->getItemData()->itemID);
				}
				break;
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
	ImGui::Text("collisionNum:%d", collisionNum);
	if (ImGui::CollapsingHeader("CollisionList"))
	{
		for (int i = 0; i < collisionNum; i++)
		{
			tmp1 = root[i * 2];
			tmp2 = root[i * 2 + 1];
			ImGui::Text("ID(%d):position(%.02f,%.02f,%.02f)<->ID(%d):position(%.02f,%.02f,%.02f)",
				tmp1->id, tmp1->position.x, tmp1->position.y, tmp1->position.z,
				tmp2->id, tmp2->position.x, tmp2->position.y, tmp2->position.z);
		}
	}
	networkClient->outputGUI();
	player->outputGUI();			//プレイヤー
	enemyManager->outputGUI();		//エネミー
	itemManager->outputGUI();		//アイテムマネージャ
	treeManager->outputGUI();		//ツリーマネージャー
	testField->outputGUI();			//テストフィールド
	camera->outputGUI();			//カメラ
	cameraOP->outputGUI();			//カメラ
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
		itemNS::ItemData power = { itemManager->issueNewItemID(),itemNS::POWER_UP, *player->getPosition() };
		itemManager->createItem(power);
	}
	if (input->wasKeyPressed('M'))
	{
		itemNS::ItemData power = { itemManager->issueNewItemID(),itemNS::POWER_UP, *player->getPosition() };
		itemManager->createItem(power);
	}

	if (input->wasKeyPressed('9'))
	{
		telopManager->playOrder(telopManagerNS::TELOP_TYPE0);
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

	//if (input->wasKeyPressed('6'))
	//{
	//	aiDirector->eventMaker.makeEventSpawningEnemyAroundPlayer(0);

	//	//aiDirector->eventMaker.makeEventEnemyAttaksTree();
	//}

	if (input->wasKeyPressed('Z'))
	{
		//player->position = D3DXVECTOR3(20, 5, 0);
		//((Powerup*)itemList[i])->powUpEffect = 
		effekseerNS::play(0, new PowUpEffect(&player[0].position));
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
