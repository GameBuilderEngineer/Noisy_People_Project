//===================================================================================================================================
//【Photograph.cpp】
// [作成者]HAL東京GP12A332 21 新里 将士
// [作成日]2019/12/26
// [更新日]2019/12/26
//===================================================================================================================================
#ifdef _DEBUG
//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Photograph.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace photographNS;


//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Photograph::Photograph()
{
	sceneName = "Scene -Photograph-";
	nextScene = SceneList::TITLE;

	//エネミーツール
	enemyTools = new ENEMY_TOOLS;

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Photograph::~Photograph()
{
	//エネミーツール
	SAFE_DELETE(enemyTools);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Photograph::initialize()
{
	//仮オブジェクト
	tmpObject = new TmpObject;

	//ターゲット
	target = new Object;
	target->initialize(&D3DXVECTOR3(0, 0, 0));

	//撮影カメラ
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera->setTarget(tmpObject->getPositionYeah());
	camera->setTargetX(&tmpObject->getAxisX()->direction);
	camera->setTargetY(&tmpObject->getAxisY()->direction);
	camera->setTargetZ(&tmpObject->getAxisZ()->direction);
	camera->setRelative(D3DXQUATERNION(0.0f, 60.0f, -15.0f, 0.0f));
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setGazeDistance(10.0f);
	camera->setRelativeGaze(D3DXVECTOR3(0, 0, 0));
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI) / 18 * 9);
	camera->setLimitRotationTop(0.1f);
	camera->setLimitRotationBottom(0.1f);

	//俯瞰カメラ
	topView = new Camera;
	topView->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	topView->setTarget(tmpObject->getPositionYeah());
	topView->setTargetX(&tmpObject->getAxisX()->direction);
	topView->setTargetY(&tmpObject->getAxisY()->direction);
	topView->setTargetZ(&tmpObject->getAxisZ()->direction);
	topView->setRelative(D3DXQUATERNION(1.0f, 100.0f, 0.0f, 0.0f));
	topView->setGazeDistance(100.0f);
	topView->setGaze(D3DXVECTOR3(0, 0, 0));
	topView->setRelativeGaze(D3DXVECTOR3(0, 0, 0));
	topView->setUpVector(D3DXVECTOR3(0, 1, 0));
	topView->setFieldOfView((D3DX_PI / 18) * 9);
	onTopView = false;

	//ライト
	light = new Light;
	light->initialize();

	//テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//プレイヤーの初期化
	tmpObject->initialize(inputNS::DINPUT_1P, 0);
	tmpObject->setCamera(camera);	//カメラポインタのセット

	tmpObject->configurationGravityWithRay(testField->getPosition(), testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());	//重力を設定
	tmpObjRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::WOLF));
	tmpObjRenderer->registerObject(tmpObject);

	//撮影カメラ確認用
	cameraObject = new Object;
	cameraObject->initialize(&tmpObject->position);
	cameraObjectRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::WOLF));
	cameraObjectRenderer->registerObject(cameraObject);
	
	cameraAxisZ = D3DXVECTOR3(0, 0, 0);
	fixedAxisZ = D3DXVECTOR3(0, 0, 0);
	cameraAxisX = D3DXVECTOR3(0, 0, 0);
	cameraAxisY = D3DXVECTOR3(0, 0, 0);
	fixedAxisX = D3DXVECTOR3(0, 0, 0);

	PointToolsListboxType = POINT_TYPE::POINT_NONE;

}


//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Photograph::uninitialize()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(topView);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(testField);
	SAFE_DELETE(tmpObjRenderer);
	SAFE_DELETE(light);
	SAFE_DELETE(target);
	SAFE_DELETE(cameraObject);
	SAFE_DELETE(cameraObjectRenderer);
}

