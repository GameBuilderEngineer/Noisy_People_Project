//===================================================================================================================================
//【VirtualController.h】
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
#include <string>

//===================================================================================================================================
//【仮想コントローラ 名前空間】
//===================================================================================================================================
namespace virtualControllerNS {

	enum {
		PLAYER_1P,
		PLAYER_2P,
		MAX_PLAYER
	};
	const DWORD MAX_CONTROLLERS = MAX_PLAYER;

	const float STICK_DEADZONE		= 0.20f * 1000;		// スティックのデッドゾーンとして範囲の20%をデフォルトとする
	const float TRIGGER_DEADZONE	= 0.70f * 1000;		// トリガーのデッドゾーンとして範囲の70%をデフォルトとする

	//仮想コントローラボタン
	enum BUTTONS
	{
		A,
		B,
		X,
		Y,
		L1,
		L2,
		L3,
		R1,
		R2,
		R3,
		UP,
		DOWN,
		RIGHT,
		LEFT,
		L_STICK,
		R_STICK,
		SPECIAL_MAIN,
		SPECIAL_SUB,
		HOME,
		CAPTURE,
		MAX_BUTTON
	};
	
	// コントローラID
	// 接頭語：[D->DInput][X->XInput]
	enum CONTROLLER_ID
	{
		EMPTY,
		D_XBOE,				// XboxOneElite コントローラ[dwDevType:66069]
		D_NSPC,				// Nintendo Switch Pro コントローラ[dwDevType:66069]
		D_DS4,				// DualShock4 コントローラ[dwDevType:66328]
		MAX_CONTROLLER_ID
	};
};

//Dinput十字キー値
namespace POV{
	enum {
		UP				= 0,		// ↑
		UP_RIGHT		= 4500,		// ↗
		RIGHT			= 9000,		// →
		RIGHT_DOWN		= 13500,	// ↘
		DOWN			= 18000,	// ↓
		DOWN_LEFT		= 22500,	// ↙
		LEFT			= 27000,	// ←
		LEFT_UP			= 31500		// ↖
	};
}

//===================================================================================================================================
//【仮想コントローラクラス】：抽象インターフェース
//===================================================================================================================================
class VirtualController:public Base
{
private:
	bool recorder[virtualControllerNS::BUTTONS::MAX_BUTTON];	//1フレーム前の入力情報を保存する
	bool buttons[virtualControllerNS::BUTTONS::MAX_BUTTON];		//ボタンの入力情報
	D3DXVECTOR2 leftStick;										//左スティックの２次元入力情報
	D3DXVECTOR2 rightStick;										//右スティックの２次元入力情報
	int controller_ID;											//コントローラID
	int player_ID;												//プレイヤーID

protected:
	bool connected;					// 接続状態
	DirectInputController * DCon;
	void onButton(int VC_BUTTON) { buttons[VC_BUTTON] = true; }
	void offButton(int VC_BUTTON) { buttons[VC_BUTTON] = false; }
	void initBefore() { for (int i = 0; i < virtualControllerNS::BUTTONS::MAX_BUTTON; i++)recorder[i] = false; }
	void recordBefore() { for (int i = 0; i < virtualControllerNS::BUTTONS::MAX_BUTTON; i++)recorder[i] = buttons[i]; }
	void resetButtons() { for (int i = 0; i < virtualControllerNS::BUTTONS::MAX_BUTTON; i++)buttons[i] = false; }
	void resetStick() 	{	leftStick = D3DXVECTOR2(0, 0); rightStick = D3DXVECTOR2(0, 0); }
	
	// 十字キーを設定
	void setPOV(int pov) {
		switch (pov)
		{
		case POV::UP:			onButton(virtualControllerNS::BUTTONS::UP); break;
		case POV::RIGHT:		onButton(virtualControllerNS::BUTTONS::RIGHT); break;
		case POV::DOWN:			onButton(virtualControllerNS::BUTTONS::DOWN); break;
		case POV::LEFT:			onButton(virtualControllerNS::BUTTONS::LEFT); break;
		case POV::UP_RIGHT:		onButton(virtualControllerNS::BUTTONS::UP);		onButton(virtualControllerNS::BUTTONS::RIGHT); break;
		case POV::RIGHT_DOWN:	onButton(virtualControllerNS::BUTTONS::RIGHT);	onButton(virtualControllerNS::BUTTONS::DOWN); break;
		case POV::DOWN_LEFT:	onButton(virtualControllerNS::BUTTONS::DOWN);	onButton(virtualControllerNS::BUTTONS::LEFT); break;
		case POV::LEFT_UP:		onButton(virtualControllerNS::BUTTONS::LEFT);	onButton(virtualControllerNS::BUTTONS::UP); break;
		}
	};

