//===================================================================================================================================
//【Window.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/17
// [更新日]2019/09/19
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Window.h"

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
Window* window = NULL; //ウィンドウクラスのポインタ---ウィンドウプロシージャのためスタティック領域へ保存

//===================================================================================================================================
//【ImGuiコールバック関数】
//===================================================================================================================================
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//===================================================================================================================================
//【ウィンドウプロシージャ：コールバック関数】
//===================================================================================================================================
LRESULT CALLBACK wndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK wndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return window->msgProc(wnd, msg, wparam, lparam);
}

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Window::Window()
{
	initialized = false;
	windowActivate = true;
	fullScreen = false;
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
HRESULT Window::initialize(HINSTANCE instance, INT x, INT y, INT width, INT height, LPSTR windowName) {
	window = this;
	//ウィンドウの定義
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndProc;
	wndClass.hInstance = instance;
	wndClass.hIcon = LoadIcon(instance, (LPCTSTR)IDI_ICON0);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszClassName = windowName;
	wndClass.hIconSm = LoadIcon(instance, (LPCTSTR)IDI_ICON0);
	RegisterClassEx(&wndClass);
	//ウィンドウの作成
	wnd = CreateWindow(windowName, windowName, WS_OVERLAPPEDWINDOW, 0, 0, width, height, 0, 0, instance, 0);
	if (!wnd)
		return E_FAIL;

	// ウィンドウを中央に設置
	setWindowCenter(wnd);

	//ウィンドウの表示
	ShowWindow(wnd, SW_SHOW);
	UpdateWindow(wnd);
	initialized = true;
	return S_OK;
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Window::update()
{
	ShowWindow(wnd, SW_SHOW);
	UpdateWindow(wnd);
}

//===================================================================================================================================
//【メッセージプロシージャ】
//===================================================================================================================================
LRESULT Window::msgProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(wnd, msg, wParam, lParam))
		return true;

	if (!initialized)	return DefWindowProc(wnd, msg, wParam, lParam);
	if (input == NULL)	return DefWindowProc(wnd, msg, wParam, lParam);
	switch (msg)
	{
	case WM_ACTIVATE:
		windowActivate = wParam != WA_INACTIVE;
		input->clearWheelFraction();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:case WM_SYSKEYDOWN: // キーが押された
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		input->keyDown(wParam);
		return 0;
	case WM_KEYUP:case WM_SYSKEYUP: // キーが離された
		input->keyUp(wParam);
		return 0;
	case WM_CHAR:					// 文字が入力された
		input->keyIn(wParam);
		return 0;
	case WM_MOUSEMOVE:				// マウスが移動された
		input->mouseIn(lParam);
		return 0;
	case WM_INPUT:					// マウスからのローデータ入力
		input->mouseRawIn(lParam);
		return 0;
	case WM_LBUTTONDOWN:			// 左マウスボタンが押された
		input->setMouseLButton(true);
		SetCapture(wnd);
		input->mouseIn(lParam);
		return 0;
	case WM_LBUTTONUP:				// 左マウスボタンが離された
		input->setMouseLButton(false);
		ReleaseCapture();
		input->mouseIn(lParam);
		return 0;
	case WM_MBUTTONDOWN:			// 中央マウスボタンが押された
		input->setMouseMButton(true);
		SetCapture(wnd);
		input->mouseIn(lParam);
		return 0;
	case WM_MBUTTONUP:				// 中央マウスボタンが離された
		input->setMouseMButton(false);
		ReleaseCapture();
		input->mouseIn(lParam);
		return 0;
	case WM_RBUTTONDOWN:			// 右マウスボタンが押された
		input->setMouseRButton(true);
		SetCapture(wnd);
		input->mouseIn(lParam);
		return 0;
	case WM_RBUTTONUP:				// 右マウスボタンが離された
		input->setMouseRButton(false);
		ReleaseCapture();
		input->mouseIn(lParam);
		return 0;
	case WM_XBUTTONDOWN:case WM_XBUTTONUP:// マウスのXボタンが押された/離された
		input->setMouseXButton(wParam);
		input->mouseIn(lParam);
		return 0;
	case WM_MOUSEWHEEL://マウスのホイールが回転した
		input->mouseWheelIn(wParam);
		return 0;
	case WM_DEVICECHANGE:			// コントローラをチェック
		input->resetController();
		return 0;
	}
	return DefWindowProc(wnd, msg, wParam, lParam);
}