//===================================================================================================================================
//【更新処理】
//===================================================================================================================================
void Photograph::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//処理落ち対策
	if (frameTime > 0.10)return;

	//テストフィールドの更新
	testField->update();
	testFieldRenderer->update();

	//ツールの更新
	enemyTools->update();

	D3DXVECTOR3 difference;
	float normal;
	if (bezier)
	{
		photoTimer -= frameTime;
		rate = photoTimer / photoTime;
		cameraObject->position = setBezier(startPoint, curvePoint, endPoint, rate);
		difference = endPoint - cameraObject->position;
		normal = D3DXVec3Length(&difference);
		if (normal <= 1.0f && normal >= -1.0f)
		{
			camera->position = startPoint;
			photoTimer = photoTime;
		}
	}

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

	//ターゲットオブジェクトの更新
	tmpObject->update(frameTime);
	LPD3DXMESH mesh = testFieldRenderer->getStaticMesh()->mesh;
	D3DXMATRIX matrix = testField->matrixWorld;
	
	if (input->wasKeyPressed('G'))
	{
		tmpObject->grounding(mesh, matrix);
	}
	tmpObjRenderer->update();
	//俯瞰カメラの更新
	topView->update();
	
	//ターゲットカメラの更新
	camera->update();

	//カメラの更新
	tmpObjRenderer->update();

	//カメラ確認用obj
	cameraObject->update();
	cameraObjectRenderer->update();

	//Enterまたは〇ボタンでリザルトへ
	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		//シーン遷移
		changeScene(nextScene);
	}

}

//===================================================================================================================================
//【描画処理】
//===================================================================================================================================
void Photograph::render()
{
	//カメラ・ウィンドウ
	direct3D9->changeViewportFullWindow();

	if (input->wasKeyPressed('Z'))
	{
		onTopView = !onTopView;
	}

	if (onTopView)
	{
		topView->renderReady();
		render3D(*topView);
		effekseerNS::setCameraMatrix(0, topView->position, topView->gazePosition, topView->upVector);
		effekseerNS::render(0);
	}
	else
	{
		camera->renderReady();
		render3D(*camera);
		effekseerNS::setCameraMatrix(0, camera->position, camera->gazePosition, camera->upVector);
		effekseerNS::render(0);

	}

	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();
}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
void Photograph::render3D(Camera currentCamera)
{
	//タイトルフィールド（テスト）
	testField->setAlpha(0.1f);
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	// プレイヤーの描画
	tmpObjRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	tmpObject->otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//撮影カメラ確認用オブジェクトの描画
	cameraObjectRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	//viewCamera->otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//ツールの描画
	enemyTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);

}


//===================================================================================================================================
//【UI/2D描画】
//===================================================================================================================================
void Photograph::renderUI()
{
	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


}

//===================================================================================================================================
//【衝突判定処理】
//===================================================================================================================================
void Photograph::collisions()
{

}

//===================================================================================================================================
//【AI処理】
//===================================================================================================================================
void Photograph::AI() 
{

}




//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
void Photograph::createGUI()
{
	float limitTop = 1000;
	float limitBottom = -1000;

	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat3("StartPoint ", startPoint, limitBottom, limitTop);
	ImGui::SliderFloat3("EndPoint", endPoint, limitBottom, limitTop);
	ImGui::SliderFloat3("CurvePoint", curvePoint, limitBottom, limitTop);
	testField->outputGUI();			//テストフィールド
	camera->outputGUI();			//カメラ
	tmpObject->outputGUI();			//プレイヤー
	cameraObject->outputGUI();		//カメラターゲット
	toolsGUI();
}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
void Photograph::toolsGUI()
{
	bool runFlag = false;
	bool deleteFlag = false;

	//開閉可能なフィールド
	if (ImGui::CollapsingHeader("Tool"))
	{
		if (ImGui::TreeNode("TreeNode A"))
		{
			if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)))
				ImGui::Text("PressKeyboard A");
			ImGui::TreePop();
		}

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("TreeNode B"))
		{
			ImGui::Text("TreeNode B is open from the Beginning");
			ImGui::TreePop();
		}

		const char* listboxPointType[] = { "NONE", "START", "END", "CURVE" };
		ImGui::ListBox("Point Type", &PointToolsListboxType, listboxPointType, POINT_TYPE::POINT_MAX);


		outputStartPointGUI(PointToolsListboxType, tmpObject->getPosition());
		outputEndPointGUI(PointToolsListboxType, tmpObject->getPosition());
		outputCurvePointGUI(PointToolsListboxType, tmpObject->getPosition());

		int backupMeshId = meshId;
		switch (PointToolsListboxType)
		{
		case POINT_TYPE::POINT_NONE:
			break;
		case POINT_TYPE::POINT_START:
			break;
		case POINT_TYPE::POINT_END:
			
			break;
		}
		
		ImGui::Checkbox("RUN", &runFlag);
		ImGui::Checkbox("Delete", &deleteFlag);

		if (runFlag)
		{
			bezier = true;
		}

	}
}

//===================================================================================================================================
//【ImGUIへの出力(StartPoint)】
//===================================================================================================================================
void Photograph::outputStartPointGUI(int GUIid,  D3DXVECTOR3* pos)
{
	if (GUIid == START_GUI_ID)
	{

		bool createFlag = false;
		bool deleteFlag = false;

		ImGui::Checkbox("Create Start_P", &createFlag);
		ImGui::Checkbox("Delete Start_P", &deleteFlag);

		if (createFlag)
		{
			startEffect = new photographNS::StartEffect(0, pos);
			effekseerNS::play(0, startEffect);
			startPoint = startEffect->position;
		}
		if (deleteFlag)
		{
			effekseerNS::stop(0, startEffect);
			startPoint = startEffect->position;
		}

	}
}
//===================================================================================================================================
//【ImGUIへの出力(EndPoint)】
//===================================================================================================================================
void Photograph::outputEndPointGUI(int GUIid, D3DXVECTOR3* pos)
{
	if (GUIid == END_GUI_ID)
	{

		bool createFlag = false;
		bool deleteFlag = false;

		ImGui::Checkbox("Create End_P", &createFlag);
		ImGui::Checkbox("Delete End_P", &deleteFlag);

		if (createFlag)
		{
			endEffect = new photographNS::EndEffect(0, pos);
			effekseerNS::play(0, endEffect);
			endPoint = endEffect->position;
		}
		if (deleteFlag)
		{
			effekseerNS::stop(0, endEffect);
			endPoint = endEffect->position;
		}
	}
}

//===================================================================================================================================
//【ImGUIへの出力(CurvePoint)】
//===================================================================================================================================
void Photograph::outputCurvePointGUI(int GUIid, D3DXVECTOR3* pos)
{
	if (GUIid == CURVE_GUI_ID)
	{
		bool createFlag = false;
		bool deleteFlag = false;

		ImGui::Checkbox("Create Curve_P", &createFlag);
		ImGui::Checkbox("Delete Curve_P", &deleteFlag);

		if (createFlag)
		{
			curveEffect = new photographNS::CurveEffect(0, pos);
			effekseerNS::play(0, curveEffect);
			curvePoint = curveEffect->position;
		}
		if (deleteFlag)
		{
			effekseerNS::stop(0, curveEffect);
			curvePoint = curveEffect->position;
		}
	}
}
void Photograph::addCurvePoint(D3DXVECTOR3* pos)
{

}


D3DXVECTOR3 Photograph::setBezier(D3DXVECTOR3 startpoint, D3DXVECTOR3 curvepoint, D3DXVECTOR3 endpoint, float rate)
{
	D3DXVECTOR3 P0_1;
	D3DXVECTOR3 P1_2;
	D3DXVECTOR3 result;

	D3DXVec3Lerp(&P0_1, &startpoint, &curvepoint, 1.0f - rate);
	D3DXVec3Lerp(&P1_2, &curvepoint, &endpoint, 1.0f - rate);
	D3DXVec3Lerp(&result, &P0_1, &P1_2, 1.0f - rate);

	return result;
}

#endif
