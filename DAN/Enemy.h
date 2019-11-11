//-----------------------------------------------------------------------------
// エネミー基底クラス [Enemy.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "NavigationMesh.h"
#include "StateMachine.h"
#include "Sound.h"
#include "SoundBase.h"


//=============================================================================
// ビルドスイッチ
//=============================================================================
#if 0	// バウンディングスフィアを描画
#define RENDER_SPHERE_COLLIDER
#endif
#if 0	// 6軸を描画
#define RENDER_SIX_AXIS
#endif
#if 1	// センサーを描画
#define RENDER_SENSOR
#endif




//=============================================================================
// 名前空間
//=============================================================================
#pragma region namespace
namespace enemyNS
{
	// エネミーの種類
	enum ENEMY_TYPE
	{
		WOLF,		// code name "WOLF"
		TIGER,		// code name "TIGER"
		BEAR,		// code name "BEAR"
		TYPE_MAX,
	};

	// エネミーの最大HPテーブル
	const int ENEMY_HP_MAX[TYPE_MAX] =
	{
		100,		// WOLF
		150,		// TIGER
		200,		// BEAR
	};

	// エネミーのセンサー更新頻度テーブル（x秒に1度更新）
	const float SENSOR_UPDATE_INTERVAL[TYPE_MAX] =
	{
		0.3f,		// WOLF
		0.2f,		// TIGER
		0.1f,		// BEAR
	};

	// エネミーの視力距離テーブル
	const float VISIBLE_DISTANCE[TYPE_MAX] =
	{
		40.0f,		// WOLF
		40.0f,		// TIGER
		40.0f,		// BEAR
	};

	// エネミーの水平視野角テーブル（Z軸 - 水平片側）
	const float HORIZONTAL_HALF_VIEWING_ANGLE[TYPE_MAX] =
	{
		0.55f,		// WOLF
		0.55f,		// TIGER
		0.55f,		// BEAR
	};

	// エネミーの垂直視野角テーブル（Z軸 - 垂直片側）
	const float VERTICAL_HALF_VIEWING_ANGLE[TYPE_MAX] =
	{
		0.5f,		// WOLF
		0.5f,		// TIGER
		0.5f,		// BEAR
	};

	// エネミーの聴覚距離テーブル
	const float NOTICEABLE_DISTANCE_PLAYER[TYPE_MAX] =
	{
		3.5f,		// WOLF
		3.5f,		// TIGER
		3.5f,		// BEAR
	};
	const float SHOT_SOUND_SCALE = 4.5f;// ショット音距離倍率

	// エネミーの攻撃力テーブル
	const int ATTACK_DAMAGE[TYPE_MAX] =
	{
		5,			// WOLF
		10,			// TIGER
		15,			// BEAR
	};

	// エネミーの攻撃移動速度テーブル
	const float ATTACK_SPEED[TYPE_MAX] =
	{
		10.0f,		// WOLF
		10.0f,		// TIGER
		15.0f,		// BEAR
	};

	// エネミーの攻撃移動時間テーブル
	const float ATTACK_TIME[TYPE_MAX] =
	{
		0.15f,		// WOLF
		0.15f,		// TIGER
		0.15f,		// BEAR
	};

	// エネミーのプレイヤー記憶時間テーブル
	const float PLAYER_MEMORIZED_TIME[TYPE_MAX] =
	{
		8.0f,		// WOLF
		8.0f,		// TIGER
		8.0f,		// BEAR
	};

	// エネミーの移動加速度テーブル
	const float MOVE_ACC[TYPE_MAX] =
	{
		20.0f,		// WOLF
		20.0f,		// TIGER
		20.0f,		// BEAR
	};

	// Physics
	const float AIR_MOVE_ACC_MAGNIFICATION = 0.12f;				// 空中移動加速度倍率
	const float STOP_SPEED = 0.5f;								// 移動停止速度
	const float FALL_SPEED_MAX = 60.0f;							// 落下最高速度
	const float MOVE_FRICTION = 0.90f;							// 地面摩擦係数
	const float WALL_FRICTION = 0.98f;							// 壁ずり摩擦係数
	const float GROUND_FRICTION = 0.25f;						// 着地摩擦係数
	const float ATTACKED_FRICTION = 0.2f;						// 攻撃後摩擦係数
	const float GRAVITY_FORCE = 9.8f * 2;						// 重力
	const float JUMP_SPEED = 6.0f;								// ジャンプ初速
	const float JUMP_CONTROL_SPEED = 1.0f;						// ジャンプ高さコントール速度
	const float DASH_MAGNIFICATION = 2.0f;						// ダッシュ倍率
	const float DIFFERENCE_FIELD = 0.05f;						// フィールド補正差分
	const float CAMERA_SPEED = 1.0f;							// カメラの速さ


	//-----------------------------------------------------------------
	// EnemyInitialSettingDataクラスはエネミー初期ステータスを保持する
	// 新規エネミー作成やエネミー配置ツールとのデータ交換に使用する
	//-----------------------------------------------------------------
	typedef struct EnemyInitialSettingData
	{
		int enemyID;					// 識別番号(0..*)
		int type;						// エネミー種別
		int defaultState;				// 初期ステート	
		D3DXVECTOR3 defaultPosition;	// 初期座標
		D3DXVECTOR3 defaultDirection;	// 初期正面方向
	} ENEMYSET;
 

	//--------------------------------------------------------------------------
	// EnemyDataクラスはエネミー固有のステータスを保持する
	// EnemyクラスオブジェクトはEnemyDataのポインタを介してステータスを参照する
	//--------------------------------------------------------------------------
	struct EnemyData
	{
		int enemyID;					// 識別番号(0..*)
		int type;						// エネミー種別
		int state;						// ステート
		int defaultState;				// 初期ステート	
		D3DXVECTOR3 position;			// 座標
		D3DXVECTOR3 defaultPosition;	// 初期座標
		D3DXVECTOR3 direction;			// 正面方向
		D3DXVECTOR3 defaultDirection;	// 初期正面方向
		int hp;							// HP
		float deadTime;					// 撃退時刻
		bool isAlive;					// 生存フラグ

		void zeroClear() { ZeroMemory(this, sizeof(EnemyData)); }
		void setUp()
		{
			state = defaultState;
			position = defaultPosition;
			direction = defaultDirection;
			hp = type >= 0 && type < TYPE_MAX ? ENEMY_HP_MAX[type] : 0;
			deadTime = 10000.0;
			isAlive = true;
		}
	};

#if _DEBUG
	struct OperationKeyTable
	{
		BYTE front;
		BYTE back;
		BYTE right;
		BYTE left;
		BYTE jump;
		BYTE reverseCameraX;
		BYTE reverseCameraY;
	};

	const OperationKeyTable KEY_TABLE_1P =
	{
		'W',		// FRONT
		'S',		// BACK
		'D',		// RIGHT
		'A',		// LEFT
		VK_SPACE,	// JUMP
		VK_F11,		// ReverseCameraAxisX
		VK_F12,		// ReverseCameraAxisY
	};
#endif //_DEBUG
}
#pragma endregion


//=============================================================================
// クラス定義
//=============================================================================
#pragma region class
// エネミー基底クラス
class Enemy: public Object
{
protected:
	enemyNS::EnemyData* enemyData;		// エネミーデータ
	static int numOfEnemy;				// エネミーの総数
	StateMachine stateMachine;			// ステートマシン
	Player* player;						// プレイヤー

	// センサー
	float sensorTime;					// センサー更新時間カウンタ
	bool canUseSensor;					// センサー実行フラグ
	
	// 環境認識
	bool isNoticedPlayer[gameMasterNS::PLAYER_NUM];

	// 記憶
	float noticedTimePlayer[gameMasterNS::PLAYER_NUM];

	// 攻撃
	bool onAttack;
	float attackTime;
	int attackTargetPlayer;
	D3DXVECTOR3 vecAttack;

	// ナビゲーションメッシュ
	NavigationMesh* naviMesh;			// ナビメッシュ
	LinkedList<meshDataNS::Index2>* edgeList;// エッジリスト
	DWORD naviFaceIndex;				// ●ナビメッシュいんでっくそ
	bool shouldSearchPath;				// パス検索フラグ	

	// 移動
	bool onMove;						// 移動フラグ
	D3DXVECTOR3* movingTarget;			// 移動ターゲット
	D3DXVECTOR3 destination;			// 目的地

	bool onJump;						// ジャンプフラグ
	bool jumping;						// ジャンプ中フラグ
	D3DXVECTOR3	centralPosition;		// 中心座標
	D3DXMATRIX	centralMatrixWorld;		// 中心座標ワールドマトリクス

	// コリジョン
	Ray	ray;							// レイ
	BoundingSphere sphereCollider;		// バウンディングスフィア
	float difference;					// フィールド補正差分
	bool onGround;						// 接地判定
	bool onGroundBefore;				// 直前フレームの接地判定
	D3DXVECTOR3	groundNormal;			// 接地面法線
	bool isHitPlayer;					// プレイヤーと接触している


	// 物理挙動
	LPD3DXMESH	attractorMesh;			// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;		// 重力（引力）発生オブジェクトマトリックス
	float friction;						// 摩擦係数

	// サウンド
	PLAY_PARAMETERS *playParameters;
	void footsteps(D3DXVECTOR3 playerPos, int playerID);

	// 純粋仮想関数
	virtual void chase() = 0;			//「追跡」ステート
	virtual void patrol() = 0;			//「警戒」ステート
	virtual void rest() = 0;			//「休憩」ステート
	virtual void die() = 0;				//「死亡」ステート

public:
	Enemy(StaticMesh* _staticMesh, enemyNS::EnemyData* _enemyData);
	~Enemy();
	// 更新処理
	virtual void update(float frameTime);
	// 事前処理
	virtual void preprocess(float frameTime);
	// 視覚センサー
	bool eyeSensor(int playerType);
	// 聴覚センサー
	bool earSensor(int playerType);
	// 攻撃
	void attack(int playerType, float frameTime);
	// 記憶の更新
	void updateMemory(float frameTime);
	// ステアリング
	void steering();
	// 接地処理
	void grounding();
	// 壁ずり
	void wallScratch();
	// 物理挙動
	void physicalBehavior();
	// 物理の更新
	void updatePhysics(float frameTime);
	// めり込み補正
	void insetCorrection();
	// 中心座標系の更新
	void updateCentralCood();
	// 重力発生メッシュ（接地メッシュ）の設定
	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void setPlayer(Player* _player);
	// エネミーのオブジェクトの数を初期化
	static void resetNumOfEnemy();

	// Getter
	int getEnemyID();
	static int getNumOfEnemy();
	enemyNS::EnemyData* getEnemyData();
	BoundingSphere* getSphereCollider();
	D3DXVECTOR3* getCentralPosition();
	D3DXMATRIX* getCentralMatrixWorld();

	// Setter
	void setMove(bool setting);
	void setMovingTarget(D3DXVECTOR3* _target);
	void setIsHitPlayer(bool setting);

#ifdef _DEBUG
	LPDIRECT3DDEVICE9 device;			// Direct3Dデバイス
	Camera*	camera;						// 操作するカメラへのポインタ
	Input* input;						// 入力処理
	enemyNS::OperationKeyTable keyTable;// 操作キーテーブル
	float reverseValueXAxis;			// 操作X軸
	float reverseValueYAxis;			// 操作Y軸
	static int debugEnemyID;			// デバッグするエネミーのID

	// デバッグ環境を設定
	void setDebugEnvironment();
	// 操作対象カメラのセット
	void setCamera(Camera* _camera) { camera = _camera; }
	// カメラ操作
	void controlCamera(float frameTime);
	// 移動操作
	void moveOperation();
	// 移動
	void move(D3DXVECTOR2 moveDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ);
	// デバッグセンサー
	Ray eyeAngleRay[4];					// 視覚レイ表示用
	Ray gazePlayer;						// 
	BoundingSphere hearingSphere[2];	// 聴覚距離表示用
	void debugSensor();					// デバッグ関数
	// ImGUIへの出力
	void outputGUI();
#endif//_DEBUG
};
#pragma endregion
