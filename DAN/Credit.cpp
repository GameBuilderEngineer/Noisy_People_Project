//===================================================================================================================================
//【Credit.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Credit.h"
#include "Sound.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace creditNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Credit::Credit()
{
	// 現在のシーン(クレジット)
	sceneName = "Scene -Credit-";
	// 次のシーン(タイトル)
	nextScene = SceneList::TITLE;

	//シーンの更新
	SoundInterface::SwitchAudioBuffer(SceneList::CREDIT);

	//再生パラメータ
	PLAY_PARAMETERS playParameters;
	FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, CREDIT_BGM_LIST::BGM_Credit, true,1.0f,false,NULL,true, filterParameters };

	//再生
	SoundInterface::BGM->playSound(&playParameters);
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Credit::~Credit()
{
	SoundInterface::BGM->uninitSoundStop();
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Credit::initialize()
{
	// サウンドの再生
	//sound->play(soundNS::TYPE::BGM_CREDIT, soundNS::METHOD::LOOP);

	//camera
	//camera = new Camera;
	//camera->initialize(WINDOW_WIDTH / 2, WINDOW_HEIGHT);
	//camera->setGaze(D3DXVECTOR3(0, 0, 0));
	//camera->setPosition(D3DXVECTOR3(0, 0, -1));
	//camera->setUpVector(D3DXVECTOR3(0, 1, 0));

	// クレジット2D初期化
	//credit2D.initialize(direct3D9->device, 0, _textureLoader);

	creditTex.initialize();
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Credit::uninitialize()
{
	//credit2D.uninitialize();
	//SAFE_DELETE(camera);
	creditTex.uninitialize();
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Credit::update(float frameTime)
{
	// カメラ更新
	//camera->update();

	// クレジット2D更新
	//credit2D.update();

	//Enter,BackSpaceまたは〇ボタン,×ボタンでタイトルへ
	// 前の遷移へ戻る
	if (input->wasKeyPressed(VK_BACK) ||
		input->getController()[PLAYER1]->wasButton(virtualControllerNS::B) ||
		input->getController()[PLAYER2]->wasButton(virtualControllerNS::B))
	{
		// サウンドの再生
		//sound->play(soundNS::TYPE::SE_CANCEL, soundNS::METHOD::PLAY);
		nextScene = SceneList::TITLE;
		changeScene(nextScene);
		return;
	}

	creditTex.update();

}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Credit::render()
{
	//1Pカメラ・ウィンドウ
	//device->SetTransform(D3DTS_VIEW, &camera->view);
	//device->SetTransform(D3DTS_PROJECTION, &camera->projection);
	direct3D9->changeViewportFullWindow();
	//render3D(direct3D9);
	//UI
	renderUI(direct3D9->device);
	
}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
//void Credit::render3D()
//{
//
//}

//===================================================================================================================================
//【2D/UI描画】
//===================================================================================================================================
void Credit::renderUI(LPDIRECT3DDEVICE9 device)
{
	// クレジット2D描画
	//credit2D.render(device);
	creditTex.render();
}

//===================================================================================================================================
//【衝突判定処理】
//===================================================================================================================================
void Credit::collisions()
{

}

//===================================================================================================================================
//【AI処理】
//===================================================================================================================================
void Credit::AI()
{

}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
#ifdef _DEBUG
void Credit::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG
