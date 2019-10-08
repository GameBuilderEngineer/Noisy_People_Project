//===================================================================================================================================
//【Input.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/19
// [更新日]2019/09/19
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "DirectInputController.h"
#include "VirtualController.h"
#include <WindowsX.h>

//===================================================================================================================================
//【マクロ定義】
//===================================================================================================================================
// 高精細マウス用
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC		((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE		((USHORT) 0x02)
#endif

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace inputNS
{
	enum {
		DINPUT_1P,
		DINPUT_2P,
		DINPUT_3P,
		DINPUT_4P,
		NUM_DINPUT_CONTROLLER,
	};
	const int KEYS_ARRAY_LEN = 256;		// キー配列のサイズ

	enum MOUSE_WHEEL_STATE {
		NONE,
		UP,
		DOWN,
	};

	// clear()、ビットフラグ
	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

//===================================================================================================================================
//【入力クラス】
//===================================================================================================================================
class Input : public Base
{
private:
	HWND wnd;

	//-----------------------
	//KeyBoard
	//-----------------------
	bool keysDown[inputNS::KEYS_ARRAY_LEN];			// 指定されたキーがダウンしている場合はtrue
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];		// 指定されたキーが押された場合はtrue
	std::string textIn;								// ユーザーが入力したテキスト
	char charIn;									// 最後に入力した文字
	bool newLine;									// 改行の開始時にtrue

	//-----------------------
	//Mouse
	//-----------------------
	int mouseRawX, mouseRawY;		// 高解像度のマウスデータ
	int mouseX, mouseY;				// マウスの画面座標
	RAWINPUTDEVICE Rid[1];			// 高精細マウス用
	bool mouseCaptured;				// マウスがキャプチャされた場合はtrue
	bool mouseLButton;				// マウスの左ボタンが押されている場合はtrue
	bool mouseMButton;				// 中マウスボタンが押されている場合はtrue
	bool mouseRButton;				// マウスの右ボタンが押されている場合はtrue
	bool mouseX1Button;				// X1のマウスボタンが押されている場合はtrue
	bool mouseX2Button;				// X2のマウスボタンが押されている場合はtrue
	bool mouseBuffer[5] = { false,false,false,false,false };
	bool mouseButtonTrigger[5] = { false,false,false,false,false };						// マウスのボタンが押された時一度だけtrue,順番は上準拠;
	int wheelFraction;				//回転量の端数
	int zDelta;						//回転量
	int mouseWheelState;			//回転方向の状態変数

	//-----------------------
	//DInputController
	//-----------------------
	LPDIRECTINPUT8 dinput = NULL;												// DirectInputインターフェース
	DirectInputController dInputController[inputNS::NUM_DINPUT_CONTROLLER];		// DirectInputコントローラ
	int enumDInputNum;

	//-----------------------
	//仮想コントローラインターフェース
	//-----------------------
	VirtualController* virtualController[inputNS::NUM_DINPUT_CONTROLLER];

public:
	Input();
	virtual ~Input();
	HRESULT initialize(HINSTANCE instance, HWND _wnd,bool capture);
	void update(bool windowActivate);
	
	void clear(UCHAR what);									// 指定された入力バッファをクリアする。// KEYS_DOWN、KEYS_PRESSED、MOUSE、TEXT_INまたはKEYS_MOUSE_TEXT。// OR '|'を使用するパルメータを組み合わせるオペレータ。
	void clearAll() { clear(inputNS::KEYS_MOUSE_TEXT); }	// キー、マウス、およびテキスト入力データをクリアします。

	//-----------------------
	//Controller
	//-----------------------
	VirtualController** getController() { return virtualController; };
	HRESULT resetController();

	//-----------------------
	//KeyBoard
	//-----------------------
	void keyDown(WPARAM);									// キーダウン状態を保存する
	void keyPress(WPARAM);									// キープレス状態を保存する
	void keyUp(WPARAM);										// キーアップ状態を保存する
	void keyIn(WPARAM);										// textIn string に入力したばかりの文字を保存する
	bool isKeyDown(UCHAR vkey) const;						// 指定された仮想キーがダウンしている場合はtrue、そうでない場合はfalseを返します。
	bool wasKeyPressed(UCHAR vkey) const;					// 指定されたVIRTUAL KEYが最後のフレームで押された場合はtrueを返します。// キーの押下は、各フレームの最後で消去されます。
	bool anyKeyPressed() const;								// 直近のフレームでキーが押された場合はtrueを返します。// キーの押下は、各フレームの最後で消去されます。
	void clearKeyPress(UCHAR vkey);							// 指定されたキーをクリアする
	void clearTextIn() { textIn.clear(); }					// クリアテキスト入力バッファ
	std::string getTextIn() { return textIn; }				// テキスト入力を文字列として返す
	char getCharIn() { return charIn; }						// 最後に入力した文字を返します

	//-----------------------
	//Mouse
	//-----------------------
	void mouseIn(LPARAM);									// マウスの位置をmouseX、mouseYに読み込みます
	void mouseRawIn(LPARAM);								// 生のマウスデータをmouseRawX、mouseRawYに読み込みます// このルーチンは高精細マウスと互換性があります
	void mouseWheelIn(WPARAM wParam);						// マウスの回転量を保存
	void clearWheelFraction() { wheelFraction = 0; };		// ウィンドウのアクティブ切り替え時に端数をリセット
	void setMouseLButton(bool b) { mouseLButton = b; }		// 左マウスボタンの状態を保存
	void setMouseMButton(bool b) { mouseMButton = b; }		// 中央マウスボタンの状態を保存
	void setMouseRButton(bool b) { mouseRButton = b; }		// 右マウスボタンの状態を保存
	void setMouseXButton(WPARAM wParam) {mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;mouseX2Button = (wParam & MK_XBUTTON2) ? true : false;}	// Xマウスボタンの状態を保存
	int getMouseX()		const { return mouseX; }			// マウスのX位置を戻す
	int getMouseY()		const { return mouseY; }			// マウスのY位置を戻す
	int getMouseRawX()	const { return mouseRawX; }			// マウスのX位置の移動のローデータを戻す。左への移動は<0、右への移動は>0//高精細マウスと互換性があります。
	int getMouseRawY()	const { return mouseRawY; }			// マウスのY位置の移動のローデータを戻す。左への移動は<0、右への移動は>0// 高精細マウスと互換性があります。
	int getMouseWheelState() { return mouseWheelState; };
	bool getMouseLButton()	const { return mouseLButton; }	// 左マウスボタンの状態を戻す
	bool getMouseLButtonTrigger() { return mouseButtonTrigger[0]; }	// 左マウスボタンのトリガー情報を戻す
	bool getMouseMButton()	const { return mouseMButton; }	// 中央マウスボタンの状態を戻す
	bool getMouseMButtonTrigger() { return mouseButtonTrigger[1]; }// 中央マウスボタンのトリガー情報を戻す
	bool getMouseRButton()	const { return mouseRButton; }	// 右マウスボタンの状態を戻す
	bool getMouseRButtonTrigger() { return mouseButtonTrigger[2]; }	// 右マウスボタンのトリガー情報を戻す
	bool getMouseX1Button()	const { return mouseX1Button; }	// X1マウスボタンの状態を戻す
	bool getMouseX1ButtonTrigger() { return mouseButtonTrigger[3]; }	// X1マウスボタンのトリガー情報を戻す
	bool getMouseX2Button()	const { return mouseX2Button; }	// X2マウスボタンの状態を戻す
	bool getMouseX2ButtonTrigger() { return mouseButtonTrigger[4]; }	// X2マウスボタンのトリガー情報を戻す
	void updateMouse();

	//-----------------------
	//DInputController
	//-----------------------
	static BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

};


//===================================================================================================================================
//【プロトタイプ宣言】
//===================================================================================================================================
//インプットクラス取得関数
extern Input* getInput();
