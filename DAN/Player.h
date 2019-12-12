//===================================================================================================================================
//【Player.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/24
// [更新日]2019/11/13
//===================================================================================================================================
#pragma once

//----------------------------
// プレイヤーの物理パラメータ
//----------------------------
// 移動最高速度				6m/s
// 静止ジャンプ高さ最大値	2m
// 静止ジャンプ滞空時間 	0.8s（ジャンプを開始してから着地するまでの時間）
// 移動ジャンプ最長飛距離	4.8m(6 * 0.8)


//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "StaticMeshRenderer.h"
#include "BoundingSphere.h"
#include "Input.h"
#include "Camera.h"
#include "GameMaster.h"
#include "Bullet.h"
#include "DigitalShiftEffect.h"
#include "Sound.h"

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
		BYTE front;					//(01)
		BYTE back;					//(02)
		BYTE right;					//(03)
		BYTE left;					//(04)
		BYTE reset;					//(05)
		BYTE dash;					//(06)
		BYTE jump;					//(07)
		BYTE vision;				//(08)
		BYTE cancelVision;			//(09)
		BYTE skyVision;				//(11)
		BYTE cancelSkyVision;		//(12)
		BYTE digitalShift;			//(13)
		BYTE reverseCameraX;		//(14)
		BYTE reverseCameraY;		//(15)
	};

	const OperationKeyTable KEY_TABLE[gameMasterNS::PLAYER_NUM] = {
		//1P
		{
			'W',					//(01)FRONT
			'S',					//(02)BACK
			'D',					//(03)RIGHT
			'A',					//(04)LEFT
			'R',					//(05)RESET
			VK_ESCAPE,				//(06)DASH:廃止
			VK_ESCAPE,				//(07)JUMP:右クリック
			VK_LSHIFT,				//(08)VISION
			VK_LSHIFT,				//(09)CANCEL_VISION
			VK_SPACE,				//(11)SKY_VISION
			VK_SPACE,				//(12)CANCEL_SKY_VISION
			VK_ESCAPE,				//(13)DIGITAL_SHIFT:左クリック
			VK_F7,					//(14)ReverseCameraAxisX
			VK_F8,					//(15)ReverseCameraAxisY
		},
		//2P
		{
			VK_UP,					//(01)FRONT
			VK_DOWN,				//(02)BACK
			VK_RIGHT,				//(03)RIGHT
			VK_LEFT,				//(04)LEFT
			'R',					//(05)RESET
			VK_ESCAPE,				//(06)DASH
			VK_ESCAPE,				//(07)JUMP:右クリック
			VK_RSHIFT,				//(08)VISION
			VK_RSHIFT,				//(09)CANCEL_VISION
			VK_BACK,				//(11)SKY_VISION
			VK_BACK,				//(12)CANCEL_SKY_VISION
			VK_ESCAPE,				//(13)DIGITAL_SHIFT:左クリック
			VK_F9,					//(14)ReverseCameraAxisX
			VK_F10,					//(15)ReverseCameraAxisY
		}
	};

	const OperationKeyTable NON_CONTOROL = {
		VK_ESCAPE,					//(01)FRONT
		VK_ESCAPE,					//(02)BACK
		VK_ESCAPE,					//(03)RIGHT
		VK_ESCAPE,					//(04)LEFT
		VK_ESCAPE,					//(05)RESET
		VK_ESCAPE,					//(06)DASH
		VK_ESCAPE,					//(07)JUMP:右クリック
		VK_ESCAPE,					//(08)VISION
		VK_ESCAPE,					//(09)CANCEL_VISION
		VK_ESCAPE,					//(11)SKY_VISION
		VK_ESCAPE,					//(12)CANCEL_SKY_VISION
		VK_ESCAPE,					//(13)DIGITAL_SHIFT:左クリック
		VK_ESCAPE,					//(14)ReverseCameraAxisX
		VK_ESCAPE,					//(15)ReverseCameraAxisY
	};

	const BYTE BUTTON_VISION		= virtualControllerNS::Y;
	const BYTE BUTTON_SKY_VISION	= virtualControllerNS::X;
	const BYTE BUTTON_DIGITA_SHIFT	= virtualControllerNS::R1;
	const BYTE BUTTON_JUMP			= virtualControllerNS::B;
	const BYTE BUTTON_BULLET		= virtualControllerNS::R1;
	const BYTE BUTTON_PASUE			= virtualControllerNS::SPECIAL_MAIN;

	enum STATE {
		NONE,
		NORMAL,
		VISION,
		SKY_VISION,
		DIGITAL_SHIFT,
		DEATH,
		STATE_NUM
	};

	const D3DXVECTOR3 START_POSITION[gameMasterNS::PLAYER_NUM] =
	{
		D3DXVECTOR3(0, 20, -180),				//1P
		D3DXVECTOR3(130, 20, 195)				//2P
	};

	// StatusParameter
	const int	MAX_HP						= 100;									//ＨＰ最大値
	const int	FULL_POWER					= 100;									//電力ゲージ一本分の電力量
	const int	MAX_POWER					= FULL_POWER * 3;						//電力ゲージ最大値
	const int	AUTO_RECOVERY_POWER			= (int)((float)FULL_POWER/30.0f);		//自動回復電力（1秒あたりの回復量:30秒で1本分全回復）
	const int	MIN_POWER					= 0;									//電力ゲージ最低値
	const float INVINCIBLE_TIME				= 3.0f;									//無敵時間


	//CameraParameter
	const D3DXVECTOR3 CAMERA_GAZE			= D3DXVECTOR3(1.0f,1.5f,0.0f);

	// Physics
	const float MOVE_ACC					= 27.0f;								//移動加速度
	const float MOVE_ACC_WHEN_NOT_GROUND	= MOVE_ACC / 8.5f;						//空中移動加速度
	const float STOP_SPEED					= 0.5f;									//移動停止速度
	const float MAX_SPEED					= 6.0f;									//移動停止速度
	const float MAX_SLIP					= 8.0f;									//移動停止速度
	const float MAX_SLOPE					= cosf(D3DXToRadian(69.0f));			//登れる傾斜角度
	const float FALL_SPEED_MAX				= 60.0f;								//落下最高速度
	const float MOVE_FRICTION				= 1.3f;									//地面摩擦係数
	const float WALL_FRICTION				= 0.98;									//壁ずり摩擦係数
	const float GROUND_FRICTION				= 0.25;									//着地摩擦係数
	const float GRAVITY_FORCE				= 9.8f;									//重力
	const float JUMP_SPEED					= 6.0f;									//ジャンプ初速
	const float JUMP_TIME					= 0.3f;									//ジャンプ初速
	const float JUMP_CONTROL_SPEED			= 1.0f;									//ジャンプ高さコントール速度
	const float DASH_MAGNIFICATION			= 2.0f;									//ダッシュ倍率
	// Another
	const float DIFFERENCE_FIELD			= 0.1f;									// フィールド補正差分
	const float CAMERA_SPEED				= 1.0f;									// カメラの速さ
	
	//Shooting
	const float MAX_DISTANCE				= bulletNS::SPEED*bulletNS::EXIST_TIME;	//最大照準距離

	//DigitalAction
	const float SKY_HEIGH					= 105.0f;								//スカイビジョン上昇距離
	const int	COST_SHIFT					= 70;									//シフト使用電力
	const float	SHIFT_TRANSITION_TIME		= 1.0f;									//空中への遷移時間
	const float	SKY_TRANSITION_TIME			= 0.5f;									//空中への遷移時間
	const float	SKY_RETURN_TIME				= 0.5f;									//空中から戻る遷移時間


	//EnableOperation
	const int	DISABLE_OPERATION			= 0x00000000;
	const int	ENABLE_SHOT					= 0x00000001;
	const int	ENABLE_JUMP					= 0x00000002;
	const int	ENABLE_VISION				= 0x00000004;
	const int	ENABLE_CANCEL_VISION		= 0x00000008;
	const int	ENABLE_SKY_VISION			= 0x00000010;
	const int	ENABLE_CANCEL_SKY_VISION	= 0x00000020;
	const int	ENABLE_SHIFT				= 0x00000040;
	const int	ENABLE_CAMERA				= 0x00000080;
	const int	ENABLE_MOVE					= 0x00000100;
	const int	ALL_OPERATION				= 0xffffffff;
}



//===================================================================================================================================
//【仮想ステートクラス】
//===================================================================================================================================
class AbstractState:public Base
{
public:
	float			frameTime;
	float			stateTimer;
	int				type;
	int				nextType;
	bool			onTrans;
	std::string		stateName;
public:
	AbstractState() {
		frameTime		= 0.0f;
		stateTimer		= 0.0f;
		type			= playerNS::STATE::NONE;
		nextType		= playerNS::STATE::NONE;
		onTrans			= false;
		stateName		= "none";
	}
	virtual void start()					= 0;
	virtual void update(float frameTime)	= 0;
	virtual void operation()				= 0;
	virtual void physics()					= 0;
	virtual void controlCamera()			= 0;
	virtual void end()						= 0;
	virtual AbstractState* transition()		= 0;
};

//===================================================================================================================================
//【プレイヤークラス】
//===================================================================================================================================
class Player : public Object
{
private:
	//プレイヤテーブル
	PlayerTable					infomation;						//プレイヤー情報

	//ステータス
	playerNS::OperationKeyTable	keyTable;						//操作Keyテーブル
	AbstractState*				state;							//状態クラス
	int							hp;								//HP
	int							power;							//電力
	int							validOperation;					//有効な操作ビットフラグ

	//タイマー
	float						frameTime;						//フレームタイム
	float						invincibleTimer;				//無敵タイマー
	float						recoveryPowerTimer;				//自動電力回復タイマー
	float						cameraTransitionTimer;			//カメラ遷移タイマー（遷移する際に0で初期化）
	float						cameraTransitionTime;			//カメラ遷移時間（状態別で設定していく）

