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
	nextScene = SceneList::SPLASH;
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

	networkServer = new NETWORK_INTERFACE;
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Display::uninitialize(void)
{
	SAFE_DELETE(treeManager);
	SAFE_DELETE(camera);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(testField);
	SAFE_DELETE(networkServer);
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
		syncTimer = package->timer;
	}

	if (package->num != 0)
	{
		//パッケージ内のイベントを呼び出す
		for (int i = 0; i < package->num; i++)
		{
			//イベント対象のツリー
			Tree* selectTree = NULL;
			//イベント対象のツリーを検索する
			for (int num = 0; num < treeManager->getTreeList().size(); num++)
			{
				Tree* tree = treeManager->getTreeList()[i];
				if (tree->getTreeData()->treeID == package->treeTable[i].id)
				{
					selectTree = tree;
					num = treeManager->getTreeList().size();//検索終了
				}
			}
		
			//イベントの対象ツリーが存在しない場合はスルー
			if (selectTree == NULL)continue;

			//イベント別にアクションする
			switch (package->treeTable[i].eventType)
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
		//パッケージ内のイベントを解放する
		free(package->treeTable);
	}

	//テストフィールドの更新
	testField->update();			//オブジェクト
	testFieldRenderer->update();	//レンダラー


	//ツリーマネージャーの更新
	treeManager->update(frameTime);



	//if (input->wasKeyPressed(VK_RETURN) ||
	//	input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
	//	input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	
	//蔡へ
	//通信プログラムでtarnsitionをtrueにしてシーン遷移を行う。
	bool transition = false;
	
	if(transition)
	{
		// シーン遷移
		changeScene(nextScene);
	}

	//カメラの更新
	{
		float rate = (sinf(sceneTimer * 6) / 2.0f) + 0.5f;
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
	render3D(*camera);

	//エフェクシアーの描画
	effekseerNS::setCameraMatrix(
		0,
		camera->position,
		camera->gazePosition,
		camera->upVector);
	effekseerNS::render(0);

	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドをつかう
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	//UI
	//renderUI();
}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
void Display::render3D(Camera currentCamera)
{
	//テストフィールドの描画
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	//ツリーの描画
	treeManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

}

//===================================================================================================================================
//【2D[UI]描画】
//===================================================================================================================================
//void Display::renderUI()
//{
//	// リザルトUI
//	resultUI.render();
//}

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
