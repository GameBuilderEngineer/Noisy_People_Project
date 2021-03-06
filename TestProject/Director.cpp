//===================================================================================================================================
//【Director.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/17
// [更新日]2019/10/06
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Director.h"
#include "Splash.h"
#include "Title.h"
#include "Tutorial.h"
#include "Credit.h"
#include "Game.h"
#include "Result.h"
#include "SE.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Director::Director() {
	ZeroMemory(this, sizeof(Director));
	//hiddenCursor = true;
	//lockCursor = true;
	fpsMode = FIXED_FPS;
	fixedFps = FRAME_RATE;

#ifdef _DEBUG
	onGUI = true;
	memory = new MemoryViewer;
#endif // _DEBUG
	//ShowCursor(FALSE);
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Director::~Director() {
	SAFE_DELETE(window);
	SAFE_DELETE(d3d);
#ifdef _DEBUG
	SAFE_DELETE(imgui);
	SAFE_DELETE(memory);
#endif // _DEBUG
	SAFE_DELETE(input);
	SAFE_DELETE(scene);
	SAFE_DELETE(textureLoader);
	SAFE_DELETE(staticMeshLoader);
	SAFE_DELETE(shaderLoader);
	//SAFE_DELETE(soundInterface);
	//SAFE_DELETE(textManager);
	//SAFE_DELETE(gameMaster);
	//SAFE_DELETE(animationLoader);
	//thread_a->join();
	//SAFE_DELETE(thread_a);
	//ShowCursor(TRUE);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
HRESULT Director::initialize() {
	//window
	window = new Window;
	if (!window)return E_FAIL;
	MFAIL(window->initialize(instance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME), "ウィンドウ作成失敗");
	wnd = window->wnd;

	//direct3D9
	d3d = new Direct3D9;
	if (d3d == NULL)
	{
		MSG("Direct3Dの初期化失敗");
		return E_FAIL;
	}
	MFAIL(d3d->initialize(wnd), "Direct3D初期化失敗");

#ifdef _DEBUG
	//thread_a = new std::thread(threadA);
	SetForegroundWindow(wnd);
	imgui = new ImguiManager(wnd);
#endif // _DEBUG

	//input
	input = new Input();
	input->initialize(instance, window->wnd, true);
	window->setInput();

	//textureLoader
	textureLoader = new TextureLoader;
	textureLoader->load(getDevice());

	//メッシュ読み込み
	// Xファイルからメッシュをロードする	
	//StaticMesh
	staticMeshLoader = new StaticMeshLoader;
	staticMeshLoader->load(getDevice());

	//HierarchyMesh
	//setVisualDirectory();
	//SKINMESH
	//setVisualDirectory();

	//シェーダー読込
	//Shader
	shaderLoader = new ShaderLoader;
	shaderLoader->load(d3d->device);

	//テキストデータ読込
	//textManager = new TextManager();
	//textManager->initialize(d3d->device);

	//ゲーム管理クラス
	//gameMaster = new GameMaster();

	//アニメーション読込クラス
	//animationLoader = new AnimationLoader();
	//animationLoader->initialize(d3d->device);

	//sound
	//soundInterface = new SoundInterface();

	//scene
	scene = new Splash();
	scene->initialize();

	//メモリ解放テスト
	//scene->uninitialize();
	//SAFE_DELETE(scene);
	//int i = 1;
	//while (i >= 0)
	//{
	//	scene = new Title();
	//	scene->initialize(d3d,input,sound,textureLoader,staticMeshLoader,shaderLoader,textManager);
	//	scene->uninitialize();
	//	SAFE_DELETE(scene);
	//	i--;
	//}
	//
	//scene = new Splash();
	//scene->initialize(d3d,input,sound,textureLoader,staticMeshLoader,shaderLoader,textManager);

	// 高分解能タイマーの準備を試みる
	if (QueryPerformanceFrequency(&timerFreq) == false)
	{
		MSG("高分解能タイマーの取得失敗しました。");
	}
	// 開始時間を取得
	QueryPerformanceCounter(&timeStart);	

	return S_OK;
}


//===================================================================================================================================
//【基本処理】
// [用途]処理の順序の定義
//===================================================================================================================================
void Director::run(HINSTANCE _instance) {
	char directory[512];
	GetCurrentDirectory(sizeof(directory), directory);
	initializeDirectory(directory);

	instance = _instance;
	if (FAILED(initialize()))
	{
		return;
	}
	ShowWindow(wnd, SW_SHOW);
	UpdateWindow(wnd);

	// メッセージループ
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			mainLoop();
		}
	}
}

//===================================================================================================================================
//【メインループ関数】
// [用途]基本的にループする内容
//===================================================================================================================================
void Director::mainLoop() {
	//リセット
	if (input->wasKeyPressed(VK_F5))
	{
		scene->changeScene(SceneList::SPLASH);
		changeNextScene();
	}
	if (scene->checkChangeOrder())//シーン切替フラグの確認
		changeNextScene();

	setFrameTime();		//フレーム時間の初期化処理
	update();			//メイン更新処理
	render();			//メイン描画処理
	fixFPS();			//固定FPS処理
	displayFPS();		//windowネームへFPS表示

	//入力をクリア:すべてのキーチェックが行われた後これを呼び出す
	input->clear(inputNS::MOUSE | inputNS::KEYS_PRESSED);
}

//===================================================================================================================================
//【更新処理】
// [用途]アプリ全体の更新処理
//===================================================================================================================================
void Director::update() {
#ifdef _DEBUG
	memory->update();
	imgui->beginFrame();
	imgui->update();//デフォルトウィンドウ
	imgui->beginImGui("DirectorGUI");
	createGUI();
	imgui->endImGui();
#endif // _DEBUG
	input->update(window->windowActivate);
	//if (input->wasKeyPressed(VK_F1))
	//{
	//	hiddenCursor = !hiddenCursor;
	//	if (hiddenCursor) {
	//		ShowCursor(FALSE);
	//	}
	//	else {
	//		ShowCursor(TRUE);
	//	}
	//}
	//if (input->wasKeyPressed(VK_F2))
	//	lockCursor = !lockCursor;
	scene->update(frameTime);
	scene->collisions();
	scene->AI();
	//sound->updateSound
#ifdef _DEBUG
	if (*scene->getShowGUI())
	{
		imgui->beginImGui(scene->getSceneName()->c_str());
		scene->createGUI();
		imgui->endImGui();
	}
#endif // _DEBUG
	//if (lockCursor)
	//{
	//	if (input->getMouseRawX() != 0 || input->getMouseRawY() != 0)
	//	{
	//		SetCursorPos((int)window->getCenter().x, (int)window->getCenter().y);
	//	}
	//}
}

//===================================================================================================================================
//【GUIウィンドウの作成】
// [用途]GUIウィンドウの作成処理
//===================================================================================================================================
void Director::createGUI()
{
#ifdef _DEBUG
	if (!onGUI)return;
	ImGui::SliderInt("fpsMode", &fpsMode, VARIABLE_FPS, FIXED_FPS);
	ImGui::SliderInt("fixedFPS", &fixedFps, MIN_FRAME_RATE, MAX_FRAME_RATE);
	ImGui::Checkbox("SceneGUI", scene->getShowGUI());	
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("CPU %.2f ％", memory->getCpuUsege());
	ImGui::Text("MEMORY %d kb", memory->getMemoryUsege());
	ImGui::Text("PHYS_MEMORY %d kb", memory->getPhysMemorys());
	
#endif // _DEBUG
}

//===================================================================================================================================
//【描画処理】
// [用途]アプリ全体の描画処理
//===================================================================================================================================
void Director::render() {
#ifdef _DEBUG
	//Debug
	d3d->clear(imgui->getClearColor());
	if (SUCCEEDED(d3d->beginScene()))
	{
		scene->render();
		imgui->render();
		d3d->endScene();
	}
	d3d->present(); 
#else
	//Release
	d3d->clear();
	if (SUCCEEDED(d3d->beginScene()))
	{
		scene->render();
		d3d->endScene();
	}
	d3d->present(); 
#endif // _DEBUG
}

