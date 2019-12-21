//===================================================================================================================================
//【Finale.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/12/12
// [更新日]2019/12/12
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Finale.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace FinaleNS;

//============================================================================================================================================
//【グローバル変数】
//============================================================================================================================================

//============================================================================================================================================
//【コンストラクタ】
//============================================================================================================================================
Finale::Finale()
{
	//今のシーン(フィナーレ)
	sceneName = ("Scene -Finale-");
	nextScene = SceneList::TITLE;
}

//============================================================================================================================================
//【デストラクタ】
//============================================================================================================================================
Finale::~Finale()
{
}

//============================================================================================================================================
//【初期化】
//============================================================================================================================================
void Finale::initialize()
{
	//ターゲットオブジェクト
	target = new Object;
	//target->initialize(&D3DXVECTOR3(0.0f, 100.0f, -150));		//ターゲットの初期位置設定
	target->initialize(&D3DXVECTOR3(0.0f, 20.0f, -300));		//ターゲットの初期位置設定
	
	//初期フォトグラフ
	stateCamera = CAMERA0;
	// Camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//camera->setGaze(D3DXVECTOR3(0, 100, 0));
	//camera->setRelative(D3DXQUATERNION(0.0f, 20.0f, -40.0f, 0.0f));	//※元の値※
	camera->setRelative(D3DXQUATERNION(0.0f, 0.0f, -1.0f, 0.0f));   //※ターゲットの初期位置に足される形になっている
	camera->setTarget(&target->position);
	camera->setGazeDistance(500.0f);
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI) / 18 * 10);
	camera->setViewProjection();


	//エフェクシアーの設定
	effekseerNS::setProjectionMatrix(0,
		camera->fieldOfView,
		camera->windowWidth,
		camera->windowHeight,
		camera->nearZ,
		camera->farZ);

	// Light
	light = new Light;
	light->initialize();

	//フィナーレフィールド（テスト）
	finaleField = new Object();
	finaleFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
	finaleFieldRenderer->registerObject(finaleField);
	finaleField->initialize(&D3DXVECTOR3(0, 0, 0));

	//スカイドーム
	sky = new Sky();

	cameraAxisZ = D3DXVECTOR3(0, 0, 0);
	fixedAxisZ = D3DXVECTOR3(0, 0, 0);
	cameraAxisX = D3DXVECTOR3(0, 0, 0);
	cameraAxisY = D3DXVECTOR3(0, 0, 0);
	fixedAxisX = D3DXVECTOR3(0, 0, 0);
	cameraRelativeQuaternion = CAMERA_RELATIVE_QUATERNION;

	// ツリーの初期化
	treeManager = new TreeManager();
	treeManager->initialize(finaleFieldRenderer->getStaticMesh()->mesh, finaleField->getMatrixWorld());
	// ツリーをツール情報を元に設置する
	treeManager->createUsingTool();
	// 非ヴィジョンの描画
	treeManager->switchingNormalView(0);
	for (int i = 0; i < treeManager->getTreeList().size(); i++)
	{
		treeManager->getTreeList()[i]->transState();
	}
	int unko = treeManager->getTreeNum();

}

//============================================================================================================================================
//【終了処理】
//============================================================================================================================================
void Finale::uninitialize(void)
{
	// ライト
	SAFE_DELETE(light);

	// カメラ
	SAFE_DELETE(camera);

	//タイトルフィールド
	SAFE_DELETE(finaleField);
	SAFE_DELETE(finaleFieldRenderer);

	//スカイドーム
	SAFE_DELETE(sky);

	//ターゲットオブジェクト
	SAFE_DELETE(target);

	// ツリーマネージャー
	SAFE_DELETE(treeManager);
}

