//===================================================================================================================================
//【DirectInputController.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/19
// [更新日]2019/09/19
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "DirectInputController.h"

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
HRESULT DirectInputController::initialize(HWND wnd)
{
	connected = false;

	if (device == NULL)return E_FAIL;

	// デバイスをジョイスティックに設定
	MFAIL(device->SetDataFormat(&c_dfDIJoystick2), "ジョイスティックの設定に失敗しました。");

	// 協調レベルの設定
	MFAIL(device->SetCooperativeLevel(wnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND),"協調レベルの設定に失敗しました。");

	//アプリケーションで使用するコントローラーのプロパティを列挙して設定する
	MFAIL(device->EnumObjects(EnumObjectsCallback, this, DIDFT_AXIS),"コントローラプロパティの列挙と設定に失敗しました。");
	
	// 軸モードを設定（絶対値モードに設定。デフォルトなので必ずしも設定は必要ない）
	DIPROPDWORD propWord;
	propWord.diph.dwSize = sizeof(propWord);
	propWord.diph.dwHeaderSize = sizeof(propWord.diph);
	propWord.diph.dwObj = 0;
	propWord.diph.dwHow = DIPH_DEVICE;
	propWord.dwData = DIPROPAXISMODE_ABS;
	//diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードの場合
	if (FAILED(device->SetProperty(DIPROP_AXISMODE, &propWord.diph)))
	{
		MSG("軸モードの設定に失敗");
		return E_FAIL;
	}

	// バッファリング・データを取得するため、バッファ・サイズを設定
	propWord.dwData = DIDEVICE_BUFFERSIZE;
	if (FAILED(device->SetProperty(DIPROP_BUFFERSIZE, &propWord.diph)))
	{
		MSG("バッファ・サイズの設定に失敗");
		return E_FAIL;
	}

	// 入力制御開始
	connected = true;
	device->Acquire();
	return S_OK;
}

//===================================================================================================================================
//【アプリケーションで使用するコントローラーのプロパティを列挙して設定する】：コールバック関数
//===================================================================================================================================
BOOL CALLBACK DirectInputController::EnumObjectsCallback(LPCDIDEVICEOBJECTINSTANCE objectInstance, VOID* context)
{
	DirectInputController* obj = (DirectInputController*)context;

	DIPROPRANGE propRange;//軸範囲
	ZeroMemory(&propRange, sizeof(propRange));
	propRange.diph.dwSize = sizeof(propRange);
	propRange.diph.dwHeaderSize = sizeof(propRange.diph);
	propRange.diph.dwHow = DIPH_BYID;
	propRange.diph.dwObj = objectInstance->dwType;
	propRange.lMin = -1000;
	propRange.lMax = +1000;

	if (FAILED(obj->device->SetProperty(DIPROP_RANGE, &propRange.diph)))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void DirectInputController::update(bool windowActivate)
{
	if (device == NULL)	return;

	HRESULT hr;
	// デバイスの直接データを取得する
	hr = device->Poll();
	if (FAILED(hr))
	{
		hr = device->Acquire();
		while (windowActivate && hr == DIERR_INPUTLOST)
		{
			hr = device->Acquire();
		}
	}

	// コントローラーの状態を取得する
	hr = device->GetDeviceState(sizeof(DIJOYSTATE2), &joyState);
	if (FAILED(hr))
	{
		if (windowActivate && hr == DIERR_INPUTLOST)
		{
			device->Acquire();
		}
	}

	// バッファリングデータを取得する
	while (windowActivate)
	{
		DWORD items = 1;
		hr = device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &objectData, &items, 0);
		if (hr == DIERR_INPUTLOST)
		{
			device->Acquire();
		}
		else if (FAILED(hr) || items == 0)
		{
			break;// データが読めないか、存在しない
		}
	}

}