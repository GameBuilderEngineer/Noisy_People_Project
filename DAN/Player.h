//===================================================================================================================================
//【Player.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/24
// [更新日]2019/10/18
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
		'W',					//FRONT
		'S',					//BACK
		'D',					//RIGHT
		'A',					//LEFT
		'R',					//RESET
		VK_LSHIFT,				//DASH
		VK_SPACE,				//JUMP
		VK_F11,					//ReverseCameraAxisX
		VK_F12,					//ReverseCameraAxisY
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

	const BYTE BUTTON_JUMP			= virtualControllerNS::B;
	const BYTE BUTTON_BULLET		= virtualControllerNS::R1;
	const BYTE BUTTON_CUT			= virtualControllerNS::X;
	const BYTE BUTTON_REVIVAL		= virtualControllerNS::A;
	const BYTE BUTTON_PILE			= virtualControllerNS::L1;

	enum STATE {
		NORMAL,
		VISION,
		SKY_VISION,
		SHIFT,
		STATE_NUM
	};

	const D3DXVECTOR3 START_POSITION = D3DXVECTOR3(-35, 10, 0);
	//const D3DXVECTOR3 START_POSITION = D3DXVECTOR3(0, 0, 0);

	// StatusParameter
	const int	MAX_HP						= 100;									// ＨＰ最大値
	const int	FULL_POWER					= 100;									// 電力ゲージ一本
	const int	MAX_POWER					= FULL_POWER * 3;						// 電力ゲージ最大値
	const float INVINCIBLE_TIME				= 3.0f;									// 無敵時間

	// Physics
	const float MOVE_ACC					= 27.0f;								// 移動加速度
	const float MOVE_ACC_WHEN_NOT_GROUND	= MOVE_ACC / 8.5f;						// 空中移動加速度
	const float STOP_SPEED					= 0.5f;									// 移動停止速度
	const float FALL_SPEED_MAX				= 60.0f;								// 落下最高速度
	const float MOVE_FRICTION				= 0.93f;								// 地面摩擦係数
	const float WALL_FRICTION				= 0.98;									// 壁ずり摩擦係数
	const float GROUND_FRICTION				= 0.25;									// 着地摩擦係数
	const float GRAVITY_FORCE				= 9.8f * 2;								// 重力
	const float JUMP_SPEED					= 6.0f;									// ジャンプ初速
	const float JUMP_CONTROL_SPEED			= 1.0f;									// ジャンプ高さコントール速度
	const float DASH_MAGNIFICATION			= 2.0f;									// ダッシュ倍率
	// Another
	const float DIFFERENCE_FIELD			= 0.05f;								// フィールド補正差分
	const float CAMERA_SPEED				= 1.0f;									// カメラの速さ
}


//===================================================================================================================================
//【クラス定義】
//===================================================================================================================================
class Player : public Object
{
private:
	//ステータス
	int							type;							//プレイヤータイプ
	playerNS::OperationKeyTable	keyTable;						//操作Keyテーブル
	int							state;							//状態変数
	int							hp;								// HP
	int							power;							// 電力

	//タイマー
	float						invincibleTimer;				//無敵時間

	//操作
	float						reverseValueXAxis;				//操作X軸
	float						reverseValueYAxis;				//操作Y軸
	bool						isExecutingMoveOperation;		// 移動操作中フラグ
	bool						onJump;							// ジャンプフラグ
	bool						jumping;						// ジャンプ中フラグ

	// UI用操作判定フラグ
	bool						isShotAble;
	bool						isJumpAble;
	bool						isVisionAble;
	bool						isSkyVisionAble;
	bool						isShiftAble;

	// 衝突
	BoundingSphere				bodyCollide;					// 球コリジョン
	float						difference;						//フィールド補正差分
	bool						onGround;						//接地判定
	bool						onGroundBefore;					// 直前フレームの接地判定

	// 物理
	float						friction;						// 摩擦係数
	Ray							ray;							// レイ
	LPD3DXMESH					attractorMesh;					//重力（引力）発生メッシュ
	D3DXMATRIX*					attractorMatrix;				//重力（引力）発生オブジェクトマトリックス

	//サウンドGUI
	bool						onSound;						//サウンドGUIのフラグ
	int							volume;							//ボリューム

	// 汎用
	LPDIRECT3DDEVICE9			device;						// Direct3Dデバイス
	Input*						input;						// 入力系
	Camera*						camera;						// 操作するカメラへのポインタ
	D3DXVECTOR3					centralPosition;			// 中心座標
	D3DXMATRIX					centralMatrixWorld;			// 中心座標ワールドマトリクス

public:
	Player();
	~Player();

	// 基本処理
	virtual void initialize(int playerType, int modelType);
	virtual void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	// 衝突
	void grounding();											// 接地処理
	void wallScratch();											// 壁ずり処理

	// 物理
	void configurationGravityWithRay(D3DXVECTOR3* attractorPosition,
		LPD3DXMESH attractorMesh, D3DXMATRIX* attractorMatrix);
	void physicalBehavior();									// 物理挙動
	void updatePhysics(float frameTime);						// 物理の更新

	//操作
	void moveOperation();										// 移動操作
	void jumpOperation();										// ジャンプ操作
	void controlCamera(float frameTime);						// カメラ操作

	// アクション
	void move(D3DXVECTOR2 moveDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ);//移動
	void jump();												//ジャンプ
	float dash();

	// その他
	virtual void outputGUI() override;							// ImGUI
	void outputSoundGUI();										// サウンドGUIの中身
	void reset();												// リセット

	//setter
	void setCamera(Camera* _camera);							//操作対象カメラのセット

	//getter
	int getState();
	int getHp();												// HPの取得
	int getPower();												// 電力の取得
	bool canShot();
	bool canJump();
	bool canDoVision();
	bool canDoSkyVision();
	bool canShift();
	BoundingSphere* getBodyCollide();							//球コリジョンの取得
};