//===================================================================================================================================
//【フレーム時間のセット】
// [用途]フレーム時間の初期設定処理
//===================================================================================================================================
void Director::setFrameTime()
{
	// 最後のフレームからの経過時間を計算、frameTimeに保存
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;
	timeStart = timeEnd;
}

//===================================================================================================================================
//【フレームレート固定処理】
// [用途]FPSを定数により指定した周期に固定する処理
// fpsModeに応じて固定フレームレートか制限なし可変フレームレートに切り替わる
//===================================================================================================================================
void Director::fixFPS() {
	if (fpsMode != FIXED_FPS)return;
	static INT Frames = 0, FPS = 0;
	static LARGE_INTEGER Frq = { 0 }, PreviousTime = { 0 }, CurrentTime = { 0 };
	DOUBLE Time = 0;
	char sz[11] = { 0 };

	while (Time <= (double)1000/(double)fixedFps)//1000ms / 60frame=16.6666 
	{
		QueryPerformanceFrequency(&Frq);

		QueryPerformanceCounter(&CurrentTime);
		Time = (DOUBLE)(CurrentTime.QuadPart - PreviousTime.QuadPart);

		Time *= (DOUBLE)1000.0 / (DOUBLE)Frq.QuadPart;

		//ここに次フレームまでの待機中にさせたい処理を記述
		//if (lockCursor)
		//{
		//	if (input->getMouseRawX() != 0 || input->getMouseRawY() != 0)
		//		SetCursorPos((int)window->getCenter().x, (int)window->getCenter().y);
		//}
	}
	PreviousTime = CurrentTime;
}

//===================================================================================================================================
//【FPS表示処理】
// [用途]FPSをウィンドウネームに表示する処理
//===================================================================================================================================
void Director::displayFPS() {
	//FPS計算表示
	static LARGE_INTEGER time;
	LARGE_INTEGER time2, frq;
	double elaptime = 0;
	static int frame = 0;
	QueryPerformanceFrequency(&frq);
	double microsec = 1000000 / (double)frq.QuadPart;

	frame++;

	QueryPerformanceCounter(&time2);
	elaptime = (time2.QuadPart - time.QuadPart)*microsec;

	if (elaptime > 1000000)
	{
		QueryPerformanceCounter(&time);

		char str[50];
		char name[200] = { 0 };
		GetClassNameA(wnd, name, sizeof(name));
		sprintf(str, " fps=%d", frame);
		strcat(name, str);
		SetWindowTextA(wnd, name);
		frame = 0;
	}
}

//===================================================================================================================================
//【シーン遷移処理】
// [用途]シーンクラスが遷移状態になった場合にシーンを遷移させる処理
//===================================================================================================================================
void Director::changeNextScene() {
	int nextScene = scene->checkNextScene();		//次のシーンIDを取得
	//scene->copyGameMaster(gameMaster);				//ゲーム管理情報をDirectorへ保存
	scene->uninitialize();
	SAFE_DELETE(scene);								// シーンの削除
	switch (nextScene)								// 指定されたシーンへ遷移
	{
	case SceneList::SPLASH:					scene = new Splash(); break;
	case SceneList::TITLE:					scene = new Title(); break;
	case SceneList::TUTORIAL:				scene = new Tutorial(); break;
	case SceneList::CREDIT:					scene = new Credit(); break;
	case SceneList::GAME:					scene = new Game(); break;
	case SceneList::RESULT:					scene = new Result(); break;
	case SceneList::NONE_SCENE:				break;
	}
	//scene->setGameMaster(gameMaster);//ゲーム管理情報をシーンへセット
	//scene->setAnimationLoader(animationLoader);
	scene->initialize();
	currentSceneName = scene->getSceneName();
	
	//サウンド
	//SEManager::SwitchAudioBuffer(nextScene);	//シーンの更新
}

//void threadA()
//{
//	while (roop)
//	{
//	}
//}