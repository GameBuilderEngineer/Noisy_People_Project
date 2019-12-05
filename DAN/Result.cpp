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
	nextScene = SceneList::TITLE;




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

	//テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
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

	//リザルトUIの初期化
	resultUI.initialize();

}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Result::uninitialize(void)
{
	SAFE_DELETE(camera);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(testField);

	resultUI.uninitialize();
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Result::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//UIの更新処理
	resultUI.update(frameTime);

	//テストフィールドの更新
	testField->update();			//オブジェクト
	testFieldRenderer->update();	//レンダラー


	//リザルトフェイズが5の時のみ Enterまたは〇ボタンでタイトルへ
	if (resultUI.resultPhase == resultUiNS::PHASE_05&&
		input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		// サウンドの再生
		//sound->play(soundNS::TYPE::SE_DECISION, soundNS::METHOD::PLAY);
		// シーン遷移
		changeScene(nextScene);
	}



	//カメラの更新
	{
		float rate = (sinf(sceneTimer*6)/2.0f) + 0.5f;
		float fov = UtilityFunction::lerp((D3DX_PI / 180) * 70, (D3DX_PI / 180) * 90, rate);
		camera->setFieldOfView(fov);
		camera->rotation(D3DXVECTOR3(0, 1, 0), CAMERA_SPEED*frameTime);
		camera->update();
	}
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

	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドをつかう
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	//UI
	renderUI();
}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
void Result::render3D(Camera currentCamera)
{
	//テストフィールドの描画
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

}

//===================================================================================================================================
//【2D[UI]描画】
//===================================================================================================================================
void Result::renderUI()
{
	// リザルトUI
	resultUI.render();
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
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG
