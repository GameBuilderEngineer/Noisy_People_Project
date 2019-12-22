#include "DebugScene.h"
#include "CollisionManager.h"

using namespace debugNS;

//=============================================================================
// コンストラクタ
//=============================================================================
DebugScene::DebugScene()
{
	//線形８分木空間分割管理クラス
	linear8TreeManager = new Linear8TreeManager<Object>;
	linear8TreeManager->initialize(5, D3DXVECTOR3(-3000, -2000, -3000), D3DXVECTOR3(3000, 3000, 3000));

	//オブジェクトカウンターのリセット
	objectNS::resetCounter();

	sceneName = "Scene -Debug-";
	nextScene = SceneList::SPLASH;
}


//=============================================================================
// デストラクタ
//=============================================================================
DebugScene::~DebugScene()
{

}


//=============================================================================
// 初期化
//=============================================================================
void DebugScene::initialize()
{
	// テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	// プレイヤー
	player = new Player[gameMasterNS::PLAYER_NUM];
	maleRenderer = new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_MALE));
	femaleRenderer = new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_FEMALE));

	// ツリー
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	treeManager->setGameMaster(gameMaster);

	// ツリーをツール情報を元に設置する
	treeManager->createUsingTool();

	// camera
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
		camera[i].setFieldOfView((D3DX_PI / 180) * 90);
		camera[i].setLimitRotationTop(0.1f);
		camera[i].setLimitRotationBottom(0.3f);
		camera[i].updateOrtho();

		// プレイヤーの設定
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

		// カメラポインタのセット
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
		effekseerNS::setProjectionMatrix(i + 1,
			camera[i].fieldOfView,
			(float)camera[i].windowWidth,
			(float)camera[i].windowHeight,
			camera[i].nearZ,
			camera[i].farZ);
	}

	// エフェクシアーの設定
	effekseerNS::setProjectionMatrix(0,
		camera[0].fieldOfView,
		(float)camera[0].windowWidth,
		(float)camera[0].windowHeight,
		camera[0].nearZ,
		camera[0].farZ);

	// light
	light = new Light;
	light->initialize();

	// ゲーム開始時処理
	gameMaster->startGame();
	gameMaster->setTreeNum(treeManager->getTreeNum());
}


//=============================================================================
// 終了
//=============================================================================
void DebugScene::uninitialize()
{
	SAFE_DELETE(testField);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE_ARRAY(player);
	SAFE_DELETE(maleRenderer);
	SAFE_DELETE(femaleRenderer);
	SAFE_DELETE(treeManager);

}


//=============================================================================
// 更新
//=============================================================================
void DebugScene::update(float _frameTime)
{

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//テストフィールドの更新
	testField->update();			//オブジェクト
	testFieldRenderer->update();	//レンダラー

	//プレイヤーの更新
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		player[i].update(frameTime);
	}

	maleRenderer->update();					//レンダラー
	femaleRenderer->update();				//レンダラー

	// ツリーの更新
	treeManager->update(frameTime);

	//カメラの更新
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		camera[i].update();
	}
}


//=============================================================================
// 描画
//=============================================================================
void DebugScene::render()
{
	direct3D9->changeViewportFullWindow();
	render3D(&camera[gameMasterNS::PLAYER_1P]);
}


//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
void DebugScene::render3D(Camera* currentCamera) {

	//テストフィールドの描画
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);

	//// プレイヤーの描画
	//maleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);

	//// ツリーの描画
	//if (player[nowRenderingWindow].getState() == playerNS::STATE::VISION ||
	//	player[nowRenderingWindow].getState() == playerNS::STATE::SKY_VISION)
	//{
	//	treeManager->switchingVisionView(nowRenderingWindow);
	//}
	//else {
	//	treeManager->switchingNormalView(nowRenderingWindow);
	//}
	//treeManager->render(currentCamera);
}


//===================================================================================================================================
//【線形８分木空間分割マネージャー再登録処理】
//===================================================================================================================================
void DebugScene::tree8Reregister(Object* tmp)
{
	//一度リストから外れる
	tmp->treeCell.remove();
	//再登録
	linear8TreeManager->registerObject(&tmp->getMin(), &tmp->getMax(), &tmp->treeCell);
}


//=============================================================================
// 衝突
//=============================================================================
void DebugScene::collisions()
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
		player[i].insetCorrection(objectNS::AXIS_X, player[i].size.x / 2, mesh, matrix);
		player[i].insetCorrection(objectNS::AXIS_RX, player[i].size.x / 2, mesh, matrix);
		player[i].insetCorrection(objectNS::AXIS_Z, player[i].size.z / 2, mesh, matrix);
		player[i].insetCorrection(objectNS::AXIS_RZ, player[i].size.z / 2, mesh, matrix);
		//照準レイ更新/姿勢更新/狙撃レイ更新
		player[i].updateAiming(mesh, matrix);
		player[i].updatePostureByAiming();
		player[i].updateShooting(mesh, matrix);
	}
}


//=============================================================================
// AI
//=============================================================================
void DebugScene::AI()
{

}


#ifdef _DEBUG
void DebugScene::createGUI()
{
	treeManager->outputGUI();		//ツリーマネージャー

}
#endif
