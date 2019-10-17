//-----------------------------------------------------------------------------
// エネミー基底クラス [Enemy.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshObject.h"
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

	struct EnemyData
	{
		int id;							// 識別番号(0..*)
		int enemyType;					// エネミー種別
		int enemyState;					// エネミーステート
		int initialPosition;			// 初期座標
		bool isDead;					// 死亡したかどうか
		void initialize() { ZeroMemory(this, sizeof(EnemyData)); }
	};


	const D3DXVECTOR3 START_POSITION = D3DXVECTOR3(-35, 10, 0);
	//const D3DXVECTOR3 START_POSITION = D3DXVECTOR3(0, 0, 0);

	const float MOVE_SPEED_MAX = 5.67f;		// 秒速5.67m
	//const float MOVE_SPEED_MAX = 50.67f;		// 秒速5.67m
	const float MOVE_FRICTION = 0.99f;
	const float MOVE_ACC = MOVE_SPEED_MAX / MOVE_FRICTION;

	const float GRAVITY_FORCE = 9.8f * 0.5;	// 重力
	const float DIFFERENCE_FIELD = 0.2f;	// フィールド補正差分
	const float CAMERA_SPEED = 1.0f;		// カメラの速さ
}


//=============================================================================
//クラス定義
//=============================================================================
class Enemy: public Object
{
private:
	enemyNS::EnemyData* enemyData;		// エネミーデータ
	static int numOfEnemy;				// エネミーの総数

	BoundingSphere sphereCollider;		// バウンディングスフィア
	 

	// 物理挙動
	LPD3DXMESH	attractorMesh;			// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;		// 重力（引力）発生オブジェクトマトリックス
	float difference;					// フィールド補正差分
	bool onGround;						// 接地判定
	bool onGroundBefore;				// 直前フレームの接地判定
	float friction;

	//// 経路探索
	//std::vector<D3DXVECTOR3> routeStack;// 経路スタック


#ifdef _DEBUG
	// デバッグ用
	LPDIRECT3DDEVICE9 device;			// Direct3Dデバイス
	Camera*	camera;						// 操作するカメラへのポインタ
	Input* input;						// 入力処理
	enemyNS::OperationKeyTable keyTable;// 操作キーテーブル
	float reverseValueXAxis;			// 操作X軸
	float reverseValueYAxis;			// 操作Y軸
#endif

public:
	Enemy();
	~Enemy();
	virtual void update(float frameTime);
	//virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	void groundingWork();
	void updatePhysicalBehavior();
	void updatePhysics(float frameTime);
	void configurationGravityWithRay(D3DXVECTOR3* attractorPosition, LPD3DXMESH attractorMesh, D3DXMATRIX* attractorMatrix);
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
