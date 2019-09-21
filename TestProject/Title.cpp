//===================================================================================================================================
//【Title.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
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
	nextScene = SceneList::SPLASH;
}

//============================================================================================================================================
//【デストラクタ】
//============================================================================================================================================
Title::~Title(void)
{
	// サウンドの停止
	//sound->stop(soundNS::TYPE::BGM_TITLE);
}

//============================================================================================================================================
//【初期化】
//============================================================================================================================================
void Title::initialize()
{
	// サウンドの再生
	//sound->play(soundNS::TYPE::BGM_TITLE, soundNS::METHOD::LOOP);

	//// Player
	//player[PLAYER_TYPE::PLAYER_1] = new Player;
	//player[PLAYER_TYPE::PLAYER_2] = new Player;

	// Camera
	//camera = new Camera[PLAYER_TYPE::PLAYER_TYPE_MAX];

	//for (int i = 0; i < PLAYER_TYPE::PLAYER_TYPE_MAX; i++)
	//{
	//	camera[i].initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//	//camera[i].setTarget(player[i].getPosition());
	//	//camera[i].setTargetX(&player[i].getAxisX()->direction);
	//	//camera[i].setTargetY(&player[i].getAxisY()->direction);
	//	//camera[i].setTargetZ(&player[i].getAxisZ()->direction);
	//	camera[i].setRelative(CAMERA_RELATIVE_QUATERNION[0]);
	//	camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
	//	camera[i].setRelativeGaze(D3DXVECTOR3(0, 0, 0));
	//	camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
	//	camera[i].setFieldOfView(D3DX_PI / 2.5);
	//}

	// Light
	//light = new Light;
	//light->initialize(_direct3D9);

	// プレイヤーの初期化
	//player[PLAYER_TYPE::PLAYER_1].initialize(playerNS::PLAYER_TYPE::PLAYER1, gameMaster->getPlayerInfomation()[PLAYER_TYPE::PLAYER_1].modelType, _direct3D9->device, staticMeshLoader, textureLoader, shaderLoader);
	//player[PLAYER_TYPE::PLAYER_1].setPosition(PLAYER_POSITION[playerNS::PLAYER_TYPE::PLAYER1]);
	//player[PLAYER_TYPE::PLAYER_1].animationPlayer.setAnimationConfiguration(animationPlayerNS::SCENE_TYPE::TITLE);

	// タイトルUIの初期化
	//uiTitle.initialize(_direct3D9->device, _textureLoader, selectStateMemory);

	// インスタンスプレーンの初期化
	//plane.createPositionSpherical(_direct3D9->device, 3000, 250.0f);
	//plane.initialize(_direct3D9->device, *shaderLoader->getEffect(shaderNS::INSTANCE_BILLBOARD), *textureLoader->getTexture(textureLoaderNS::BACKGROUND_DUST));

	// シーンエフェクト初期化
	//sceneEffect.initialize(_direct3D9->device, textureLoader, *shaderLoader->getEffect(shaderNS::INSTANCE_BILLBOARD));
}

//============================================================================================================================================
//【終了処理】
//============================================================================================================================================
void Title::uninitialize(void)
{
	// プレイヤー
	//player[PLAYER_TYPE::PLAYER_1].animationPlayer.release();

	// ライト
	//SAFE_DELETE(light);

	// カメラ
	//SAFE_DELETE_ARRAY(camera);

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
		// サウンドの再生
		//sound->play(soundNS::TYPE::SE_DECISION, soundNS::METHOD::PLAY);
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
	//device->SetTransform(D3DTS_VIEW, &camera[PLAYER_TYPE::PLAYER_1].view);
	//device->SetTransform(D3DTS_PROJECTION, &camera[PLAYER_TYPE::PLAYER_1].projection);
	direct3D9->changeViewportFullWindow();

	// 3D
	//render3D(camera[PLAYER_TYPE::PLAYER_1]);

	// αブレンドをつかう
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// αソースカラーの指定
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// αデスティネーションカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 2D
	render2D();

}

//============================================================================================================================================
//【3D描画】
//============================================================================================================================================
//void Title::render3D(Camera _currentCamera)
//{
	// シーンエフェクトの描画
	//sceneEffect.render(_direct3D9->device, _currentCamera.view, _currentCamera.projection, _currentCamera.position);

	// プレーン( インスタンシング )
	//plane.render(_direct3D9->device, _currentCamera.view, _currentCamera.projection, _currentCamera.position);

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
//}

//============================================================================================================================================
//【2D描画】
//============================================================================================================================================
void Title::render2D()
{
	// タイトルUI
	//uiTitle.render(_device);
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
}
#endif // _DEBUG