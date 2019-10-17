//===================================================================================================================================
//【Title.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/30
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
	nextScene = SceneList::GAME;

	//シーンの更新
	SoundInterface::SwitchAudioBuffer(SceneList::TITLE);	

	//再生パラメータ
	memset(playParameters, 0, sizeof(playParameters));
	XAUDIO2_FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.1f, 1.5f };
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,TITLE_SE_LIST::TITLE_SE_01, false,NULL,true, filterParameters };
	playParameters[1] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,TITLE_SE_LIST::TITLE_SE_02, false,NULL,true, filterParameters };
	playParameters[2] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, TITLE_BGM_LIST::TITLE_BGM_01, true,1.0f,true, filterParameters };

	//再生
	SoundInterface::playSound(playParameters[0]);
	SoundInterface::playSound(playParameters[1]);
	SoundInterface::playSound(playParameters[2]);
}

//============================================================================================================================================
//【デストラクタ】
//============================================================================================================================================
Title::~Title(void)
{
	// サウンドの停止
	SoundInterface::stopSound(playParameters[0]);
	SoundInterface::stopSound(playParameters[1]);
	SoundInterface::stopSound(playParameters[2]);
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

	// Light
	light = new Light;
	light->initialize();

	// タイトルUIの初期化
	//uiTitle.initialize(_direct3D9->device, _textureLoader, selectStateMemory);

	//インスタンシングビルボードテスト
	plane.initialize(
		*shaderNS::reference(shaderNS::INSTANCE_BILLBOARD),
		*textureNS::reference(textureNS::LIGHT_001)
	);

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

	// タイトルUI
	//uiTitle.release();

}

//============================================================================================================================================
//【更新】
//============================================================================================================================================
void Title::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	plane.update(frameTime);

	// カメラ
	//camera[0].setUpVector(player[PLAYER_TYPE::PLAYER_1].getAxisY()->direction);
	//camera[0].update();

	//player[PLAYER_TYPE::PLAYER_1].animationPlayer.updateTitle();

	// タイトルUI
	//uiTitle.update(input, sound);

	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		updateInput();
		changeScene(nextScene);
	}

	// シーンエフェクトの更新
	//sceneEffect.update(_frameTime);

	//for (int i = 0; i < EFFECT_MAX; i++)
	//{
	//	// シーンエフェクト発生
	//	sceneEffect.generateSceneEffect(1, D3DXVECTOR3((float)(rand() % 100 - 50), (float)(rand() % 100 - 50), (float)(rand() % 100 - 50)));
	//}
	camera->update();
}

//============================================================================================================================================
//【入力の更新】
//============================================================================================================================================
void Title::updateInput(void)
{
	//switch (uiTitle.getSelectState())
	//{
	//case uiTitleNS::TITLE_MENU_TYPE::MENU_GAME_START:
	//	selectStateMemory = uiTitleNS::TITLE_MENU_TYPE::MENU_GAME_START;
	//	nextScene = (SceneList::SELECT);
	//	changeScene(nextScene);
	//	break;
	//case uiTitleNS::TITLE_MENU_TYPE::MENU_TUTORIAL:
	//	selectStateMemory = uiTitleNS::TITLE_MENU_TYPE::MENU_TUTORIAL;
	//	nextScene = (SceneList::TUTORIAL);
	//	changeScene(nextScene);
	//	break;
	//case uiTitleNS::TITLE_MENU_TYPE::MENU_OPERATION:
	//	selectStateMemory = uiTitleNS::TITLE_MENU_TYPE::MENU_OPERATION;
	//	nextScene = (SceneList::OPERATION);
	//	changeScene(nextScene);
	//	break;
	//case uiTitleNS::TITLE_MENU_TYPE::MENU_CREDIT:
	//	selectStateMemory = uiTitleNS::TITLE_MENU_TYPE::MENU_CREDIT;
	//	nextScene = (SceneList::CREDIT);
	//	changeScene(nextScene);
	//	break;
	//case uiTitleNS::TITLE_MENU_TYPE::MENU_GAME_EXIT:
	//	PostQuitMessage(NULL);
	//	break;
	//default:
	//	break;
	//}
}

//============================================================================================================================================
//【描画】
//============================================================================================================================================
void Title::render()
{
	// カメラ・ウィンドウ
	camera->renderReady();
	direct3D9->changeViewportFullWindow();

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
	// シーンエフェクトの描画
	//sceneEffect.render(_direct3D9->device, _currentCamera.view, _currentCamera.projection, _currentCamera.position);

	// プレーン( インスタンシング )
	plane.render(_currentCamera.view, _currentCamera.projection, _currentCamera.position);

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
	//uiTitle.render(_device);

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
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("node:%d", plane.getList().nodeNum);

}
#endif // _DEBUG