//===================================================================================================================================
//【Direct3D9.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/17
// [更新日]2019/11/09
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Direct3D9.h"

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
static LPDIRECT3DDEVICE9* pointerDevice = NULL;
static Direct3D9* pointerDirect3D9 = NULL;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Direct3D9::Direct3D9()
{
	ZeroMemory(this, sizeof(Direct3D9));
	pointerDevice = &this->device;
	pointerDirect3D9 = this;
	fullScreen = false;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Direct3D9::~Direct3D9()
{
	pointerDevice = NULL;
	pointerDirect3D9 = NULL;
	SAFE_RELEASE(device);
	SAFE_RELEASE(d3d);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
HRESULT Direct3D9::initialize(HWND targetWnd)
{
	//Direct3Dオブジェクトの作成
	if (NULL == (d3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MSG("Direct3Dの作成に失敗しました");
		return E_FAIL;
	}
	//ディスプレイモードの取得
	D3DDISPLAYMODE dMode;
	d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dMode);

	//--------------------------------------
	//「DIRECT3Dデバイス」オブジェクトの作成
	//--------------------------------------

	//Windowモードのパラメータの作成
	ZeroMemory(&d3dppWindow, sizeof(d3dppWindow));
	d3dppWindow.BackBufferFormat				= dMode.Format;
	d3dppWindow.BackBufferCount					= 1;
	d3dppWindow.SwapEffect						= D3DSWAPEFFECT_DISCARD;
	//==============================================================================
	d3dppWindow.Windowed						= true;
	d3dppWindow.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;
	//d3dppWindow.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;
	//==============================================================================
	d3dppWindow.BackBufferWidth					= WINDOW_WIDTH;
	d3dppWindow.BackBufferHeight				= WINDOW_HEIGHT;
	d3dppWindow.EnableAutoDepthStencil			= true;
	d3dppWindow.AutoDepthStencilFormat			= D3DFMT_D24S8;
	d3dppWindow.MultiSampleType					= D3DMULTISAMPLE_NONE;

	//FullScreenモードのパラメータの作成
	ZeroMemory(&d3dppFullScreen, sizeof(d3dppFullScreen));
	d3dppFullScreen.BackBufferFormat			= dMode.Format;
	d3dppFullScreen.BackBufferCount				= 1;
	d3dppFullScreen.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	//==============================================================================
	d3dppFullScreen.FullScreen_RefreshRateInHz	= dMode.RefreshRate;
	d3dppFullScreen.Windowed					= false;
	d3dppFullScreen.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;
	//==============================================================================
	d3dppFullScreen.BackBufferWidth				= WINDOW_WIDTH;
	d3dppFullScreen.BackBufferHeight			= WINDOW_HEIGHT;
	d3dppFullScreen.EnableAutoDepthStencil		= true;
	d3dppFullScreen.AutoDepthStencilFormat		= D3DFMT_D24S8;
	d3dppFullScreen.MultiSampleType				= D3DMULTISAMPLE_NONE;


//	D3DPRESENT_PARAMETERS d3dpp;
//	ZeroMemory(&d3dpp, sizeof(d3dpp));
//	d3dpp.BackBufferFormat			= dMode.Format;
//	d3dpp.BackBufferCount			= 1;
//	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
//#ifdef _DEBUG
//#if 1
//	d3dpp.Windowed					= true;
//	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
//#else
//	d3dpp.FullScreen_RefreshRateInHz = dMode.RefreshRate;
//	d3dpp.Windowed = false;
//	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
//#endif
//#else
//	d3dpp.FullScreen_RefreshRateInHz = dMode.RefreshRate;
//	d3dpp.Windowed					= false;
//	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;
//#endif // _DEBUG
//	d3dpp.BackBufferWidth			= WINDOW_WIDTH;
//	d3dpp.BackBufferHeight			= WINDOW_HEIGHT;
//	d3dpp.EnableAutoDepthStencil	= true;
//	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
//	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;

#ifdef _DEBUG
#if 1
	fullScreen = false;
#else
	fullScreen = true;
#endif
#else
	fullScreen = true;
#endif // _DEBUG

	if (fullScreen)
	{
		d3dpp = d3dppFullScreen;
	}
	else {
		d3dpp = d3dppWindow;
	}

	if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, targetWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &device)))
	{
		MessageBox(0, "HARDWAREモードでDIRECT3Dデバイスを作成できません\nSOFTWAREモードで再試行します", NULL, MB_OK);
		if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, targetWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &device)))
		{
			MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
			if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, targetWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &device)))
			{
				if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, targetWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &device)))
				{
					MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	device->SetRenderState(D3DRS_ZENABLE, true);					//Zバッファー処理を有効にする
	device->SetRenderState(D3DRS_LIGHTING, true);					//ライトを有効にする
	device->SetRenderState(D3DRS_AMBIENT, 0x22111111);				//アンビエントライト（環境光）を設定する
	device->SetRenderState(D3DRS_SPECULARENABLE, true);				//スペキュラ（光沢反射）を有効にする
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			//カリングの無効化
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			//反時計回りカリング有効化
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);			//時計回りカリング有効化
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);		//アンチエイリアシングをかける
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			//αブレンドを行う
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//αソースカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//αデスティネーションカラーの指定
	device->GetViewport(&viewPort);									//ビューポートを取得
	device->GetRenderTarget(0,&backBuffer);							//バックバッファを取得
	device->GetDepthStencilSurface(&zBuffer);						//Zバッファを取得
	return S_OK;
}

