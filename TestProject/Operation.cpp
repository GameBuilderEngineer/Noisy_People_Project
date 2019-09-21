//===================================================================================================================================
//【Splash.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Operation.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace operationNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Operation::Operation()
{
	// 現在のシーン(オペレーション)
	sceneName = "Scene -Operation-";
	// 次のシーン(タイトル)
	nextScene = SceneList::TITLE;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Operation::~Operation()
{
	// サウンドの停止
	//sound->stop(soundNS::TYPE::BGM_OPERATION);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Operation::initialize()
{
	// サウンドの再生
	//sound->play(soundNS::TYPE::BGM_OPERATION, soundNS::METHOD::PLAY);

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
	//light->initialize(direct3D9);

	// キーボード説明初期化
	//keyOpe.initialize(direct3D9->device, 0, _textureLoader);

	// パッド説明初期化
	//padOpe.initialize(direct3D9->device, 0, _textureLoader);

	opeTransition = NULL; // オペレーション画像入れ替え

	// シーンエフェクト初期化
	//sceneEffect.initialize(direct3D9->device, textureLoader, *shaderLoader->getEffect(shaderNS::INSTANCE_BILLBOARD));

}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Operation::uninitialize()
{
	// カメラ
	//SAFE_DELETE_ARRAY(camera);

	// ライト
	//SAFE_DELETE(light);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Operation::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	// カメラ更新
	//camera->update();

	// 前の遷移へ戻る
	if (input->wasKeyPressed(VK_BACK) ||
		input->getController()[PLAYER_TYPE::PLAYER_1]->wasButton(virtualControllerNS::B) ||
		input->getController()[PLAYER_TYPE::PLAYER_2]->wasButton(virtualControllerNS::B))
	{
		// サウンドの再生
		//sound->play(soundNS::TYPE::SE_CANCEL, soundNS::METHOD::PLAY);
		nextScene = SceneList::TITLE;
		changeScene(nextScene);
		return;
	}

	// キーを押したら選択UI移動
	if (input->wasKeyPressed(VK_RIGHT) ||
		input->getController()[PLAYER_TYPE::PLAYER_1]->wasButton(virtualControllerNS::RIGHT) ||
		input->getController()[PLAYER_TYPE::PLAYER_2]->wasButton(virtualControllerNS::RIGHT))
	{
		if (opeTransition < 1)
		{
			// サウンドの再生
			//sound->play(soundNS::TYPE::SE_SELECT, soundNS::METHOD::PLAY);
			opeTransition++;
		}
	}
	else if (input->wasKeyPressed(VK_LEFT) ||
		input->getController()[PLAYER_TYPE::PLAYER_1]->wasButton(virtualControllerNS::LEFT) ||
		input->getController()[PLAYER_TYPE::PLAYER_2]->wasButton(virtualControllerNS::LEFT))
	{
		if (opeTransition > 0)
		{
			// サウンドの再生
			//sound->play(soundNS::TYPE::SE_SELECT, soundNS::METHOD::PLAY);
			opeTransition--;
		}
	}

	// 選択UI下限
	if (opeTransition > OPE_MAX)
	{
		opeTransition = 0;
	}
	// 選択UI上限
	else if (opeTransition < 0)
	{
		opeTransition = OPE_MAX;
	}

	// シーンエフェクトの更新
	//sceneEffect.update(frameTime);

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		// シーンエフェクト発生
		//sceneEffect.generateSceneEffect(1, D3DXVECTOR3((float)(rand() % 100 - 50), (float)(rand() % 100 - 50), (float)(rand() % 100 - 50)));
	}

}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Operation::render()
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
	renderUI(direct3D9->device);
}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
//void Operation::render3D(Direct3D9* direct3D9, Camera _currentCamera)
//{
//	// シーンエフェクトの描画
//	sceneEffect.render(direct3D9->device, _currentCamera.view, _currentCamera.projection, _currentCamera.position);
//}

//===================================================================================================================================
//【2D[UI]描画】
//===================================================================================================================================
void Operation::renderUI(LPDIRECT3DDEVICE9 device)
{
	switch (opeTransition)
	{
	case 0:	// パッド説明
		//padOpe.render(device);
		//keyOpe.render(device);
		break;
	case 1:// キーボード説明
		//keyOpe.render(device);
		//padOpe.render(device);
		break;
	}
}

//===================================================================================================================================
//【衝突処理】
//===================================================================================================================================
void Operation::collisions()
{
	//none
}

//===================================================================================================================================
//【AI処理】
//===================================================================================================================================
void Operation::AI()
{
	//none
}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
#ifdef _DEBUG
void Operation::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG
