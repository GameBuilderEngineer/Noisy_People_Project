//===================================================================================================================================
//【Tutorial.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Tutorial.h"

//===================================================================================================================================
// 【using宣言】
//===================================================================================================================================
using namespace tutorialNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Tutorial::Tutorial()
{
	//線形８分木空間分割管理クラス
	linear8TreeManager = new Linear8TreeManager<Object>;
	linear8TreeManager->initialize(4, D3DXVECTOR3(-1000, -500, -1000), D3DXVECTOR3(1000, 1000, 1000));

	//オブジェクトカウンターのリセット
	objectNS::resetCounter();

	// 現在のシーン(チュートリアル)
	sceneName = "Scene -Tutorial-";
	// 次のシーン(タイトル)
	nextScene = SceneList::TITLE;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Tutorial::~Tutorial()
{
	// サウンドの停止
	//sound->stop(soundNS::TYPE::BGM_TUTORIAL);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Tutorial::initialize()
{
	//テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_NAVMESH));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//player
	player = new Player[gameMasterNS::PLAYER_NUM];
	maleRenderer = new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_MALE));
	femaleRenderer = new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_FEMALE));

	//camera
	camera = new Camera[gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//カメラの設定
		camera[i].initialize(WINDOW_WIDTH / 2, WINDOW_HEIGHT);
		camera[i].setTarget(player[i].getPosition());
		camera[i].setTargetX(&player[i].getAxisX()->direction);
		camera[i].setTargetY(&player[i].getAxisY()->direction);
		camera[i].setTargetZ(&player[i].getAxisZ()->direction);
		camera[i].setRelative(CAMERA_RELATIVE_QUATERNION);
		camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
		camera[i].setRelativeGaze(CAMERA_RELATIVE_GAZE);
		camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
		camera[i].setFieldOfView((D3DX_PI / 180) * 90);

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

		//進捗
		step[i] = tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_1;
		planeStep[i] = tutorialPlaneNS::TUTORIAL_PLANE_ID::PLANE_ENEMY;
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

	//スカイドームの初期化
	sky = new Sky();

	//アニメションキャラの初期化
	InitMoveP(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.04f, 0.04f, 0.04f), true);

	// エネミー
	enemyManager = new EnemyManager;
	enemyManager->initialize(sceneName,testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld(),player);
	//test(sai[マージする時に消して])
	enemyNS::ENEMYSET enemySet = { 0 };
	enemySet.defaultDirection = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	enemySet.defaultPosition = D3DXVECTOR3(-3.0f, 1.5f, 24.5f);
	enemySet.defaultState = stateMachineNS::ENEMY_STATE::CHASE;
	enemySet.type = enemyNS::ENEMY_TYPE::WOLF;
	enemySet.enemyID = 0;
	enemyData = enemyManager->createEnemyData(enemySet);
	enemyManager->createEnemy(enemyData);

	//ディスプレイプレーン
	D3DXVECTOR3 enemyPlanePos = enemySet.defaultPosition;
	enemyPlanePos.y += 5;
	plane = new TutorialPlane*[gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		plane[i] = new TutorialPlane(enemyPlanePos);
	}

	//UI
	tutorialUI = new TutorialUI;

	// サウンドの再生
	//sound->play(soundNS::TYPE::BGM_GAME, soundNS::METHOD::LOOP);

	//テキストの初期化
	//text.initialize(direct3D9->device,10,10, 0xff00ff00);
	//text2.initialize(direct3D9->device,11,11, 0xff0000ff);

	//タイマー
	timer = new Timer;
	timer->initialize();

	// チュートリアル2D初期化
	tutorialTex.initialize();

#ifdef _DEBUG
	enemyManager->setDebugEnvironment(camera, &player[gameMasterNS::PLAYER_1P]);
	playerSelect = gameMasterNS::PLAYER_1P;
#endif
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Tutorial::uninitialize()
{
	SAFE_DELETE(linear8TreeManager);
	SAFE_DELETE_ARRAY(player);
	SAFE_DELETE_ARRAY(camera);
	SAFE_DELETE(light);
	SAFE_DELETE(testField);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(maleRenderer);
	SAFE_DELETE(enemyManager);
	SAFE_DELETE(femaleRenderer);
	SAFE_DELETE(sky);
	SAFE_DELETE(timer);
	SAFE_DELETE(tutorialUI);

	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		SAFE_DELETE(plane[i]);
	}
	SAFE_DELETE_ARRAY(plane);

	UninitMoveP();

	tutorialTex.uninitialize();
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Tutorial::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//テストフィールドの更新
	testField->update();			//オブジェクト
	testFieldRenderer->update();	//レンダラー

	//プレイヤーの更新
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{		
		//オブジェクト
		player[i].update(frameTime);	

		//ディスプレイプレーン
		plane[i]->update(frameTime, planeStep[i]);
	}

	maleRenderer->update();				//レンダラー
	femaleRenderer->update();			//レンダラー

	// エネミーの更新
	enemyManager->update(frameTime);
	//for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	//	enemyManager->footsteps(*player[i].getPosition(), i);		//足音

	UpdateMoveP(0.01f);

	//スカイドームの更新
	sky->update();

	//電力減少（電力回復確認用）
	player->pullpower(1);

	//カメラの更新
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		camera[i].update();

	//タイマーの更新
	timer->update();


	//Enterまたは〇ボタンで次へ
	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[PLAYER1]->wasButton(virtualControllerNS::A) ||
		input->getController()[PLAYER2]->wasButton(virtualControllerNS::A))
	{
		// サウンドの再生
		//sound->play(soundNS::TYPE::SE_PAPER, soundNS::METHOD::PLAY);

		//チュートリアルを次へ
		tutorialTex.nextPage++;

		return;
	}
	//Backまたは×ボタンで前へ
	else if (input->wasKeyPressed(VK_BACK)||
		input->getController()[PLAYER1]->wasButton(virtualControllerNS::B) ||
		input->getController()[PLAYER2]->wasButton(virtualControllerNS::B))
		
	{
		// サウンドの再生
		//sound->play(soundNS::TYPE::SE_CANCEL, soundNS::METHOD::PLAY);

		if (tutorialTex.nextPage > 0)
		{
			// チュートリアルを前へ
			tutorialTex.nextPage--;
		}
		return;
	}

	//Deleteまたは特殊メインボタンでタイトルへ
	if (input->wasKeyPressed(VK_DELETE) ||
		input->getController()[PLAYER1]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
		input->getController()[PLAYER2]->wasButton(virtualControllerNS::SPECIAL_MAIN))
	{
		changeScene(nextScene);
	}

	// チュートリアルが最後まで行ったらタイトルへ
	if (tutorialTex.nextPage >= tutorialTex::TUTORIAL_2D_SCENE_MAX)
	{
		//changeScene(nextScene);
	}

	// チュートリアル2D更新
	tutorialTex.update();

}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Tutorial::render()
{
	//1Pカメラ・ウィンドウ・エフェクシアーマネージャー
	camera[gameMasterNS::PLAYER_1P].renderReady();
	direct3D9->changeViewport1PWindow();
	render3D(camera[gameMasterNS::PLAYER_1P], gameMasterNS::PLAYER_1P);
	effekseerNS::setCameraMatrix(
		camera[gameMasterNS::PLAYER_1P].position,
		camera[gameMasterNS::PLAYER_1P].gazePosition,
		camera[gameMasterNS::PLAYER_1P].upVector);
	effekseerNS::render();

	//2Pカメラ・ウィンドウ・エフェクシアーマネージャー
	camera[gameMasterNS::PLAYER_2P].renderReady();
	direct3D9->changeViewport2PWindow();
	render3D(camera[gameMasterNS::PLAYER_2P], gameMasterNS::PLAYER_2P);
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
void Tutorial::render3D(Camera currentCamera, int playerID)
{
	//テストフィールドの描画
//testField->setAlpha(0.1f); 
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	// プレイヤーの描画
	maleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	femaleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	// プレイヤーの他のオブジェクトの描画
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		player[i].otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//スカイドームの描画
	sky->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	DrawMoveP();

	// エネミーの描画
	enemyManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	//ディスプレイ用プレーンサンプル
	plane[playerID]->render(currentCamera.view, currentCamera.projection, currentCamera.position);

#if _DEBUG
	//4分木空間分割のライン描画
	//linear4TreeManager->render();
	//8分木空間分割のライン描画
	linear8TreeManager->render();
	Ray ray;
	ray.color = D3DXCOLOR(255, 255, 0, 255);
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

}

//===================================================================================================================================
//【2D[UI]描画】
//===================================================================================================================================
void Tutorial::renderUI()
{
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定

	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//UI
	tutorialUI->render();

	//タイマーの描画
	timer->render();

	//チュートリアル2D描画
	tutorialTex.render();
}

//===================================================================================================================================
//【線形８分木空間分割マネージャー再登録処理】
//===================================================================================================================================
void Tutorial::tree8Reregister(Object* tmp)
{
	//一度リストから外れる
	tmp->treeCell.remove();
	//再登録
	linear8TreeManager->registerObject(&tmp->getMin(), &tmp->getMax(), &tmp->treeCell);
}

//===================================================================================================================================
//【衝突処理】
//===================================================================================================================================
void Tutorial::collisions()
{
	//再登録
	tree8Reregister(&player[gameMasterNS::PLAYER_1P]);
	tree8Reregister(&player[gameMasterNS::PLAYER_2P]);

	//衝突対応リストを取得
	collisionNum = linear8TreeManager->getAllCollisionList(&collisionList);
	collisionNum /= 2;//2で割るのはペアになっているため
}

//===================================================================================================================================
//【AI処理】
//===================================================================================================================================
void Tutorial::AI()
{
	//none
}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
#ifdef _DEBUG
void Tutorial::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::SliderInt("PLAYER", &playerSelect, gameMasterNS::PLAYER_1P, gameMasterNS::PLAYER_2P, "");

	ImGui::SliderInt("STEP", &step[playerSelect], tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_1, tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_5, "");
	if (tutorialUI->getStep(playerSelect) != step[playerSelect])
	{
		tutorialUI->setStep(playerSelect, step[playerSelect]);
	}

	ImGui::SliderInt("PLANE STEP", &planeStep[playerSelect], tutorialPlaneNS::TUTORIAL_PLANE_ID::PLANE_ENEMY, tutorialPlaneNS::TUTORIAL_PLANE_ID::PLANE_CLEAR, "");
	if (plane[playerSelect]->getPlaneStep() != planeStep[playerSelect])
	{
		plane[playerSelect]->update(frameTime, planeStep[playerSelect]);
	}
}
#endif // _DEBUG
