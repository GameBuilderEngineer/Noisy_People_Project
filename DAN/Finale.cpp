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

	// Camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//camera->setGaze(D3DXVECTOR3(0, 100, 0));
	//camera->setRelative(D3DXQUATERNION(0.0f, 20.0f, -40.0f, 0.0f));	//※元の値※
	camera->setRelative(D3DXQUATERNION(0.0f, 0.0f, 20.0f, 0.0f));   //※ターゲットの初期位置に足される形になっている
	//camera->setTarget(&target->position);

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

	//タイトルフィールド（テスト）
	//titleField = new Object();
	//titleFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
	//titleFieldRenderer->registerObject(titleField);
	//titleField->initialize(&D3DXVECTOR3(0, 0, 0));

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
	//SAFE_DELETE(titleField);
	//SAFE_DELETE(titleFieldRenderer);

}

//============================================================================================================================================
//【更新】
//============================================================================================================================================
void Finale::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//タイトルフィールド（テスト）
	//titleField->update();	//オブジェクト
	//titleFieldRenderer->update();

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




	////カメラ移動
	//if (input->isKeyDown('W'))
	//{
	//	fixedAxisZ *= 1.0f;
	//	target->position += fixedAxisZ;
	//}
	//if (input->isKeyDown('S'))
	//{
	//	fixedAxisZ *= -1.0f;
	//	target->position += fixedAxisZ;
	//}
	//if (input->isKeyDown('A'))
	//{
	//	cameraAxisX *= -1.0f;
	//	target->position += cameraAxisX;
	//}
	//if (input->isKeyDown('D'))
	//{
	//	cameraAxisX *= 1.0f;
	//	target->position += cameraAxisX;
	//}
	//if (input->isKeyDown('Q'))
	//{
	//	Y *= 1.0f;
	//	target->position += camera->upVector;
	//}
	//if (input->isKeyDown('E'))
	//{
	//	Y *= 1.0f;
	//	target->position -= camera->upVector;

	//}

	////カメラ回転
	////camera->rotation(D3DXVECTOR3(0, -1, 0), degree);
	////Y軸
	//if (input->isKeyDown(VK_RIGHT))
	//{
	//	
	//	camera->rotation(camera->upVector, inputDegree);
	//	//target->quaternion.y += 5.0f;
	//}
	//if (input->isKeyDown(VK_LEFT))
	//{
	//	camera->rotation(-camera->upVector, inputDegree);
	//	//target->quaternion.y -= 5.0f;
	//}
	////X軸
	//if (input->isKeyDown(VK_UP))
	//{
	//	camera->rotation(-fixedAxisX, inputDegree);
	//}
	//if (input->isKeyDown(VK_DOWN))
	//{
	//	camera->rotation(fixedAxisX, inputDegree);
	//}
	////ズーム
	//if (input->isKeyDown('Z'))
	//{
	//	camera->relativeQuaternion -= camera->relativeQuaternion * 0.05f;
	//}
	//if (input->isKeyDown('X'))
	//{
	//	camera->relativeQuaternion += camera->relativeQuaternion * 0.05f;
	//}



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

	//タイトルフィールド（テスト）
	//titleFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), _currentCamera.view, _currentCamera.projection, _currentCamera.position);



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