//===================================================================================================================================
//【ウィンドウの中央位置への補正処理】
//===================================================================================================================================
BOOL Window::setWindowCenter(HWND _windowHandle)
{
	// 変数宣言
	RECT windowRegion;			//	ウィンドウ領域
	RECT desktopRegion;			//	デスクトップ領域
	int windowPositionY = NULL;	//	ウィンドウ位置 Y
	int windowPositionX = NULL;	//	ウィンドウ位置 X
	int windowSizeX = NULL;		//	ウィンドウサイズ X
	int windowSizeY = NULL;		//	ウィンドウサイズ Y

	// 各サイズの取得
	GetMonitorRect(&desktopRegion);					//	デスクトップのサイズを取得
	GetWindowRect(_windowHandle, &windowRegion);	//	ウィンドウのサイズを取得

	// クライアント領域がWINDOW_WIDTH x WINDOW_HEIGHTとなるようにウィンドウサイズを調整
	RECT clientRect;
	// ウィンドウのクライアント領域のサイズを取得
	GetClientRect(_windowHandle, &clientRect);

	// 各座標の割り出し
	windowSizeX = (windowRegion.right - windowRegion.left);													//	ウインドウの横幅の割り出し
	windowSizeY = (windowRegion.bottom - windowRegion.top);													//	ウインドウの縦幅の割り出し
	windowPositionX = (((desktopRegion.right - desktopRegion.left) - windowSizeX) / 2 + desktopRegion.left);//	横方向の中央座標軸の割り出し
	windowPositionY = (((desktopRegion.bottom - desktopRegion.top) - windowSizeY) / 2 + desktopRegion.top);	//	縦方向の中央座標軸の割り出し

	// ウィンドウを画面中央に移動
	return SetWindowPos	//	SetWindowPos関数：子ウィンドウ、ポップアップウィンドウ、またはトップレベルウィンドウのサイズ、位置、および Z オーダーを変更する( これらのウィンドウは、その画面上での表示に従って順序が決められる、最前面にあるウィンドウは最も高いランクを与えられ、Zオーダーの先頭に置かれる )
	(
		_windowHandle,									//	ウィンドウハンドル
		NULL,											//	配置順序のハンドル：先行するウィンドウのハンドルを指定
		windowPositionX,								//	ウィンドウ左上隅の"X"座標を指定：横方向の位置 X
		windowPositionY,								//	ウィンドウ左上隅の"Y"座標を指定：縦方向の位置 Y
		windowSizeX + (windowSizeX - clientRect.right),	//	ウィンドウの横幅を指定 X
		windowSizeY + (windowSizeY - clientRect.bottom),//	ウィンドウの縦幅を指定 Y
		(SWP_NOZORDER | SWP_NOOWNERZORDER)				//	ウィンドウ位置のオプション：ウィンドウのサイズや、位置の変更に関するフラグを指定
	);


}

//===================================================================================================================================
//【setter】
//===================================================================================================================================
void Window::changeDisplayMode(bool fullScreen)
{
	if (this->fullScreen == fullScreen)	return;

	this->fullScreen = fullScreen;

	if (this->fullScreen)
	{

	}
	else {
		setWindowCenter(wnd);
	}

}

//===================================================================================================================================
//【setter】
//===================================================================================================================================
void wndresizebyclient(HWND hWnd, int x, int y) {
	RECT size;
	RECT wndsize;
	GetClientRect(hWnd, &size);
	GetWindowRect(hWnd, &wndsize);
	wndsize.right = wndsize.right - wndsize.left;
	wndsize.bottom = wndsize.bottom - wndsize.top;
	SetWindowPos(hWnd, NULL, 0, 0, x + wndsize.right - size.right, y + wndsize.bottom - size.bottom, SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOZORDER);
}

//===================================================================================================================================
//【setter】
//===================================================================================================================================
void Window::setInput(){input = getInput();}	//Inputクラスのセット

//===================================================================================================================================
//【getter】
//===================================================================================================================================
//ウィンドウの中央位置取得
D3DXVECTOR2 Window::getCenter()
{
	GetWindowRect(wnd, &rect);//現在のウィンドウの矩形情報を取得
	D3DXVECTOR2 center((float)(rect.right - rect.left) / 2, (float)(rect.bottom - rect.top) / 2);//ウィンドウ左上からの中央位置への差を計算
	center += D3DXVECTOR2((float)rect.left, (float)rect.top);//0,0位置からウィンドウ左上位置を加算	
	return center;
}
//ウィンドウの矩形情報の取得
RECT Window::getRect()
{
	GetWindowRect(wnd, &rect);//現在のウィンドウの矩形情報を取得
	return rect;
}