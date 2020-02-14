//===================================================================================================================================
//【Display.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/12/10
// [更新日]2019/12/10
//===================================================================================================================================

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//！蔡へ：まずはここを読んで～！
//サーバー		：中央のモニター側→アプリ起動時ディスプレイモードを選択しdisplayシーンを更新。
//クライアント	：ゲームプレイ側→アプリ起動時にゲームモードを選択し、splashシーンから開始し、サーバーへ情報を送信し続ける。
//上記について逆の方が良い場合は、それでもいいです。
//
//【通信確認達成目標】
//①通信の確立		：通信ができる状態であることを確認するために、シーン遷移を行う。
//②通信のずれ確認	：どれほどラグがあるか視認するために、タイマーの同期テストを行う。
//③通信量の確認	：どれくらいのデータを送れるかテストする。
//④実際に木を生やす：ゲームの進行に合わせて、実際に木をはやす。
//
//【絶対に必要な機能】
//①IPアドレスの入力機能
//②ゲームプレイ中は、絶対に通信状況の影響を受けない
//　→例）通信ができていなくてもゲームはプレイできる。
//　→例）通信が切断されてもゲームはプレイできる。
//　→例）通信が途中から開始されてもゲームはプレイできる。などなど
//③ゲームプレイ中に接続ができるようにする。→アプリケーション起動時だけでなく、プレイ中も接続できるようにする。
//④通信プレイのエラーが起きた場合に、ゲームを妨げない。
//⑤アプリケーション開始時に、モード選択ができる。
//
//思いつきで書いてるんで、絶対ではないし、疑問点や問題点がある場合は必ず相談してください。
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Display.h"
#include "UtilityFunction.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace DisplayNS;


//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Display::Display()
{
	// 今のシーン( リザルト )
	sceneName = ("Scene -Display-");

	// 次のシーン( タイトル )
	nextScene = SceneList::DISPLAY;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Display::~Display()
{
}


//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Display::initialize()
{
	syncTimer = 0.0f;
	//テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_FACE));
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
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI / 180) * 90);
	camera->setLimitRotationTop(0.1f);
	camera->setLimitRotationBottom(0.1f);
	camera->setGazeDistance(400.0f);

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

	//スカイドームの初期化
	sky = new Sky();
	//海面の初期化
	ocean = new Ocean();

	//マーカー
	markerRenderer = new MarkerRenderer;
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		playerPosition[i] = D3DXVECTOR3(0, 0, 0);
		markerRenderer->playerPosition[i] = &playerPosition[i];
	}

	networkServer = new NETWORK_INTERFACE;
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Display::uninitialize(void)
{
	SAFE_DELETE(markerRenderer);
	SAFE_DELETE(treeManager);
	SAFE_DELETE(camera);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(testField);
	SAFE_DELETE(networkServer);
	SAFE_DELETE(sky);
	SAFE_DELETE(ocean);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Display::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	PACKAGE *package = networkServer->updata();

	//同期タイマー：ゲームプレイ（クライアント）時更新
	if (package->networkTester == true)
	{
		//タイマーの同期
		syncTimer = package->timer;

		//プレイヤー位置情報の同期
		if (package->record1P)playerPosition[gameMasterNS::PLAYER_1P] = package->pos1P;
		if (package->record2P)playerPosition[gameMasterNS::PLAYER_2P] = package->pos2P;

		if (package->treeMax != 0)
		{
			//パッケージ内のイベントを呼び出す
			for (int i = 0; i < package->treeMax; i++)
			{
				//記録されていない場合スルー
				if (!package->treeTable[i].onRecord)continue;
				//イベント対象のツリー
				Tree* selectTree = NULL;
				//イベント対象のツリーを検索する
				for (int num = 0; num < treeManager->getTreeList().size(); num++)
				{
					Tree* tree = treeManager->getTreeList()[num];
					if (tree->getTreeData()->treeID == package->treeTable[i].id)
					{
						selectTree = tree;
						num = treeManager->getTreeList().size();//検索終了
					}
				}

				//イベントの対象ツリーが存在しない場合はスルー
				if (selectTree == NULL)continue;

				//イベント別にアクションする
				if(package->treeTable[i].greenState != selectTree->getTreeData()->greenState)
				{
					selectTree->transState();
				}
			}

		}

		bool transition = package->sceneReset;
		if (transition)
		{
			// シーン遷移
			changeScene(nextScene);
		}
	}

	//テストフィールドの更新
	testField->update();			//オブジェクト
	testFieldRenderer->update();	//レンダラー

	//スカイドームの更新
	sky->update();
	//海面の更新
	ocean->update();
	//ツリーマネージャーの更新
	treeManager->update(frameTime);

	//マーカーの更新
	markerRenderer->update(frameTime);

	//OFF
	if (input->wasKeyPressed('0'))
	{
		SerialCommunicationNS::send(SerialCommunicationNS::OFF);
	}
	//緑化状況10%
	if (input->wasKeyPressed('1') || treeManager->getGreeningRate() >= 0.1)
	{
		SerialCommunicationNS::send(SerialCommunicationNS::GREENING_10);
	}
	//緑化状況30%
	if (input->wasKeyPressed('2') || treeManager->getGreeningRate() >= 0.3)
	{
		SerialCommunicationNS::send(SerialCommunicationNS::GREENING_30);
	}
	//緑化状況50%
	if (input->wasKeyPressed('3') || treeManager->getGreeningRate() >= 0.5)
	{
		SerialCommunicationNS::send(SerialCommunicationNS::GREENING_50);
	}
	//CLEAR
	if (input->wasKeyPressed('4'))
	{
		SerialCommunicationNS::send(SerialCommunicationNS::CLEAR);
	}
	//FAILURE
	if (input->wasKeyPressed('5'))
	{
		SerialCommunicationNS::send(SerialCommunicationNS::FAILURE);
	}
	
	//カメラの更新
	{
		float rate = (sinf(sceneTimer) / 2.0f) + 0.5f;
		float fov = UtilityFunction::lerp((D3DX_PI / 180) * 70, (D3DX_PI / 180) * 90, 0.5);
		camera->setFieldOfView(fov);
		camera->rotation(D3DXVECTOR3(0, 1, 0), CAMERA_SPEED*frameTime);
		camera->update();
	}
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Display::render()
{
	//描画対象をウィンドウ全体に切替
	direct3D9->changeViewportFullWindow();

	//カメラの描画準備
	camera->renderReady();

	//3D描画
	render3D(camera);

	//エフェクシアーの描画
	effekseerNS::setCameraMatrix(
		0,
		camera->position,
		camera->gazePosition,
		camera->upVector);
	effekseerNS::render(0);

}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
void Display::render3D(Camera* currentCamera)
{
	//テストフィールドの描画
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);

	//ツリーの描画
	treeManager->render(currentCamera);

	//スカイドームの描画
	sky->render(currentCamera->view, currentCamera->projection, currentCamera->position);
	//海面の描画
	ocean->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	//マーカーの描画
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		markerRenderer->render(i, currentCamera);
	}
}

//===================================================================================================================================
//【2D[UI]描画】
//===================================================================================================================================
void Display::renderUI()
{
	
}

//===================================================================================================================================
//【衝突処理】
//===================================================================================================================================
void Display::collisions(void)
{
	// None
}

//===================================================================================================================================
//【AI処理】
//===================================================================================================================================
void Display::AI(void)
{
	// None
}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
#ifdef _DEBUG
void Display::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("syncTimer = %f", syncTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG
