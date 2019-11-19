//===================================================================================================================================
//【Game.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/11/13
//===================================================================================================================================

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

	//シーンの更新
	SoundInterface::SwitchAudioBuffer(SceneList::GAME);

	//再生パラメータ
	PLAY_PARAMETERS playParameters[2];
	memset(playParameters, 0, sizeof(playParameters));
	FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Decision, false ,NULL,false,NULL, true, filterParameters };
	playParameters[1] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Game, true,1.0f,false,NULL,true, filterParameters };
	

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
	SoundInterface::StopAllSound();
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Game::initialize() {

	//テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//player
	player				= new Player[gameMasterNS::PLAYER_NUM];
	maleRenderer		= new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_MALE));
	femaleRenderer		= new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_FEMALE));

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
		camera[i].setLimitRotationTop(0.3f);
		camera[i].setLimitRotationBottom(0.7f);

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

		//重力を設定
		player[i].configurationGravityWithRay(
			testField->getPosition(),
			testFieldRenderer->getStaticMesh()->mesh,
			testField->getMatrixWorld());
	}

	//エフェクシアーの設定
	effekseerNS::setProjectionMatrix(
		camera[0].fieldOfView, 
		(float)camera[0].windowWidth, 
		(float)camera[0].windowHeight, 
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
	//海面の初期化
	ocean = new Ocean();

	//アニメションキャラの初期化
	InitMoveP(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.006f, 0.006f, 0.006f), true);


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
	ad = new Advertisement();

	// エネミー
	enemyManager = new EnemyManager;
	enemyManager->initialize(*getSceneName(),testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld(), player);

	// ツリー
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// アイテム
	itemManager = new ItemManager;
	itemManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// 風
	windManager = new WindManager;
	windManager->initialize(*getSceneName(), testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	//// マップオブジェクト
	mapObjectManager = new MapObjectManager;
	mapObjectManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// テロップ
	/*telop = new Telop;
	telop->initialize();*/

	//テロップマネージャー
	telopManager = new TelopManager;
	telopManager->initialize();

	// AI
	aiDirector = new AIDirector;
	aiDirector->initialize(gameMaster, player, enemyManager, treeManager, itemManager, NULL);
	naviMesh = new NavigationMesh(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
	naviMesh->initialize();

	//固定されたUI
	fixedUI = new FixedUI;
	fixedUI->initialize();

	//プレイヤー周りのUI
	playerUI = new PlayerUI;
	playerUI->initialize(&player[gameMasterNS::PLAYER_1P]);

	//レティクル
	reticle = new Reticle();
	reticle->setAimingPosition1(player[gameMasterNS::PLAYER_1P].getAiming());
	reticle->setAimingPosition2(player[gameMasterNS::PLAYER_2P].getAiming());

	//Sprite実験
	spriteGauge = new SpriteGauge;
	spriteGauge->initialize();
#ifdef _DEBUG
	// デバッグエネミーモードにするための準備
	enemyManager->setDebugEnvironment(camera, &player[gameMasterNS::PLAYER_1P]);
#endif // _DEBUG
	// エネミーをランダムに設置する
	for (int i = 0; i < enemyNS::ENEMY_OBJECT_MAX; i++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(rand() % 400, 150, rand() % 480);
		pos -= D3DXVECTOR3(200, 0, 240);
		enemyNS::ENEMYSET tmp =
		{
			enemyManager->issueNewEnemyID(),
			rand() % enemyNS::ENEMY_TYPE::TYPE_MAX,
			stateMachineNS::PATROL,
			pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		};
		enemyNS::EnemyData* p = enemyManager->createEnemyData(tmp);
		enemyManager->createEnemy(p);
	}

	// ツリーをランダムに設置する
	treeNS::TreeData treeData;
	treeData.treeID = treeManager->issueNewTreeID();
	treeData.hp = 0;
	treeData.type = treeNS::ANALOG_TREE;
	treeData.size = treeNS::STANDARD;
	treeData.greenState = treeNS::DEAD;
	treeData.model = treeNS::B_MODEL;
	for (int i = 0; i < 50; i++)
	{
		treeData.initialPosition =
			D3DXVECTOR3(rand() % 400, 150, rand() % 480);
		treeData.initialPosition -= D3DXVECTOR3(200, 0, 240);
		
		treeManager->createTree(treeData);
	}


	
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
	SAFE_DELETE(playerUI);
	SAFE_DELETE(ad);

	UninitMoveP();

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Game::update(float _frameTime) {

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//gameMaster処理落ちやポーズに関わらない更新
	gameMaster->update(frameTime);


	//【処理落ち】
	//フレーム時間が約10FPS時の時の時間より長い場合は、処理落ち（更新しない）
	//※フレーム時間に準拠している処理が正常に機能しないため
	if (frameTime > 0.10)return;

	//ゲームタイムの更新
	gameMaster->updateGameTime(frameTime);


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
	//for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		//enemyManager->footsteps(*player[i].getPosition(), i);		//足音

	// ツリーの更新
	treeManager->update(frameTime);

	// アイテムの更新
	itemManager->update(frameTime);

	// 風の更新
	windManager->update(frameTime);

	// マップオブジェクトの更新
	mapObjectManager->update(frameTime);

	UpdateMoveP(0.01f);

	//キャラクターの場所と回転の連携
	MOVEP *mp = GetMovePAdr();
	mp->Pos = player->position;
	D3DXQUATERNION q = player->quaternion;
	Base::anyAxisRotation(&q,D3DXVECTOR3(0,1,0),180);
	mp->Quaternion = q;

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
			D3DXVECTOR3 * syncPosition;
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


	//テロップマネージャーの更新
	telopManager->update(frameTime);
	//テロップ発生フラグ
	if (input->wasKeyPressed('L'))
	{
		//telopManager->play(telopManagerNS::TELOP_INFO_BAR);
		telopManager->play(telopManagerNS::TELOP_TYPE0);
	}
	if (input->wasKeyPressed('K'))
	{
		//telopManager->play(telopManagerNS::TELOP_INFO_BAR);
		telopManager->play(telopManagerNS::TELOP_TYPE1);
	}
	

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
	//海面の更新
	ocean->update();

	//エフェクト（インスタンシング）テスト
	//testEffect->update(frameTime);

	//ディスプレイ用プレーンサンプル
	samplePlane->update(frameTime);
	// 開発中広告
	ad->update(frameTime);

	//電力減少（電力回復確認用）
	//player->pullpower(1);

	//カメラの更新
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		camera[i].update();

	//固定UIの更新
	fixedUI->update();
	gameMaster->getGameTime();//←ソメヤくんへ：ゲームタイムです。


	//プレイヤー周りのUIの更新
	playerUI->update();

	//レティクルの更新
	reticle->update(frameTime);


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
	nowRenderingWindow = gameMasterNS::PLAYER_1P;
	camera[gameMasterNS::PLAYER_1P].renderReady();
	direct3D9->changeViewport1PWindow();
	render3D(camera[gameMasterNS::PLAYER_1P]);
	effekseerNS::setCameraMatrix(
		camera[gameMasterNS::PLAYER_1P].position, 
		camera[gameMasterNS::PLAYER_1P].gazePosition, 
		camera[gameMasterNS::PLAYER_1P].upVector);
	effekseerNS::render();

	//2Pカメラ・ウィンドウ・エフェクシアーマネージャー
	nowRenderingWindow = gameMasterNS::PLAYER_2P;
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
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	// プレイヤーの描画
	maleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	//femaleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	// プレイヤーの他のオブジェクトの描画
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		player[i].otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);
	//アニメーションモデルの描画
	DrawMoveP();

	////木の描画
	//deadTree->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	////木Aの描画
	//treeA->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	////木Bの描画
	//treeB->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	////石の描画
	//stone->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//スカイドームの描画
	sky->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//海面の描画
	ocean->render(currentCamera.view, currentCamera.projection, currentCamera.position);


	// エネミーの描画
	enemyManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// ツリーの描画
	treeManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// アイテムの描画
	itemManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// 風の描画
	windManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// マップオブジェクトの描画
	mapObjectManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	//エフェクト（インスタンシング）テスト
	//testEffect->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	//ディスプレイ用プレーンサンプル
	samplePlane->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// 開発中広告
	ad->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	//レティクル3D描画
	reticle->render3D(nowRenderingWindow,currentCamera.view, currentCamera.projection, currentCamera.position);

#if _DEBUG
	//4分木空間分割のライン描画
	//linear4TreeManager->render();
	//8分木空間分割のライン描画
	//linear8TreeManager->render();
	Ray ray;
	ray.color = D3DXCOLOR(150, 150, 0, 255);
	Object** root = collisionList->getRoot();
	Object* tmp1 = NULL;
	Object* tmp2 = NULL;
	D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0);
	float length = 0;
	for (int i = 0; i < collisionNum; i++)
	{
		tmp1 = root[i * 2];
		tmp2 = root[i * 2 + 1];
		length = Base::between2VectorDirection(&direction, tmp1->position, tmp2->position);
		ray.initialize(tmp1->position, direction);
		ray.render(length);
	}
#endif

#ifdef _DEBUG
#if 0	// ナビゲーションメッシュのデバッグ描画
	naviMesh->debugRender(currentCamera.view, currentCamera.projection, currentCamera.position);
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

	//プレイヤー周りのUIの描画
	playerUI->render();

	//レティクルの描画
	reticle->render2D();
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
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		windManager->windCollision(&player[i]);
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
				FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
				PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_EnemyAttack, false ,NULL,false,NULL,true, filterParameters };
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
	//エネミー増殖し続けるのでコメントアウト
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

	player->outputGUI();			//プレイヤー
	enemyManager->outputGUI();		//エネミー
	itemManager->outputGUI();		// アイテムマネージャ
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

	// デバッグエネミーで'7','8'使用中
	// マップオブジェクトマネージャーのテスト
	if (input->wasKeyPressed('7'))	// 作成
	{
		mapObjectNS::MapObjectData mapObjectData;
		mapObjectData.zeroClear();
		mapObjectData.mapObjectID = mapObjectManager->issueNewMapObjectID();
		mapObjectData.type = mapObjectNS::STONE_01;
		mapObjectData.defaultPosition = *player->getPosition();
		mapObjectManager->createMapObject(mapObjectData);
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
}
#endif // _DEBUG