//============================================================================================================================================
//【更新】
//============================================================================================================================================
void Finale::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//タイトルフィールド（テスト）
	finaleField->update();	//オブジェクト
	finaleFieldRenderer->update();

	//スカイフィールドの更新
	sky->update();

	target->update();

	// カメラ
	//camera[0].setUpVector(player[PLAYER_TYPE::PLAYER_1].getAxisY()->direction);
	//camera[0].update();

	//player[PLAYER_TYPE::PLAYER_1].animationPlayer.updateFinale();

	//バーの移動
	//if(input->wasKeyPressed()


	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		changeScene(nextScene);
	}

	

	//Base::anyAxisRotationSlerp(&cameraQ,D3DXVECTOR3(13.2f, 6.0f, -13.0f),);

	//注視オブジェクトとカメラの二点間ベクトル（カメラZ軸ベクトル）
	cameraAxisZ = camera->getAxisZ();
	fixedAxisZ = Base::slip(cameraAxisZ, camera->upVector);	//カメラの傾きに対応
	//カメラX軸ベクトル
	D3DXVec3Cross(&cameraAxisX, &camera->upVector, &cameraAxisZ);
	//カメラY軸ベクトル（上方向の更新）
	D3DXVECTOR3 Y = D3DXVECTOR3(0, 0, 0);
	D3DXVec3Cross(&Y, &fixedAxisZ, &cameraAxisX);
	D3DXVec3Cross(&cameraAxisY, &cameraAxisZ, &cameraAxisX);
	D3DXVec3Cross(&fixedAxisX, &cameraAxisY, &cameraAxisZ);

	// ツリーの更新
	treeManager->update(frameTime);

	switch (stateCamera)
	{
	case CAMERA0:
		if (sceneTimer > 0.0f)
		{
			startPos = target->position;	//ラープ始点
			moveTime = 3.0f;				//終点までの時間
			moveTimer = moveTime;			//移動タイマー
			stateCamera++;
			//カメラの相対位置を一時保存
			tmpCameraQ = camera->relativeQuaternion;
		}
		break;
	case CAMERA1:

		if (moveTimer > 0.0f)
		{
			moveTimer -= frameTime;
			rate = moveTimer / moveTime;

			distance = UtilityFunction::lerp(500, 20, 1.0f - rate);
			camera->setGazeDistance(distance);
			//D3DXVec3Lerp(&target->position, &D3DXVECTOR3(0.0f, 100.0f, -150), &D3DXVECTOR3(0.0f, 20.0f, -300), 1.0f - rate);
			if (moveTimer <= 0)
			{
				target->position = D3DXVECTOR3(0.0f, 20.0f, -300);
				startPos = target->position;	//ラープ始点
				moveTime = 10.0f;				//終点までの時間
				moveTimer = moveTime;			//移動タイマー
				degreeTime = 10.0;				//回転時間（Y軸）
				degreeTimer = degreeTime;		//回転タイマー（Y軸）
				degreeTimeX = 2.0;				//回転時間（X軸）
				degreeTimerX = degreeTimeX;		//回転タイマー（X軸）
				stateCamera++;
				//カメラの相対位置を一時保存
				tmpCameraQ = camera->relativeQuaternion;
			}

		}
		break;
	case CAMERA2:
		if (moveTimer > 0)
		{
			moveTimer -= frameTime;
			degreeTimer -= frameTime;
			rate = moveTimer / moveTime;

			rateY = degreeTimer / degreeTime;
			degreeY = UtilityFunction::lerp(0, 380, 1.0 - rateY);
			
			//D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(-34.0f, 180.0f, -135.0f), 1.0f - rate);
			BezierPoint1 = Title::BezierCurve(startPos, D3DXVECTOR3(-1500, 800, -150), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);//丘上
			BezierPoint2 = Title::BezierCurve(startPos, D3DXVECTOR3(-4000, -2700, 0), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);//橋下
			D3DXVec3Lerp(&Curve1, &BezierPoint1, &BezierPoint2, 1.0f - rate);
			
			BezierPoint3 = Title::BezierCurve(startPos, D3DXVECTOR3(80, 300, 2000), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);//狭道
			D3DXVec3Lerp(&Curve2, &Curve1, &BezierPoint3, 1.0f - rate);
			
			BezierPoint4 = Title::BezierCurve(startPos, D3DXVECTOR3(200, 400, 2000), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);//砂浜南
			D3DXVec3Lerp(&Curve3, &Curve2, &BezierPoint4, 1.0f - rate);

			BezierPoint5 = Title::BezierCurve(startPos, D3DXVECTOR3(0, -300, 2300), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);
			D3DXVec3Lerp(&Curve4, &Curve3, &BezierPoint5, 1.0 - rate);

			BezierPoint6 = Title::BezierCurve(startPos, D3DXVECTOR3(1300, 200, -300), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);
			D3DXVec3Lerp(&target->position, &Curve4, &BezierPoint6, 1.0 - rate);

			if (degreeTimer > 0)
			{
				//前のカメラの相対位置に補正する
				camera->relativeQuaternion = tmpCameraQ;
				camera->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
				if (moveTimer < 1.0f)
				{
					degreeTimerX -= frameTime;
					rateX = degreeTimerX / degreeTimeX;
					degreeX = UtilityFunction::lerp(0, 20, 1.0 - rateX);

					camera->rotation(fixedAxisX, degreeX);
				}
			}
			if (moveTimer <= 0)
			{
				target->position = D3DXVECTOR3(-70.0f, 150.0f, -370);
				startPos = target->position;
				moveTime = 5.0f;//待機
				moveTimer = moveTime;
				//degreeTimer = 6.0f;
				//degreeTime = degreeTimer;
				////カメラの相対位置を一時保存
				//camera->rotation(D3DXVECTOR3(0, 1, 0), 55.0f);
				//camera->rotation(fixedAxisX, 45.0f);
				//tmpCameraQ = camera->relativeQuaternion;
				stateCamera++;


			}
		}
		break;
	case CAMERA3:
		if (moveTimer > 0)
		{
			moveTimer -= frameTime;
			if (moveTimer <= 0)
			{
				changeScene(nextScene);
			}


		}


		break;
	default:
		break;
	}



	//カメラ移動
	if (input->isKeyDown('W'))
	{
		fixedAxisZ *= 1.0f;
		target->position += fixedAxisZ;
	}
	if (input->isKeyDown('S'))
	{
		fixedAxisZ *= -1.0f;
		target->position += fixedAxisZ;
	}
	if (input->isKeyDown('A'))
	{
		cameraAxisX *= -1.0f;
		target->position += cameraAxisX;
	}
	if (input->isKeyDown('D'))
	{
		cameraAxisX *= 1.0f;
		target->position += cameraAxisX;
	}
	if (input->isKeyDown('Q'))
	{
		Y *= 1.0f;
		target->position += camera->upVector;
	}
	if (input->isKeyDown('E'))
	{
		Y *= 1.0f;
		target->position -= camera->upVector;

	}

	//カメラ回転
	//camera->rotation(D3DXVECTOR3(0, -1, 0), degree);
	//Y軸
	if (input->isKeyDown(VK_RIGHT))
	{
		
		camera->rotation(camera->upVector, inputDegree);
		//target->quaternion.y += 5.0f;
	}
	if (input->isKeyDown(VK_LEFT))
	{
		camera->rotation(-camera->upVector, inputDegree);
		//target->quaternion.y -= 5.0f;
	}
	//X軸
	if (input->isKeyDown(VK_UP))
	{
		camera->rotation(-fixedAxisX, inputDegree);
	}
	if (input->isKeyDown(VK_DOWN))
	{
		camera->rotation(fixedAxisX, inputDegree);
	}
	//ズーム
	if (input->isKeyDown('Z'))
	{
		camera->relativeQuaternion -= camera->relativeQuaternion * 0.05f;
	}
	if (input->isKeyDown('X'))
	{
		camera->relativeQuaternion += camera->relativeQuaternion * 0.05f;
	}



	//カメラ
	camera->update();

}

//============================================================================================================================================
//【描画】
//============================================================================================================================================
void Finale::render()
{
	// カメラ・ウィンドウ
	camera->renderReady();
	direct3D9->changeViewportFullWindow();

	//エフェクシアーテスト描画
	effekseerNS::setCameraMatrix(0, camera->position, camera->gazePosition, camera->upVector);
	effekseerNS::render(0);

	// 3D
	render3D(*camera);

	// 2D
	render2D();

}

//============================================================================================================================================
//【3D描画】
//============================================================================================================================================
void Finale::render3D(Camera _currentCamera)
{

	//フィナーレフィールド（テスト）
	finaleFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), _currentCamera.view, _currentCamera.projection, _currentCamera.position);

	//スカイフィールドの描画
	sky->render(_currentCamera.view, _currentCamera.projection, _currentCamera.position);

	//ツリーの描画
	treeManager->render(_currentCamera.view, _currentCamera.projection, _currentCamera.position);
}

//============================================================================================================================================
//【2D描画】
//============================================================================================================================================
void Finale::render2D()
{
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);


	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================================================================================
//【衝突処理】
//============================================================================================================================================
void Finale::collisions(void)
{
	// None
}

//============================================================================================================================================
//【AI処理】
//============================================================================================================================================
void Finale::AI(void)
{
	// None
}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
#ifdef _DEBUG
void Finale::createGUI()
{
	bool createScene = false;
	float limitTop = 1000;
	float limitBottom = -1000;
	camera->outputGUI();

	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Create Scene", &createScene);

	ImGui::Text("controller1 LStick(%.02f,%.02f)",
		input->getController()[inputNS::DINPUT_1P]->getLeftStick().x,
		input->getController()[inputNS::DINPUT_1P]->getLeftStick().y);
	ImGui::Text("controller1 LStickTrigger(%.02f,%.02f)",
		input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().x,
		input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().y);

}
#endif // _DEBUG