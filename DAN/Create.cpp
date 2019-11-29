//===================================================================================================================================
//【Create.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/20
// [更新日]2019/10/18
//===================================================================================================================================
#ifdef _DEBUG
//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Create.h"
#include "Game.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace gameNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Create::Create()
{
	sceneName = "Scene -Create-";

	nextScene = SceneList::RESULT;

	//エネミーツール
	enemyTools = new ENEMY_TOOLS;
	//アイテムツール
	itemTools = new ITEM_TOOLS;
	//ツリーツール
	treeTools = new TREE_TOOLS;

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Create::~Create()
{
	//エネミーツール
	SAFE_DELETE(enemyTools);
	//アイテムツール
	SAFE_DELETE(itemTools);
	//ツリーツール
	SAFE_DELETE(treeTools);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Create::initialize() {

	//tmpObj
	tmpObject = new TmpObject;

	//camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera->setTarget(tmpObject->getPositionYeah());
	camera->setTargetX(&tmpObject->getAxisX()->direction);
	camera->setTargetY(&tmpObject->getAxisY()->direction);
	camera->setTargetZ(&tmpObject->getAxisZ()->direction);
	camera->setRelative(CAMERA_RELATIVE_QUATERNION);
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setRelativeGaze(CAMERA_RELATIVE_GAZE);
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI / 18) * 9);
	camera->setLimitRotationTop(0.3f);
	camera->setLimitRotationBottom(0.7f);

	//light
	light = new Light;
	light->initialize();

	//テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//プレイヤーの初期化
	tmpObject->initialize(inputNS::DINPUT_1P, 0);
	tmpObject->setCamera(camera);	//カメラポインタのセット
	tmpObject->configurationGravityWithRay(testField->getPosition(), testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());	//重力を設定
	tmpObjRenderer = new StaticMeshRenderer *[tmpObjNS::TMPOBJ_LIST::TMPOBJ_MAX];
	for (int i = 0; i < tmpObjNS::TMPOBJ_LIST::TMPOBJ_MAX; i++)
	{
		int staticMeshNo = 0;
		switch (i)
		{
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_PLAYER:
			staticMeshNo = staticMeshNS::YAMADA_ROBOT2;
			break;
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_WOLF:
			staticMeshNo = staticMeshNS::SAMPLE_REDBULL;
			break;
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_TIGER:
			staticMeshNo = staticMeshNS::STAR_REGULAR_POLYHEDRON;
			break;
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_BEAR:
			staticMeshNo = staticMeshNS::DICE;
			break;
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_BATTERY:
			staticMeshNo = staticMeshNS::SAMPLE_SCISSORS;
			break;
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_EXAMPLE:
			staticMeshNo = staticMeshNS::YAMADA_ROBOT2;
			break;
		default:
			break;
		}
		tmpObjRenderer[i] = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNo));
		tmpObjRenderer[i]->registerObject(tmpObject);
	}

	//枯木の初期化
	deadTree = new DeadTree();
	//木Aの初期化
	treeA = new TreeTypeA();
	//木Bの初期化
	treeB = new TreeTypeB();
	//石の初期化
	stone = new Stone();

	//エフェクト（インスタンシング）テスト
	testEffect = new TestEffect();

	//ツール
	ToolsListboxType = TOOLS_TYPE::TOOLS_NONE;
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Create::uninitialize() {
	SAFE_DELETE(camera);
	SAFE_DELETE(light);
	SAFE_DELETE(testFieldRenderer);

	//たつき待ち
	for (int i = 0; i < tmpObjNS::TMPOBJ_LIST::TMPOBJ_MAX; i++)
	{
		SAFE_DELETE(tmpObjRenderer[i]);
	}
	ES_SAFE_DELETE_ARRAY(tmpObjRenderer);

	SAFE_DELETE(deadTree);
	SAFE_DELETE(treeA);
	SAFE_DELETE(treeB);
	SAFE_DELETE(stone);
	SAFE_DELETE(testEffect);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Create::update(float _frameTime) {

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//【処理落ち】
	//フレーム時間が約10FPS時の時の時間より長い場合は、処理落ち（更新しない）
	//※フレーム時間に準拠している処理が正常に機能しないため
	if (frameTime > 0.10)return;

	//エフェクト（インスタンシング）テスト
	testEffect->update(frameTime);

	//テストフィールドの更新
	testField->update();
	testFieldRenderer->update();

	//ツールの更新
	enemyTools->update();
	itemTools->update();
	treeTools->update();

	//プレイヤーの更新
	tmpObject->update(frameTime);

	// アイテムの更新
	if (input->wasKeyPressed('0'))
	{
		itemNS::ItemData unko = { 0, itemNS::BATTERY, *tmpObject->getPosition() };
	}
	if (input->wasKeyPressed('9'))
	{
	}

	////カメラの更新
	//camera->update();
	for (int i = 0; i < tmpObjNS::TMPOBJ_LIST::TMPOBJ_MAX; i++)
	{
		tmpObjRenderer[i]->update();
	}

	//枯木の更新
	deadTree->update();
	//木Aの更新
	treeA->update();
	//木Bの更新
	treeB->update();
	//石の更新
	stone->update();

	//カメラの更新
	camera->update();

	// Enterまたは〇ボタンでリザルトへ
	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		// シーン遷移
		changeScene(nextScene);
	}

}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Create::render() {

	//カメラ・ウィンドウ
	camera->renderReady();
	direct3D9->changeViewportFullWindow();
	render3D(*camera);
	effekseerNS::setCameraMatrix(camera->position, camera->gazePosition, camera->upVector);
	effekseerNS::render();

	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();

}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
void Create::render3D(Camera currentCamera) {

	//テストフィールドの描画
	testField->setAlpha(0.1f);
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	//エフェクト（インスタンシング）テスト
	testEffect->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// プレイヤーの描画
	tmpObjRenderer[getBufferID(meshId)]->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	// プレイヤーの他のオブジェクトの描画
	tmpObject->otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//枯木の描画
	deadTree->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//木Aの描画
	treeA->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//木Bの描画
	treeB->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//石の描画
	stone->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//ツールの描画
	enemyTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	itemTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	treeTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);
}

//===================================================================================================================================
//【UI/2D描画】
//===================================================================================================================================
void Create::renderUI() {

	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);						// αブレンドを行う
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);				// αソースカラーの指定
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);			// αデスティネーションカラーの指定

	//device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//===================================================================================================================================
//【衝突判定処理】
//===================================================================================================================================
void Create::collisions()
{
	
}

//===================================================================================================================================
//【AI処理】
//===================================================================================================================================
void Create::AI() {
}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
void Create::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("node:%d", testEffect->getList().nodeNum);
	testField->outputGUI();			//テストフィールド
	camera->outputGUI();			//カメラ
	tmpObject->outputGUI();			//プレイヤー
	toolsGUI();						//ツール
}

//===================================================================================================================================
//【ツールのGUI】
//===================================================================================================================================
void Create::toolsGUI()
{
	if (!ImGui::CollapsingHeader("Tools"))
	{
		collideGUI();	//当たり判定

		//ツールの種類
		const char* listboxEnemyType[] = { "NONE","ENEMY", "ITEM", "TREE" ,"MAP OBJECT" };
		ImGui::ListBox("Tools Type", &ToolsListboxType, listboxEnemyType, TOOLS_TYPE::TOOLS_MAX);

		enemyTools->outputEnemyToolsGUI(ToolsListboxType, *tmpObject->getPosition(), tmpObject->getAxisZ()->direction);
		itemTools->outputItemToolsGUI(ToolsListboxType, *tmpObject->getPosition(), tmpObject->getAxisZ()->direction);
		treeTools->outputTreeToolsGUI(ToolsListboxType, *tmpObject->getPosition(), tmpObject->getAxisZ()->direction);

		int backupMeshId = meshId;
		switch (ToolsListboxType)
		{
		case TOOLS_TYPE::TOOLS_NONE:
			break;
		case TOOLS_TYPE::TOOLS_ENEMY:
			meshId = enemyTools->GetStaticMeshID(enemyTools->EnemyListboxType);
			break;
		case TOOLS_TYPE::TOOLS_ITEM:
			meshId = itemTools->GetStaticMeshID(itemTools->ItemListboxType);
			break;
		case TOOLS_TYPE::TOOLS_TREE:
			break;
		case TOOLS_TYPE::TOOLS_MAP_OBJECT:
			break;
		default:
			break;
		}
		if (backupMeshId != meshId)
		{
			tmpObject->resetMesh(meshId);
		}
	}
}

//===================================================================================================================================
//【当たり判定のGUI】
//===================================================================================================================================
void Create::collideGUI()
{
	//当たり判定
	ImGui::Text("Enemy:");
	for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
	{
		if (enemyTools->bodyCollide[i].collide(tmpObject->getBodyCollide()->getCenter(),
			tmpObject->getRadius(), *enemyTools->object[i]->getMatrixWorld(), *tmpObject->getMatrixWorld()))
		{
			ImGui::Text("%d", i);
		}
	}
	ImGui::Text("Item:");
	for (int i = 0; i < itemTools->GetItemMax(); i++)
	{
		if (itemTools->bodyCollide[i].collide(tmpObject->getBodyCollide()->getCenter(),
			tmpObject->getRadius(), *itemTools->object[i]->getMatrixWorld(), *tmpObject->getMatrixWorld()))
		{
			ImGui::Text("%d", i);
		}
	}
}

//===================================================================================================================================
//【バッファIDを取得 meshID->bufferID】
//===================================================================================================================================
int Create::getBufferID(int meshID)
{
	switch (meshID)
	{
	case staticMeshNS::YAMADA_ROBOT2:
		return  tmpObjNS::TMPOBJ_LIST::TMPOBJ_EXAMPLE;
		break;
	case staticMeshNS::SAMPLE_REDBULL:
		return tmpObjNS::TMPOBJ_LIST::TMPOBJ_WOLF;
		break;
	case staticMeshNS::STAR_REGULAR_POLYHEDRON:
		return tmpObjNS::TMPOBJ_LIST::TMPOBJ_TIGER;
		break;
	case staticMeshNS::DICE:
		return tmpObjNS::TMPOBJ_LIST::TMPOBJ_BEAR;
		break;
	case staticMeshNS::SAMPLE_SCISSORS:
		return tmpObjNS::TMPOBJ_LIST::TMPOBJ_BATTERY;
		break;
	//case staticMeshNS::YAMADA_ROBOT2:
	//	return tmpObjNS::TMPOBJ_LIST::TMPOBJ_EXAMPLE;
	//	break;
	default:
		return tmpObjNS::TMPOBJ_LIST::TMPOBJ_PLAYER;
		break;
	}

	return tmpObjNS::TMPOBJ_LIST::TMPOBJ_PLAYER;
}
#endif
