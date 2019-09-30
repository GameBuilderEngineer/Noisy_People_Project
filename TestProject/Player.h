//===================================================================================================================================
//【Player.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/24
// [更新日]2019/09/24
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "StaticMeshObject.h"
#include "BoundingSphere.h"
#include "Input.h"
#include "Camera.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace playerNS{

	enum PLAYER_TYPE{
		PLAYER1,
		PLAYER2,
		TITLE_PLAYER,
		NUM_PLAYER
	};

	struct OperationKeyTable
	{
		BYTE front;
		BYTE back;
		BYTE right;
		BYTE left;
		BYTE reset;
		BYTE dash;
		BYTE jump;
		BYTE reverseCameraX;
		BYTE reverseCameraY;
	};

	const OperationKeyTable KEY_TABLE_1P = {
		'W',		//FRONT
		'S',		//BACK
		'D',		//RIGHT
		'A',		//LEFT
		'R',		//RESET
		VK_LSHIFT,	//DASH
		VK_SPACE,	//JUMP
		VK_F11,		//ReverseCameraAxisX
		VK_F12,		//ReverseCameraAxisY
	};

	const OperationKeyTable NON_CONTOROL = {
		VK_ESCAPE,		//FRONT
		VK_ESCAPE,		//BACK
		VK_ESCAPE,		//RIGHT
		VK_ESCAPE,		//LEFT
		VK_ESCAPE,		//RESET
		VK_ESCAPE,		//DASH
		VK_ESCAPE,		//JUMP
		VK_ESCAPE,		//ReverseCameraAxisX
		VK_ESCAPE,		//ReverseCameraAxisY
	};

	const BYTE BUTTON_JUMP = virtualControllerNS::B;
	const BYTE BUTTON_BULLET = virtualControllerNS::R1;
	const BYTE BUTTON_CUT = virtualControllerNS::X;
	const BYTE BUTTON_REVIVAL = virtualControllerNS::A;
	const BYTE BUTTON_PILE = virtualControllerNS::L1;

	enum STATE {
		GROUND,
		STATE_NUM
	};

	const int MAX_HP					= 100;		//ＨＰ最大値
	const float SPEED					= 100.0f;	//速度
	const float DASH_MAGNIFICATION		= 2.0f;		//ダッシュ倍率
	const float JUMP_FORCE				= 30.0f;	//ジャンプ力
	const float GRAVITY_FORCE			= 9.8f*7.0;	//重力
	const float DIFFERENCE_FIELD		= 0.2f;		//フィールド補正差分
	const float INVINCIBLE_TIME			= 3.0f;		//無敵時間
	const float CAMERA_SPEED			= 1.0f;		//カメラの速さ

	//プレイヤーのスタートポジション
	const D3DXVECTOR3 START_POSITION =	D3DXVECTOR3(0, 1000, 0);

}

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
class Player : public StaticMeshObject
{
protected:
	//汎用デバイス・クラス
	LPDIRECT3DDEVICE9 device;
	Input* input;
	int type;
	//Sound* sound;										//サウンドクラス

	//ステータス
	Camera* camera;										//カメラへのポインタ
	playerNS::OperationKeyTable keyTable;				//操作Keyテーブル
	int state;											//状態変数

	//球コリジョン
	BoundingSphere bodyCollide;

	//重力処理に利用
	LPD3DXMESH attractorMesh;							//重力（引力）発生メッシュ
	D3DXMATRIX* attractorMatrix;						//重力（引力）発生オブジェクトマトリックス

	//タイマー
	float invincibleTimer;								//無敵時間

	//操作関係
	float reverseValueXAxis;							//操作X軸
	float reverseValueYAxis;							//操作Y軸
	bool onJump;										//ジャンプフラグ

	//接地関係
	float difference;									//フィールド補正差分
	bool onGround;										//接地判定

	//サウンドGUI
	bool onSound;										//サウンドGUIのフラグ
	int volume;											//ボリューム
	void outputSoundGUI();								//サウンドGUIの中身
public:

	Player();
	~Player();

	//processing
	virtual void initialize(int playerType, int modelType);
	virtual void update(float frameTime);
	void toonRender(LPDIRECT3DDEVICE9 device, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon,
		LPD3DXEFFECT effect, LPDIRECT3DTEXTURE9 textureShade, LPDIRECT3DTEXTURE9 textureLine);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//operation
	void configurationGravityWithRay(D3DXVECTOR3* attractorPosition, LPD3DXMESH attractorMesh, D3DXMATRIX* attractorMatrix);
	void moveOperation();
	void move(D3DXVECTOR2 moveDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ);
	void jump();
	void reset();
	void updateGround(float frameTime, bool onJump);
	void controlCamera(float frameTime);
	virtual void outputGUI() override;

	//setter
	void setCamera(Camera* _camera);

	//getter
	int getState();
	BoundingSphere* getBodyCollide();	//球コリジョン

};