	// 各軸・回転がデッドゾーンに該当するかどうか
	bool isDeadZone(LONG value)		{return value < virtualControllerNS::STICK_DEADZONE && value > -virtualControllerNS::STICK_DEADZONE ? true : false;}
	bool notDeadZone(LONG value)	{return !isDeadZone(value);}

	//スティック情報のセット
	void setX_Left_Stick(LONG axisValue)	{ leftStick.x = (float)axisValue;	onButton(virtualControllerNS::L_STICK);}// x軸：左スティック
	void setY_Left_Stick(LONG axisValue)	{ leftStick.y = (float)axisValue;	onButton(virtualControllerNS::L_STICK);}// y軸：左スティック
	void setX_Right_Stick(LONG axisValue)	{ rightStick.x = (float)axisValue;	onButton(virtualControllerNS::R_STICK);}// x軸：右スティック
	void setY_Right_Stick(LONG axisValue)	{ rightStick.y = (float)axisValue;	onButton(virtualControllerNS::R_STICK);}// y軸：右スティック

public:
	void setPlayerID(int ID) { player_ID = ID; }
	void setControllerID(int ID) { controller_ID = ID; }
	std::string getPlayerName() {
		std::string buffer;
		switch (player_ID)
		{
		case virtualControllerNS::PLAYER_1P: buffer = "Player01"; break;
		case virtualControllerNS::PLAYER_2P: buffer = "Player02"; break;
		}
		return buffer;
	}
	std::string getControllerName() {
		if (DCon == NULL)return "NULL";
		return DCon->name;
	}
	int getControllerID() { return controller_ID; };
	virtual void update() = 0;

	//接続状態を確認
	bool checkConnect() { return connected; }

	// 直前のボタンの状態を取得
	bool getBefore(int VC_BUTTON) { return recorder[VC_BUTTON]; }

	// ボタンの状態を取得
	bool getButton(int VC_BUTTON) { return buttons[VC_BUTTON]; }

	// コントローラの指定したボタンの状態を戻す
	// 押している間はtureを戻す
	bool isButton(int VC_BUTTON)
	{
		return getButton(VC_BUTTON);
	}

	// コントローラnの指定したボタンの状態を戻す
	// 押されたときのみtrueを返す
	bool wasButton(int VC_BUTTON)
	{
		if (getBefore(VC_BUTTON) == true) { return false; }
		else { return getButton(VC_BUTTON); }
	}

	// スティックの情報を取得
	D3DXVECTOR2 getLeftStick()			{ return leftStick; }
	D3DXVECTOR2 getLeftStickTrigger() 
	{	if (getBefore(virtualControllerNS::L_STICK) == true) { return D3DXVECTOR2(0,0); }
		else { return leftStick; }}
	D3DXVECTOR2 getRightStick() { return rightStick; }
	D3DXVECTOR2 getRightStickTrigger() 
	{	if (getBefore(virtualControllerNS::R_STICK) == true) { return D3DXVECTOR2(0,0); }
		else { return rightStick; }}
};

//===================================================================================================================================
// 空のコントローラ
// コントローラを取得できなかった場合はこのクラスで初期化する
//===================================================================================================================================
class EMPTY_CONTROLLER :public VirtualController
{
private:
	//常にfalseや0値を返す機能しないコントローラ
public:
	EMPTY_CONTROLLER() {
		setControllerID(virtualControllerNS::EMPTY);
		setPlayerID(-1);
		DCon = NULL;
		initBefore();
		recordBefore();
		resetButtons();
		resetStick();
		connected = false;
	};

	virtual void update()
	{
		recordBefore();
		resetButtons();
		resetStick();
	};
};

//===================================================================================================================================
// [DInput型]VC
// XBOX ONE ELITEコントローラ
//===================================================================================================================================
class D_XBOX_ONE_ELITE :public VirtualController
{
private:
	const int A = 0;
	const int B = 1;
	const int X = 2;
	const int Y = 3;
	const int LB = 4;
	const int LSB = 8;
	const int RB = 5;
	const int RSB = 9;
	const int MENU = 7;
	const int VIEW = 6;

public:
	// プレイヤー番号を設定し、取得しているDInputからどれを参照するか指定した上で作成する。
	D_XBOX_ONE_ELITE(int playerID, DirectInputController* DC) {
		setControllerID(virtualControllerNS::D_XBOE);
		setPlayerID(playerID);
		DCon = DC;
		initBefore();
		connected = true;
	};

	virtual void update()
	{
		recordBefore();
		resetButtons();
		resetStick();
		if (DCon->joyState.rgbButtons[A] & 0x80)		onButton(virtualControllerNS::B);
		if (DCon->joyState.rgbButtons[B] & 0x80)		onButton(virtualControllerNS::A);
		if (DCon->joyState.rgbButtons[X] & 0x80)		onButton(virtualControllerNS::Y);
		if (DCon->joyState.rgbButtons[Y] & 0x80)		onButton(virtualControllerNS::X);
		if (DCon->joyState.rgbButtons[LB] & 0x80)		onButton(virtualControllerNS::L1);
		if (DCon->joyState.rgbButtons[LSB] & 0x80)		onButton(virtualControllerNS::L3);
		if (DCon->joyState.rgbButtons[RB] & 0x80)		onButton(virtualControllerNS::R1);
		if (DCon->joyState.rgbButtons[RSB] & 0x80)		onButton(virtualControllerNS::R3);
		if (DCon->joyState.rgbButtons[MENU] & 0x80)		onButton(virtualControllerNS::SPECIAL_MAIN);
		if (DCon->joyState.rgbButtons[VIEW] & 0x80)		onButton(virtualControllerNS::SPECIAL_SUB);
		// トリガー検知
		if (DCon->joyState.lZ > virtualControllerNS::TRIGGER_DEADZONE) onButton(virtualControllerNS::L2);
		if (DCon->joyState.lZ < -virtualControllerNS::TRIGGER_DEADZONE) onButton(virtualControllerNS::R2);
		// 左スティック入力値を格納
		if (notDeadZone(DCon->joyState.lX)) setX_Left_Stick(DCon->joyState.lX);
		if (notDeadZone(DCon->joyState.lY)) setY_Left_Stick(DCon->joyState.lY);

		// 右スティック入力値を格納
		if (notDeadZone(DCon->joyState.lRx)) setX_Right_Stick(DCon->joyState.lRx);
		if (notDeadZone(DCon->joyState.lRy)) setY_Right_Stick(DCon->joyState.lRy);
		// 十字キーの設定
		setPOV(DCon->joyState.rgdwPOV[0]);
	};
};

//===================================================================================================================================
// [DInput型]VC
// NintendoSwitchProコントローラ
//===================================================================================================================================
class Nintendo_Switch_Pro_Contoroller :public VirtualController
{
private:
	const int B = 0;			// joystick_Button01
	const int A = 1;			// joystick_Button02
	const int Y = 2;			// joystick_Button03
	const int X = 3;			// joystick_Butoon04
	const int L = 4;			// joystick_Button05
	const int R = 5;			// joystick_Button06
	const int ZL = 6;			// joystick_Button07
	const int ZR = 7;			// joystick_Button08
	const int MINUS = 8;		// joystick_Button09
	const int PLUS = 9;			// joystick_Button10
	const int LSB = 10;			// joystick_Button11
	const int RSB = 11;			// joystick_Button12
	const int HOME = 12;		// joystick_Button13
	const int CAPTURE = 13;		// joystick_Button14

public:
	Nintendo_Switch_Pro_Contoroller(int playerID, DirectInputController* DC) {
		setPlayerID(playerID);
		setControllerID(virtualControllerNS::D_NSPC);
		DCon = DC;
		initBefore();
		connected = true;
	};

	virtual void update()
	{
		recordBefore();
		resetButtons();
		resetStick();
		if (DCon->joyState.rgbButtons[A] & 0x80)		onButton(virtualControllerNS::A);
		if (DCon->joyState.rgbButtons[B] & 0x80)		onButton(virtualControllerNS::B);
		if (DCon->joyState.rgbButtons[X] & 0x80)		onButton(virtualControllerNS::X);
		if (DCon->joyState.rgbButtons[Y] & 0x80)		onButton(virtualControllerNS::Y);
		if (DCon->joyState.rgbButtons[L] & 0x80)		onButton(virtualControllerNS::L1);
		if (DCon->joyState.rgbButtons[LSB] & 0x80)		onButton(virtualControllerNS::L3);
		if (DCon->joyState.rgbButtons[R] & 0x80)		onButton(virtualControllerNS::R1);
		if (DCon->joyState.rgbButtons[RSB] & 0x80)		onButton(virtualControllerNS::R3);
		if (DCon->joyState.rgbButtons[PLUS] & 0x80)		onButton(virtualControllerNS::SPECIAL_MAIN);
		if (DCon->joyState.rgbButtons[MINUS] & 0x80)	onButton(virtualControllerNS::SPECIAL_SUB);
		if (DCon->joyState.rgbButtons[HOME] & 0x80)		onButton(virtualControllerNS::HOME);
		if (DCon->joyState.rgbButtons[CAPTURE] & 0x80)	onButton(virtualControllerNS::CAPTURE);
		// トリガー検知
		if (DCon->joyState.rgbButtons[ZL] & 0x80)		onButton(virtualControllerNS::L2);
		if (DCon->joyState.rgbButtons[ZR] & 0x80)		onButton(virtualControllerNS::R2);
		// 左スティック入力値を格納
		if (notDeadZone(DCon->joyState.lX)) setX_Left_Stick(DCon->joyState.lX);
		if (notDeadZone(DCon->joyState.lY)) setY_Left_Stick(DCon->joyState.lY);
		// 右スティック入力値を格納
		if (notDeadZone(DCon->joyState.lRx)) setX_Right_Stick(DCon->joyState.lRx);
		if (notDeadZone(DCon->joyState.lRy)) setY_Right_Stick(DCon->joyState.lRy);
		// 十字キーの設定
		setPOV(DCon->joyState.rgdwPOV[0]);
	};
};

//===================================================================================================================================
// [DInput型]VC
// DualShock4コントローラ
//===================================================================================================================================
class DualShock4 :public VirtualController
{
private:
	const int SQUARE = 0;		// □01
	const int CROSS = 1;		// ×02
	const int CIRCLE = 2;		// 〇03
	const int TRIANGLE = 3;		// △04
	const int L1 = 4;			// 05
	const int R1 = 5;			// 06
	const int L2 = 6;			// 07
	const int R2 = 7;			// 08
	const int SHARE = 8;		// 09
	const int OPTION = 9;		// 10
	const int L3 = 10;			// 11
	const int R3 = 11;			// 12
	const int PS = 12;			// 13
	const int TOUCHPAD = 13;	// 14

public:
	DualShock4(int playerID, DirectInputController* DC) {
		setPlayerID(playerID);
		setControllerID(virtualControllerNS::D_DS4);
		DCon = DC;
		initBefore();
		connected = true;
	};

	virtual void update()
	{
		recordBefore();
		resetButtons();
		resetStick();
		if (DCon->joyState.rgbButtons[CIRCLE] & 0x80)	onButton(virtualControllerNS::A);
		if (DCon->joyState.rgbButtons[CROSS] & 0x80)	onButton(virtualControllerNS::B);
		if (DCon->joyState.rgbButtons[TRIANGLE] & 0x80)	onButton(virtualControllerNS::X);
		if (DCon->joyState.rgbButtons[SQUARE] & 0x80)	onButton(virtualControllerNS::Y);
		if (DCon->joyState.rgbButtons[L1] & 0x80)		onButton(virtualControllerNS::L1);
		if (DCon->joyState.rgbButtons[L3] & 0x80)		onButton(virtualControllerNS::L3);
		if (DCon->joyState.rgbButtons[R1] & 0x80)		onButton(virtualControllerNS::R1);
		if (DCon->joyState.rgbButtons[R3] & 0x80)		onButton(virtualControllerNS::R3);
		if (DCon->joyState.rgbButtons[OPTION] & 0x80)	onButton(virtualControllerNS::SPECIAL_MAIN);
		if (DCon->joyState.rgbButtons[TOUCHPAD] & 0x80)	onButton(virtualControllerNS::SPECIAL_SUB);
		if (DCon->joyState.rgbButtons[PS] & 0x80)		onButton(virtualControllerNS::HOME);
		if (DCon->joyState.rgbButtons[SHARE] & 0x80)	onButton(virtualControllerNS::CAPTURE);
		// トリガー検知
		if (DCon->joyState.rgbButtons[L2] & 0x80)		onButton(virtualControllerNS::L2);
		if (DCon->joyState.rgbButtons[R2] & 0x80)		onButton(virtualControllerNS::R2);
		// 左スティック入力値を格納
		if (notDeadZone(DCon->joyState.lX)) setX_Left_Stick(DCon->joyState.lX);
		if (notDeadZone(DCon->joyState.lY)) setY_Left_Stick(DCon->joyState.lY);
		// 右スティック入力値を格納
		if (notDeadZone(DCon->joyState.lZ))  setX_Right_Stick(DCon->joyState.lZ);
		if (notDeadZone(DCon->joyState.lRz)) setY_Right_Stick(DCon->joyState.lRz);
		// 十字キーの設定
		setPOV(DCon->joyState.rgdwPOV[0]);
	};
};
