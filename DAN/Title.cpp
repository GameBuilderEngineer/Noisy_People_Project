//===================================================================================================================================
//【Title.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/10/17
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Title.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace titleNS;

//============================================================================================================================================
//【グローバル変数】
//============================================================================================================================================
//int selectStateMemory = uiTitleNS::TITLE_MENU_TYPE::MENU_GAME_START;	//	タイトルメニュー選択状態記憶

//============================================================================================================================================
//【コンストラクタ】
//============================================================================================================================================
Title::Title(void)
{
	// 今のシーン( タイトル )
	sceneName = ("Scene -Title-");
	nextScene = SceneList::TUTORIAL;

	//再生パラメータ
	PLAY_PARAMETERS playParameters = { 0 };//同時に再生したい数
	FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.1f, 1.5f };//フィルターの設定
	//再生する曲の指定サウンドID,ループ,スピードNULLでしない,基本false,基本NULL,フィルターを使うか使わないか
	playParameters= { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Title, true,1.0f,false,NULL,true, filterParameters };//BGMの設定

	//再生
	SoundInterface::BGM->playSound(&playParameters);

	//初期化
	tmpVolume = 1.0f;
}

//============================================================================================================================================
//【デストラクタ】
//============================================================================================================================================
Title::~Title(void)
{
	// サウンドの停止
	SoundInterface::BGM->uninitSoundStop();
}

//============================================================================================================================================
//【初期化】
//============================================================================================================================================
void Title::initialize()
{
	// サウンドの再生
	//sound->play(soundNS::TYPE::BGM_TITLE, soundNS::METHOD::LOOP);

	// Camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setRelative(D3DXQUATERNION(0.0f, 20.0f, -40.0f, 0.0f));
	camera->setRelativeGaze(D3DXVECTOR3(0, 0, 0));
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI) / 18 * 10);

	//エフェクシアーの設定
	effekseerNS::setProjectionMatrix(
		camera->fieldOfView,
		camera->windowWidth,
		camera->windowHeight,
		camera->nearZ,
		camera->farZ);

	//WaveBall
	waveBall = new WaveBall;

	// Light
	light = new Light;
	light->initialize();

	//タイトルUIの初期化
	titleUI.initialize();

	//エフェクト（インスタンシング）テスト
	testEffect = new TestEffect();
}

//============================================================================================================================================
//【終了処理】
//============================================================================================================================================
void Title::uninitialize(void)
{
	// ライト
	SAFE_DELETE(light);

	// カメラ
	SAFE_DELETE(camera);

	//エフェクト（インスタンシング）テスト
	SAFE_DELETE(testEffect);

	//WaveBall
	SAFE_DELETE(waveBall);

	// タイトルUI
	titleUI.uninitialize();

}

//============================================================================================================================================
//【更新】
//============================================================================================================================================
void Title::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//エフェクト（インスタンシング）テスト
	testEffect->update(frameTime);

	// カメラ
	//camera[0].setUpVector(player[PLAYER_TYPE::PLAYER_1].getAxisY()->direction);
	//camera[0].update();

	//player[PLAYER_TYPE::PLAYER_1].animationPlayer.updateTitle();

	//バーの移動
	//if(input->wasKeyPressed()

	//エフェクトの再生
	if (input->wasKeyPressed('1'))
	{
		effekseerNS::play(new effekseerNS::Instance);
	}
	//ミュート
	if (input->isKeyDown('M'))
	{
		SoundInterface::SE->setEndPointVoiceVolume(0.0f);
		SoundInterface::BGM->setEndPointVoiceVolume(0.0f);
		SoundInterface::S3D->setEndPointVoiceVolume(0.0f);
		waveBall->setVolume(0.0f);
		waveBall->setOnCol(false);
	}
	//ミュート解除
	if (input->isKeyDown('N'))
	{
		SoundInterface::SE->setEndPointVoiceVolume(1.0f);
		SoundInterface::BGM->setEndPointVoiceVolume(1.0f);
		SoundInterface::S3D->setEndPointVoiceVolume(1.0f);
		waveBall->setVolume(1.0f);
		waveBall->setOnCol(true);
	}
	//10%(test)
	if (input->isKeyDown('B'))
	{
		SoundInterface::SE->setEndPointVoiceVolume(0.1f);
		SoundInterface::BGM->setEndPointVoiceVolume(0.1f);
		SoundInterface::S3D->setEndPointVoiceVolume(0.1f);
		waveBall->setVolume(0.1f);
		waveBall->setOnCol(true);
	}

	// タイトルUI
	titleUI.update(input);

	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{

		updateInput();
		changeScene(nextScene);
	}

	//カメラ
	camera->update();
}