	//操作
	float						reverseValueXAxis;				//操作X軸
	float						reverseValueYAxis;				//操作Y軸
	bool						isExecutingMoveOperation;		//移動操作中フラグ
	bool						jumping;						//ジャンプ中フラグ
	bool						onJump;

	// 衝突
	BoundingSphere				bodyCollide;					//球コリジョン
	float						difference;						//フィールド補正差分
	bool						onGround;						//接地判定
	bool						onGroundBefore;					//直前フレームの接地判定
	D3DXVECTOR3					groundNormal;					//接地面法線

	// 物理
	float						friction;						//摩擦係数
	Ray							ray;							//レイ

	// 汎用
	LPDIRECT3DDEVICE9			device;							//Direct3Dデバイス
	Input*						input;							//入力系
	Camera*						camera;							//操作するカメラへのポインタ
	D3DXVECTOR3					cameraGaze;						//カメラ注視位置
	D3DXVECTOR3					beforeGaze;						//遷移前注視位置
	D3DXVECTOR3					nextGaze;						//遷移後注視位置
	D3DXVECTOR3					cameraGazeRelative;				//カメラ注視相対位置
	bool						nowCameraTransing;				//カメラ遷移中

	//シューティングアクション
	Ray							aimingRay;						//照準レイ（カメラからのレイ）
	Ray							shootingRay;					//狙撃レイ（プレイヤーからのレイ）
	D3DXVECTOR3					launchPosition;					//発射位置
	D3DXVECTOR3					aimingPosition;					//照準位置(カメラレイ準拠）
	D3DXVECTOR3					shootingPosition;				//衝突位置(プレイヤーレイ準拠）
	float						collideDistance;				//照射距離
	BulletManager*				bulletManager;					//バレットマネージャー
	

	//デジタルアクション
	DigitalShiftEffect*			digitalShiftEffect;				//デジタルシフトエフェクト
	Line						shiftLine;						//デジタルシフトライン
	Ray							shiftRay;						//デジタルシフトレイ
	float						shiftDistance;
	D3DXVECTOR3					shiftPosition;

	//再生パラメータ
	PLAY_PARAMETERS shiftStartSE;
	PLAY_PARAMETERS shiftFinishSE;
	PLAY_PARAMETERS visionSE;
	PLAY_PARAMETERS visionStartSE;
	PLAY_PARAMETERS visionFinishSE;
	PLAY_PARAMETERS skyVisionStartSE;
	PLAY_PARAMETERS skyVisionFinishSE;

	//debug
	float dot;

public:
	Player();
	~Player();

	// 基本処理
	virtual void initialize(PlayerTable info);
	virtual void update(float frameTime);
	void otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//状態遷移
	void transState(int next);									//状態遷移

	// 衝突
	void grounding();											// 接地処理
	void wallScratch();											// 壁ずり処理

