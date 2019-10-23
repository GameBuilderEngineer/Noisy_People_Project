//-----------------------------------------------------------------------------
// エネミー基底クラス [Enemy.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshRenderer.h"
#include "BoundingSphere.h"
#include "Input.h"
#include "Camera.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace enemyNS
{
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


	// エネミーの種類
	enum ENEMY_TYPE
	{
		WOLF,			// オオカミ
		TIGER,			// トラ
		BEAR,			// クマ
		TYPE_MAX		
	};

	// ステートの種類
	enum ENEMY_STATE
	{
		CHASE,			// 追跡ステート
		PATROL,			// 警戒ステート
		REST,			// 休憩ステート
		DIE,			// 死亡ステート
		DEAD,			// 撃退済み判定
		STATE_MAX
	};

	// 定数定義
	const D3DXVECTOR3 START_POSITION = D3DXVECTOR3(-35, 10, 0);
	//const D3DXVECTOR3 START_POSITION = D3DXVECTOR3(0, 0, 0);
	const float MOVE_SPEED_MAX = 5.67f;		// 秒速5.67m
	//const float MOVE_SPEED_MAX = 50.67f;		// 秒速5.67m
	const float MOVE_FRICTION = 0.99f;
	const float MOVE_ACC = MOVE_SPEED_MAX / MOVE_FRICTION;
	const float GRAVITY_FORCE = 9.8f * 0.5;	// 重力
	const float DIFFERENCE_FIELD = 0.2f;	// フィールド補正差分
	const float CAMERA_SPEED = 1.0f;		// カメラの速さ


	// エネミーの最大HPテーブル
	const int ENEMY_HP_MAX[TYPE_MAX] =
	{
		100,		// WOLF
		150,		// TIGER
		200,		// BEAR
	};


	// EnemyInitialSettingDataクラスはエネミー初期ステータスを保持する
	// エネミー配置ツールとのデータ交換に使用する
	typedef struct EnemyInitialSettingData
	{
		int id;							// 識別番号(0..*)
		int type;						// エネミー種別
		int defaultState;				// 初期ステート	
		D3DXVECTOR3 defaultPosition;	// 初期座標
		D3DXVECTOR3 defaultDirection;	// 初期正面方向
	} ENEMYSET;
 

	// EnemyDataクラスはエネミー固有のステータスを保持する
	// EnemyクラスオブジェクトはEnemyDataのポインタを介してステータスを参照する
	struct EnemyData
	{
		int id;							// 識別番号(0..*)
		int type;						// エネミー種別
		int state;						// ステート
		int defaultState;				// 初期ステート	
		D3DXVECTOR3 position;			// 座標
		D3DXVECTOR3 defaultPosition;	// 初期座標
		D3DXVECTOR3 direction;			// 正面方向
		D3DXVECTOR3 defaultDirection;	// 初期正面方向
		int hp;							// HP
		bool isAlive;					// 生存フラグ
		static int numOfEnemyData;		// エネミーデータの数

		EnemyData() { numOfEnemyData++; }
		~EnemyData() { numOfEnemyData--; }
		void zeroClear() { ZeroMemory(this, sizeof(EnemyData)); }
		void setUp()
		{
			state = defaultState;
			position = defaultPosition;
			direction = defaultDirection;
			hp = type >= 0 && type < TYPE_MAX ? ENEMY_HP_MAX[type] : 0;
			isAlive = true;
		}
		static int getNumOfEnemyData() { return numOfEnemyData; }
	};
}


//=============================================================================
//クラス定義
//=============================================================================
class Enemy: public Object
{
private:
	enemyNS::EnemyData* enemyData;		// エネミーデータ
	static int numOfEnemy;				// エネミーの総数

	// 物理挙動
	LPD3DXMESH	attractorMesh;			// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;		// 重力（引力）発生オブジェクトマトリックス
	float difference;					// フィールド補正差分
	bool onGround;						// 接地判定
	bool onGroundBefore;				// 直前フレームの接地判定
	float friction;

#ifdef _DEBUG
#endif
	// デバッグ用
	LPDIRECT3DDEVICE9 device;			// Direct3Dデバイス
	Camera*	camera;						// 操作するカメラへのポインタ
	Input* input;						// 入力処理
	enemyNS::OperationKeyTable keyTable;// 操作キーテーブル
	float reverseValueXAxis;			// 操作X軸
	float reverseValueYAxis;			// 操作Y軸

	void previousWork();				// 事前処理
	virtual void chase() = 0;			//「追跡」ステート
	virtual void patrol() = 0;			//「警戒」ステート
	virtual void rest() = 0;			//「休憩」ステート
	virtual void die() = 0;				//「死亡」ステート


public:
	Enemy(StaticMesh* _staticMesh, enemyNS::EnemyData* _enemyData);
	~Enemy();
	virtual void update(float frameTime);
	BoundingSphere sphereCollider;		// バウンディングスフィア

	void grounding();
	void physicalBehavior();
	void updatePhysics(float frameTime);
	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void outputGUI();

	// Debug
	void setDebugEnvironment();								// デバッグ環境を設定
	void controlCamera(float frameTime);					// カメラ操作
	void move(D3DXVECTOR2 moveDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ);	//移動
	void moveOperation();									// 移動操作
	bool isGoingMoveOperation;

	// Getter
	static int getNumOfEnemy();								// エネミーの数を取得
	enemyNS::EnemyData* getEnemyData();						// エネミーデータを取得

	//setter
	void setDataToEnemy(enemyNS::EnemyData* _enemyData);	// エネミーデータをエネミーに設定
	void setCamera(Camera* _camera);						// 操作対象カメラのセット

};