//===================================================================================================================================
//【FPS表示処理】
//===================================================================================================================================
void Direct3D9::showFPS()
{
	static INT frames = 0, FPS = 0;
	static LARGE_INTEGER frq = { 0 }, previous = { 0 }, current = { 0 };
	DOUBLE time = 0;
	//char sz[11] = { 0 };

	QueryPerformanceFrequency(&frq);

	QueryPerformanceCounter(&current);
	time = (DOUBLE)(current.QuadPart - previous.QuadPart);
	time *= (DOUBLE)1100.0 / (DOUBLE)frq.QuadPart;
	if (time >= 1100.000)
	{
		previous = current;
		FPS = frames;
		frames = 0;
	}

	frames++;
}

//===================================================================================================================================
//【画面クリア処理】
//===================================================================================================================================
void Direct3D9::clear()
{
	//画面クリア（実際は単色で画面を塗りつぶす処理）
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}
void Direct3D9::clear(D3DCOLOR color)
{
	//画面クリア（実際は単色で画面を塗りつぶす処理）
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
}

//===================================================================================================================================
//【画面更新処理】
//===================================================================================================================================
HRESULT Direct3D9::present()
{
	//画面更新（バックバッファをフロントバッファにする）
	device->Present(NULL, NULL, NULL, NULL);
	return S_OK;
}

//===================================================================================================================================
//【描画開始処理】
//===================================================================================================================================
HRESULT Direct3D9::beginScene()
{
	if (device == NULL)return E_FAIL;
	return device->BeginScene();//描画のためのシーンを開始する
}

//===================================================================================================================================
//【描画終了処理】
//===================================================================================================================================
HRESULT Direct3D9::endScene()
{
	if (device == NULL)return E_FAIL;
	return device->EndScene();
}

//===================================================================================================================================
//【描画先切替処理】
//[切替先]ウィンドウ全体
//===================================================================================================================================
HRESULT Direct3D9::changeViewportFullWindow()
{
	changeViewport(0, 0, viewPort.Width, viewPort.Height);
	return S_OK;
}

//===================================================================================================================================
//【描画先切替処理】
//[切替先]ウィンドウ左側
//===================================================================================================================================
HRESULT Direct3D9::changeViewport1PWindow()
{
	changeViewport(0, 0, viewPort.Width / 2, viewPort.Height);
	return S_OK;
}