//============================================================================================================================================
//【入力の更新】
//============================================================================================================================================
void Title::updateInput(void)
{
	switch (titleUI.getSelectState())
	{
	case titleUiNS::TUTORIAL:
		selectStateMemory = titleUiNS::TUTORIAL;
		nextScene = (SceneList::TUTORIAL);
		changeScene(nextScene);
		break;
	case titleUiNS::GAME:
		selectStateMemory = titleUiNS::GAME;
		nextScene = (SceneList::GAME);
		changeScene(nextScene);
		break;
	case titleUiNS::CREDIT:
		selectStateMemory = titleUiNS::CREDIT;
		nextScene = (SceneList::CREDIT);
		changeScene(nextScene);
		break;
	case titleUiNS::EXIT:
		PostQuitMessage(NULL);
		break;
	
	default:
		break;
	}
}

//============================================================================================================================================
//【描画】
//============================================================================================================================================
void Title::render()
{
	// カメラ・ウィンドウ
	camera->renderReady();
	direct3D9->changeViewportFullWindow();

	//エフェクシアーテスト描画
	effekseerNS::setCameraMatrix(camera->position, camera->gazePosition, camera->upVector);
	effekseerNS::render();

	// 3D
	render3D(*camera);

	// 2D
	render2D();

}

//============================================================================================================================================
//【3D描画】
//============================================================================================================================================
void Title::render3D(Camera _currentCamera)
{
	//エフェクト（インスタンシング）テスト
	testEffect->render(_currentCamera.view, _currentCamera.projection, _currentCamera.position);

	// タイトルプレイヤー描画
	//player[0].toonRender
	//(
	//	_direct3D9->device,
	//	_currentCamera.view,
	//	_currentCamera.projection,
	//	_currentCamera.position,
	//	*shaderLoader->getEffect(shaderNS::TOON),
	//	*textureLoader->getTexture(textureLoaderNS::TOON_SHADE),
	//	*textureLoader->getTexture(textureLoaderNS::TOON_OUT_LINE));

}

//============================================================================================================================================
//【2D描画】
//============================================================================================================================================
void Title::render2D()
{
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// タイトルUI
	titleUI.render();

#if _DEBUG
	//WaveBall
	waveBall->draw();
#endif

	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================================================================================
//【衝突処理】
//============================================================================================================================================
void Title::collisions(void)
{
	// None
}

//============================================================================================================================================
//【AI処理】
//============================================================================================================================================
void Title::AI(void)
{
	// None
}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
#ifdef _DEBUG
void Title::createGUI()
{
	bool createScene = false;
	float backUpTmpVolume = tmpVolume;

	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("node:%d", testEffect->getList().nodeNum);
	ImGui::Checkbox("Create Scene", &createScene);
	ImGui::SliderFloat("volume control", &tmpVolume, 0.0f, 1.0f);

	if (backUpTmpVolume != tmpVolume)
	{
		backUpTmpVolume = tmpVolume;
		waveBall->setVolume(tmpVolume);
	}

	//ツール用シーン
	if (createScene)
	{
		selectStateMemory = titleUiNS::CREATE;
		nextScene = (SceneList::CREATE);
		changeScene(nextScene);
	}
}
#endif // _DEBUG