	bool grounding(LPD3DXMESH mesh, D3DXMATRIX matrix);
	
	//めり込み補正
	//Ray ray：補正方向レイ（取得した法線方向に補正される）
	//float distance：補正距離
	//LPD3DXMESH mesh：衝突対象メッシュ
	//D3DXMATRIX matrix：衝突対象行列
	bool insetCorrection(Ray ray, float distance, LPD3DXMESH mesh, D3DXMATRIX matrix);
	bool insetCorrection(int axisID, float distance, LPD3DXMESH mesh, D3DXMATRIX matrix);


	// 物理
	void physicalBehavior();									//物理挙動
	void updatePhysics(float frameTime);						//物理の更新

	//操作
	void moveOperation();										//移動操作
	void jumpOperation();										//ジャンプ操作

	//カメラ
	void controlCamera(float frameTime);						//カメラ操作
	void controlSkyCamera();									//スカイカメラ操作
	void transitionCamera();									//カメラ遷移
	void startTransitionCamera(float time, D3DXVECTOR3 before, D3DXVECTOR3 next);	//カメラ遷移を開始(位置指定)
	void returnTransitionCamera(float time);

	//自動処理
	void recoveryPower();										//電力の自動回復
	void respawn();
	// アクション
	void move(D3DXVECTOR2 moveDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ);//移動
	void jump();												//ジャンプ
	float dash();												//ダッシュ
	bool shot();												//弾を打つ

	//デジタルアクション
	bool digitalShift();										//デジタルシフト
	bool skyVision();											//スカイビジョン
	bool vision();												//ビジョン
	bool executionDigitalShift();								//[実行]デジタルシフト
	bool executionSkyVision();									//[実行]スカイビジョン
	bool executionVision();										//[実行]ビジョン
	bool cancelSkyVision();										//[キャンセル]スカイビジョン
	bool cancelVision();										//[キャンセル]ビジョン
	void collideShiftRay(LPD3DXMESH mesh, D3DXMATRIX matrix);	//シフトレイとの衝突
	void collideShiftRay(D3DXVECTOR3 position);					//シフトレイとの衝突
	bool collideShiftRay(Cylinder target);						//シフトレイとの衝突
	void stopSelectLight();
	void playSelectLight();
	void shownSelectLight(bool shown);

	//シューティング
	void updateAiming(LPD3DXMESH mesh, D3DXMATRIX matrix);		//照準方向を更新する
	void updatePostureByAiming();								//狙撃方向へ姿勢を向ける
	void updateShooting(LPD3DXMESH mesh, D3DXMATRIX matrix);										//狙撃位置の更新

	// その他
	virtual void outputGUI() override;							// ImGUI
	void reset();												// リセット

	//setter
	void	setCamera(Camera* _camera);							//操作対象カメラのセット
	void	addpower(int add);									//電力加算
	void	pullpower(int pull);								//電力減算
	void	setInfomation(PlayerTable info);					//プレイヤー情報のセット
	void	damage(int _damage);								//ダメージ処理
	void	setValidOperation(int value);						//有効操作の設定
	void	enableOperation(int value);							//操作を有効にする
	void	disableOperation(int value);						//操作を無効にする
	//void resetOnGround() { onGround = false;  jumping = true; }	//

	//getter
	int getHp();												// HPの取得
	int getPower();												// 電力の取得
	int getState();
	bool whetherValidOperation(int operation);					//有効操作の確認
	bool canShot();
	bool canJump();
	bool canDoVision();
	bool canDoSkyVision();
	bool canShift();
	BoundingSphere* getBodyCollide();							//球コリジョンの取得
	PlayerTable*	getInfomation();							//プレイヤー情報取得
	D3DXVECTOR3*	getCameraGaze();							//カメラ注視ポジション
	D3DXVECTOR3*	getAiming();								//照準ポイントの取得
	Bullet*			getBullet(int i);							//発射中の弾へのポインタ
	int				getShootingNum();							//発射中の弾数
	LPD3DXMESH		getMesh();									//レイ衝突用メッシュ
	bool			getWhetherExecutingMoveOpe();				//移動操作中か取得
	BulletManager*  getBulletManager();							//バレットマネージャを取得
	bool			getOnGround();								//接地しているか取得
	D3DXVECTOR3*	getGroundNormal();							//接地面法線を取得
};