//===================================================================================================================================
//【描画先切替処理】
//[切替先]ウィンドウ右側
//===================================================================================================================================
HRESULT Direct3D9::changeViewport2PWindow()
{
	changeViewport(viewPort.Width / 2, 0, viewPort.Width / 2, viewPort.Height);
	return S_OK;
}

//===================================================================================================================================
//【ビューポート切替処理】
//[用途]ビューポートを切り替えることで描画先を切り替える
//===================================================================================================================================
HRESULT Direct3D9::changeViewport(DWORD x, DWORD y, DWORD width, DWORD height)
{
	D3DVIEWPORT9 vp;

	vp.X = x;
	vp.Y = y;
	vp.Width = width;
	vp.Height = height;
	vp.MinZ = 0;
	vp.MaxZ = 1;

	
	(device->SetViewport(&vp), "ビューポート切り替え失敗");

	return S_OK;
}

//===================================================================================================================================
//【バックバッファへレンダーターゲットする】
//===================================================================================================================================
void Direct3D9::setRenderBackBuffer(DWORD index)
{
	device->SetRenderTarget(index, backBuffer);
	device->SetDepthStencilSurface(zBuffer);
}

//===================================================================================================================================
//【画面モード切替】
//===================================================================================================================================
void Direct3D9::changeDisplayMode(bool fullScreen)
{
	if (this->fullScreen == fullScreen)	return;

	this->fullScreen = fullScreen;

	if (this->fullScreen)
	{
		d3dpp = d3dppFullScreen;
	}
	else {
		d3dpp = d3dppWindow;
	}
	bool deviceLost = false;
	HRESULT hr = device->Reset(&d3dpp);
	if (FAILED(hr))
	{
		switch (hr)
		{
		case D3DERR_WRONGTEXTUREFORMAT        :
			break;
		case D3DERR_UNSUPPORTEDCOLOROPERATION :
			break;
		case D3DERR_UNSUPPORTEDCOLORARG       :
			break;
		case D3DERR_UNSUPPORTEDALPHAOPERATION :
			break;
		case D3DERR_UNSUPPORTEDALPHAARG       :
			break;
		case D3DERR_TOOMANYOPERATIONS         :
			break;
		case D3DERR_CONFLICTINGTEXTUREFILTER  :
			break;
		case D3DERR_UNSUPPORTEDFACTORVALUE    :
			break;
		case D3DERR_CONFLICTINGRENDERSTATE    :
			break;
		case D3DERR_UNSUPPORTEDTEXTUREFILTER  :
			break;
		case D3DERR_CONFLICTINGTEXTUREPALETTE :
			break;
		case D3DERR_DRIVERINTERNALERROR       :
			break;
		case D3DERR_NOTFOUND                  :
			break;
		case D3DERR_MOREDATA                  :
			break;
		case D3DERR_DEVICELOST                :
			break;
		case D3DERR_DEVICENOTRESET            :
			deviceLost = true;break;
		case D3DERR_NOTAVAILABLE              :
			break;
		case D3DERR_OUTOFVIDEOMEMORY          :
			break;
		case D3DERR_INVALIDDEVICE             :
			break;
		case D3DERR_INVALIDCALL               :
			break;
		case D3DERR_DRIVERINVALIDCALL         :
			break;
		case D3DERR_WASSTILLDRAWING           :
			break;
		case D3DOK_NOAUTOGEN                  :
			break;
		default:
			MSG("ERROR:ChangeDisplayMode Reset");
			break;
		}

	}

	if (this->fullScreen)
	{

	}
}

//===================================================================================================================================
//【Direct3Dデバイス取得関数】
//===================================================================================================================================
LPDIRECT3DDEVICE9 getDevice() { return *pointerDevice; }

//===================================================================================================================================
//【Direct3D9クラス取得関数】
//===================================================================================================================================
Direct3D9* getDirect3D9() { return pointerDirect3D9; }
