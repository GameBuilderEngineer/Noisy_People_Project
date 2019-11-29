//===================================================================================================================================
//【Direct3D9.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/17
// [更新日]2019/10/31
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//【Direct3D9クラス】
//===================================================================================================================================
class Direct3D9 :public Base
{
public:
	//Data
	HWND				wnd;
	LPDIRECT3D9			d3d;
	D3DVIEWPORT9		viewPort;
	LPDIRECT3DDEVICE9	device;
	LPDIRECT3DSURFACE9	backBuffer;		//バックバッファへのポインタ
	LPDIRECT3DSURFACE9	zBuffer;

public:
	//Method
	Direct3D9();
	~Direct3D9();
	HRESULT initialize(HWND targetWnd);
	void showFPS();
	void clear();
	void clear(D3DCOLOR color);
	HRESULT present();
	HRESULT beginScene();
	HRESULT endScene();
	HRESULT changeViewport(DWORD x, DWORD y, DWORD width, DWORD height);
	HRESULT changeViewportFullWindow();
	HRESULT changeViewport1PWindow();
	HRESULT changeViewport2PWindow();
	void setRenderBackBuffer(DWORD index);
	//HRESULT initShader();
};

//===================================================================================================================================
//【プロトタイプ宣言】
//===================================================================================================================================
//Direct3Dデバイス取得関数
extern LPDIRECT3DDEVICE9 getDevice();
//Direct3D9クラス取得関数
extern Direct3D9* getDirect3D9();
