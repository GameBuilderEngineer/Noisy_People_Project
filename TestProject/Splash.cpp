//===================================================================================================================================
//【Splash.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#include "Splash.h"
//#include "Sound.h"

//===================================================================================================================================
// 【using宣言】
//===================================================================================================================================
using namespace splashNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Splash::Splash()
{
	// 現在のシーン(スプラッシュ)
	sceneName = "Scene -Splash-";
	// 次のシーン(タイトル)
	nextScene = SceneList::TITLE;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Splash::~Splash()
{

}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Splash::initialize()
{
	// サウンドの再生
	//sound->play(soundNS::TYPE::BGM_SPLASH, soundNS::METHOD::PLAY);

	// Camera
	//camera = new Camera[PLAYER_TYPE::PLAYER_TYPE_MAX];

	//for (int i = 0; i < PLAYER_TYPE::PLAYER_TYPE_MAX; i++)
	//{
		//camera[i].initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
		//camera[i].setTarget(player[i].getPosition());
		//camera[i].setTargetX(&player[i].getAxisX()->direction);
		//camera[i].setTargetY(&player[i].getAxisY()->direction);
		//camera[i].setTargetZ(&player[i].getAxisZ()->direction);
		//camera[i].setRelative(CAMERA_RELATIVE_QUATERNION[0]);
		//camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
		//camera[i].setRelativeGaze(D3DXVECTOR3(0, 0, 0));
		//camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
		//camera[i].setFieldOfView(D3DX_PI / 2.5);
	//}

	// Light
	//light = new Light;
	//light->initialize(direct3D9);

	// スプラッシュ2D初期化
	//splash2D.initialize(direct3D9->device,0, _textureLoader);
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Splash::uninitialize()
{
	// サウンドの停止
	//sound->stop(soundNS::TYPE::BGM_SPLASH);

	// カメラ
	//SAFE_DELETE_ARRAY(camera);

	// ライト
	//SAFE_DELETE(light);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Splash::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	// カメラ更新
	//camera->update();

	// スプラッシュ2D更新
	//splash2D.update();

	//Enter,Spaceまたは〇ボタン,Optionsでタイトルへ
	if (input->wasKeyPressed(VK_RETURN) ||
		input->wasKeyPressed(VK_SPACE) ||
		input->getController()[PLAYER_TYPE::PLAYER_1]->wasButton(virtualControllerNS::A) ||
		input->getController()[PLAYER_TYPE::PLAYER_2]->wasButton(virtualControllerNS::A) ||
		input->getController()[PLAYER_TYPE::PLAYER_1]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
		input->getController()[PLAYER_TYPE::PLAYER_2]->wasButton(virtualControllerNS::SPECIAL_MAIN)
		)changeScene(nextScene);


	// フェードが終わったらタイトルへ
	//if (splash2D.gotitle)changeScene(nextScene);
}

//===================================================================================================================================
//【描画処理】
//===================================================================================================================================
void Splash::render()
{
	//1Pカメラ・ウィンドウ
	//device->SetTransform(D3DTS_VIEW, &camera[PLAYER_TYPE::PLAYER_1].view);
	//device->SetTransform(D3DTS_PROJECTION, &camera[PLAYER_TYPE::PLAYER_1].projection);
	direct3D9->changeViewportFullWindow();

	//render3D(camera[PLAYER_TYPE::PLAYER_1]);

	// αブレンドをつかう
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// αソースカラーの指定
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// αデスティネーションカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//UI
	renderUI();

}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
//void Splash::render3D(Camera _currentCamera)
//{
//
//}

//===================================================================================================================================
//【UI描画】
//===================================================================================================================================
void Splash::renderUI()
{
	// αテストを有効に
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// スプラッシュ2D描画
	//splash2D.render(device);

	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//===================================================================================================================================
//【衝突処理】
//===================================================================================================================================
void Splash::collisions()
{
}

//===================================================================================================================================
//【AI処理】
//===================================================================================================================================
void Splash::AI()
{
}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
#ifdef _DEBUG
void Splash::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG
