//===================================================================================================================================
//【Result.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/12/05
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Result.h"
#include "Sound.h"
#include "UtilityFunction.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace resultNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Result::Result(void)
{
	// 今のシーン( リザルト )
	sceneName = ("Scene -Result-");

	// 次のシーン( タイトル )
	nextScene = SceneList::FINALE;

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Result::~Result(void)
{
	// サウンドの停止
	SoundInterface::BGM->uninitSoundStop();

}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Result::initialize()
{
	//リザルトUIの初期化
	resultUI.initialize();
	
	resultUI.greenigPersent =gameMaster->getGreeningRate();//全体緑化率
	resultUI.greeningNum01 =gameMaster->getGreeningTreeNum(basicUiNS::P1);//player1の緑化本数
	resultUI.greeningNum02 = gameMaster->getGreeningTreeNum(basicUiNS::P2);//player2の緑化本数
	resultUI.defeat01 = gameMaster->getKillEnemyNum(basicUiNS::P1);//player1の撃破数
	resultUI.defeat02 = gameMaster->getKillEnemyNum(basicUiNS::P2);//player2の撃破数

	//テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//camera
	camera = new Camera;
	//カメラの設定
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera->setTarget(&testField->position);
	camera->setTargetX(&testField->getAxisX()->direction);
	camera->setTargetY(&testField->getAxisY()->direction);
	camera->setTargetZ(&testField->getAxisZ()->direction);
	camera->setRelative(CAMERA_RELATIVE_QUATERNION);
	camera->setGazeDistance(400.0f);
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI / 180) * 90);
	camera->setLimitRotationTop(0.1f);
	camera->setLimitRotationBottom(0.1f);

	//エフェクシアーの設定
	effekseerNS::setProjectionMatrix(0,
		camera->fieldOfView,
		(float)camera->windowWidth,
		(float)camera->windowHeight,
		camera->nearZ,
		camera->farZ);

	// ツリー
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	treeManager->createUsingTool();
	treeManager->switchingNormalView(gameMasterNS::PLAYER_1P);
	
	//リプレイタイマー
	playbackTimer = 0.0f;

	// UIにゲームマスターを渡しておく
	resultUI.gameMaster = gameMaster;
	// サウンドの再生
	//PLAY_PARAMETERS playParameters = { 0 };//同時に再生したい数
	////再生する曲の指定サウンドID,ループ,スピードNULLでしない,基本false,基本NULL,フィルターを使うか使わないか
	//playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Game, true,1.1f,false,NULL };//BGMの設定
	////再生
	//SoundInterface::BGM->playSound(&playParameters);;

	//背景ビルボード
	backGround = new InstancingBillboard();
	backGround->setRenderType(InstancingBillboardNS::NORMAL_PASS);
	backGround->initialize(*textureNS::reference(textureNS::RESULT_BG));
	backGroundInstance = new InstancingBillboardNS::Instance();
	backGroundInstance->scale = D3DXVECTOR2(1920.0f,1080.0f);
	backGround->generateInstance(backGroundInstance);
	backGroundDistance = 886.0f;
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Result::uninitialize(void)
{
	SAFE_DELETE(treeManager);
	SAFE_DELETE(camera);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(testField);
	SAFE_DELETE(backGround);
	resultUI.uninitialize();

}
//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Result::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//リプレイタイマー
	playbackTimer += frameTime*PLAYBACK_SPEED;

	//UIの更新処理
	resultUI.update(frameTime);

	//テストフィールドの更新
	testField->update();			//オブジェクト
	testFieldRenderer->update();	//レンダラー

	//ゲームマスターからイベントを取得し、再生
	if (gameMaster->getEventList(NULL,playbackTimer) > 0)
	{
		TreeTable* eventList = NULL;	//イベントリスト
		int eventNum;					//イベント数

		//イベントを取得
		eventNum = gameMaster->getEventList(&eventList,playbackTimer);

		//取得したイベントを実行する
		for (int num = 0; num < eventNum; num++)
		{
			//イベント対象のツリー
			Tree* selectTree = NULL;
			
			//イベント対象のツリーを選択
			for (int i = 0; i < treeManager->getTreeList().size(); i++)
			{
				if (selectTree)continue;//選択済みならスルー
				Tree* tree = treeManager->getTreeList()[i];
				//イベント対象ツリーをIDにより検索
				if (tree->getTreeData()->treeID == eventList[num].id)
				{
					selectTree = tree;
					i = treeManager->getTreeList().size();//検索終了
				}
			}
			
			//イベントの対象ツリーが存在しない場合はスルー
			if (selectTree == NULL)continue;

			//イベント別にアクションをする
			switch (eventList[num].eventType)
			{
			case gameMasterNS::TO_DEAD:
				selectTree->transState();
				break;
			case gameMasterNS::TO_GREEN_WITH_ANALOG:
				selectTree->transState();
				break;
			case gameMasterNS::TO_GREEN_WITH_DIGITAL:
				selectTree->transState();
				break;
			}

		}

		//取得したイベントリストを削除
		SAFE_DELETE_ARRAY(eventList);
	}
	//ツリーマネージャーの更新
	treeManager->update(frameTime);

	if (resultUI.resultPhase == resultUiNS::PHASE_05)
	{
		if (gameMaster->getGreeningRate() >= 70)
		{
			SerialCommunicationNS::send(SerialCommunicationNS::CLEAR);

		}
		else {
			SerialCommunicationNS::send(SerialCommunicationNS::FAILURE);
		}
	}



	//リザルトフェイズが5の時のみ Enterまたは〇ボタンでタイトルへ
	if (resultUI.resultPhase == resultUiNS::PHASE_05&&
		input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		// シーン遷移
		changeScene(nextScene);
	}

	//カメラの更新
	{
		float rate = (sinf(sceneTimer*6)/2.0f) + 0.5f;
		float fov = UtilityFunction::lerp((D3DX_PI / 180) * 70, (D3DX_PI / 180) * 90, 0.5);
		camera->setFieldOfView(fov);
		camera->rotation(D3DXVECTOR3(0, 1, 0), CAMERA_SPEED*frameTime);
		camera->update();
	}

	//背景の更新
	backGroundInstance->position = *camera->target + camera->getDirectionZ()*backGroundDistance;
	backGround->update(0.0f);

}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Result::render()
{
	//描画対象をウィンドウ全体に切替
	direct3D9->changeViewportFullWindow();

	//カメラの描画準備
	camera->renderReady();

	//3D描画
	render3D(*camera);

	//エフェクシアーの描画
	effekseerNS::setCameraMatrix(
		0,
		camera->position,
		camera->gazePosition,
		camera->upVector);
	effekseerNS::render(0);

	//UI
	renderUI();
}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
void Result::render3D(Camera currentCamera)
{
	//背景の描画
	backGround->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	//テストフィールドの描画
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	//ツリーの描画
	treeManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

}

//===================================================================================================================================
//【2D[UI]描画】
//===================================================================================================================================
void Result::renderUI()
{
	//お試し用のフラグこいつがtrueになると2DUIの描画開始
	bool testFlag = true;
	// リザルトUI
	resultUI.render(testFlag);
}

//===================================================================================================================================
//【衝突処理】
//===================================================================================================================================
void Result::collisions(void)
{
	// None
}

//===================================================================================================================================
//【AI処理】
//===================================================================================================================================
void Result::AI(void)
{
	// None
}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
#ifdef _DEBUG
void Result::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("playbackTimer = %f", playbackTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::InputFloat("backGroundDistance", &backGroundDistance);

}
#endif // _DEBUG
