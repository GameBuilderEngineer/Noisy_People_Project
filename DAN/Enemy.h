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
#include "Tree.h"
#include "EnemyChaseMark.h"
#include "PartsAnimationManager.h"


//=============================================================================
// ビルドスイッチ
//=============================================================================
#if 1	// センサーを描画
#define RENDER_SENSOR
#endif

//=============================================================================
// 名前空間
//=============================================================================
#pragma region [Name Space]
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

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////ENEMY_TYPEごとの定数 //////////////////////////
	///////////////////////////////////////////////////////////////////////////
	// エネミーの最大HPテーブル
	const int ENEMY_HP_MAX[TYPE_MAX] =
	{
		60,			// WOLF
		100,		// TIGER
		450,		// BEAR
	};

	// エネミーのプレイヤー記憶時間テーブル
	const float PLAYER_MEMORIZED_TIME[TYPE_MAX] =
	{
		4.0f,		// WOLF
		8.0f,		// TIGER
		10.0f,		// BEAR
	};

	// エネミーの移動加速度テーブル
	const float MOVE_ACC[TYPE_MAX] =
	{
		15.0f,		// WOLF
		15.0f,		// TIGER
		12.0f,		// BEAR
	};

	//-------------
	//センサー関係
	//-------------
	// エネミーのセンサー更新頻度テーブル（x秒に1度更新）
	const float SENSOR_UPDATE_INTERVAL[TYPE_MAX] =
	{
		0.3f,		// WOLF
		0.3f,		// TIGER
		0.3f,		// BEAR
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
		0.8f,		// BEAR
	};

	// エネミーの聴覚距離テーブル
	const float NOTICEABLE_DISTANCE_PLAYER[TYPE_MAX] =
	{
		3.5f,		// WOLF
		5.5f,		// TIGER
		18.5f,		// BEAR
	};
	const float SHOT_SOUND_SCALE = 4.5f;// ショット音距離倍率

	//----------
	// 攻撃関係
	//----------
	// エネミーの攻撃力テーブル
	const int ATTACK_DAMAGE[TYPE_MAX] =
	{
		10,			// WOLF
		20,			// TIGER
		30,			// BEAR
	};

	// エネミーの攻撃移動速度テーブル
	const float ATTACK_SPEED[TYPE_MAX] =
	{
		15.0f,		// WOLF
		15.0f,		// TIGER
		15.0f,		// BEAR
	};
	const float MULTIPLICATION_TO_MAKE_ATTACK_ACC = 6.5f;//加速度化する係数

	// エネミーの攻撃移動時間テーブル
	const float ATTACK_TIME[TYPE_MAX] =
	{
		0.7f,		// WOLF
		0.7f,		// TIGER
		0.7f,		// BEAR
	};

	// エネミーの攻撃間隔テーブル
	const float ATTACK_INTERVAL[TYPE_MAX] =
	{
		8.0f,		// WOLF
		8.0f,		// TIGER
		10.0f,		// BEAR
	};

	//--------
	// その他
	//--------
	// 追跡マークの大きさ
	const D3DXVECTOR2 MARK_SCALE[TYPE_MAX] =
	{
		D3DXVECTOR2(2.0f, 2.0f),	// WOLF
		D3DXVECTOR2(3.5f, 3.5f),	// TIGER
		D3DXVECTOR2(3.0f, 3.0f),	// BEAR
	};

	// アイテムを出す確率の分母（1/x）
	const int ITEM_DROP_PROBABILITY_DENOMINATOR[TYPE_MAX] =
	{
		7,			// WOLF
		2,			// TIGER
		1			// BEAR
	};

	// エフェクトサイズ
	const float DEATH_EFFECT_SCALE[TYPE_MAX] =
	{
		0.85f,		// WOLF
		1.5f,		// TIGER
		5.0f,		// BEAR
	};

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////エネミー共通定数 ///////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	// Physics Constant
	const float AIR_MOVE_ACC_MAGNIFICATION = 0.12f;		// 空中移動加速度倍率
	const float STOP_SPEED = 0.5f;						// 移動停止速度
	const float FALL_SPEED_MAX = 60.0f;					// 落下最高速度
	const float MOVE_FRICTION = 0.90f;					// 地面摩擦係数
	const float WALL_FRICTION = 0.98f;					// 壁ずり摩擦係数
	const float GROUND_FRICTION = 0.25f;				// 着地摩擦係数
	const float ATTACKED_FRICTION = 0.2f;				// 攻撃後摩擦係数
	const float GRAVITY_FORCE = 9.8f * 2;				// 重力

	// AI Constant
	const float ARRIVAL_DISTANCE = 0.5f;				// 到着距離
	const float MOVE_LIMIT_TIME = 10.0f;				// 移動リミット時間
	const float PATH_SEARCH_INTERVAL_WHEN_CHASE = 0.3f;	// 追跡ステート時のパス検索間隔
	const float TIME_BEFORE_TREE_COLLISION = 10.0f;		// ツリー攻撃ステートでツリーに触れないままでいられる時間

	// Other Constant
	const float DIFFERENCE_FIELD = 0.05f;				// フィールド補正差分
	const float JUMP_SPEED = 6.0f;						// ジャンプ初速
	const float JUMP_CONTROL_SPEED = 1.0f;				// ジャンプ高さコントール速度
	const float DASH_MAGNIFICATION = 2.0f;				// ダッシュ倍率
	const float CAMERA_SPEED = 1.0f;					// カメラの速さ
	const int PATOROL_ROUTE_MAX = 8;					// 警戒ステート時の巡回座標最大数
	const float AUTO_DESTRUCTION_HEIGHT = -100.0f;		// 自動破棄される高さ
	const float MARK_FLOATING_HEIGHT = 0.1f;			// 追跡マークの高さ
	const float DIE_STATE_TIME = 4.0f;					// 死亡ステートの時間
	const float DIE_STATE_RENDERING_TIME = 2.8f;		// 死亡ステートのうち描画が続く時間

	// 3DSound
	enum SE_3D
	{
		FOOT_STEPS_SE,	// 足音
		ATTACK_SE,		// 攻撃音
		DIE_SE,			// 死亡音
		SE_3D_MAX
	};

	///////////////////////////////////////////////////////////////////////////
	/////////////////////////////// クラス定義 ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------
	// EnemyInitialSettingDataクラスはエネミー初期ステータスを保持する
	// 新規エネミー作成やエネミー配置ツールとのデータ交換に使用する
	//-----------------------------------------------------------------
	typedef struct EnemyInitialSettingData
	{
		int enemyID;									// 識別番号(0..*)
		int type;										// エネミー種別
		int defaultState;								// 初期ステート	
		D3DXVECTOR3 defaultPosition;					// 初期座標
		D3DXVECTOR3 defaultDirection;					// 初期正面方向
		int numRoute;									// 警戒ステート時の巡回座標の数
		D3DXVECTOR3 patrolRoute[PATOROL_ROUTE_MAX];		// 警戒ステート時の巡回座標
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
		int numRoute;					// 警戒ステート時の巡回座標の数
		D3DXVECTOR3* patrolRoute;		// 警戒ステート時の巡回座標
		Tree* targetTree;				// 攻撃対象ツリー
		int hp;							// HP
		float deadTime;					// 撃退時刻
		bool isAlive;					// 生存フラグ
		bool isGeneratedBySpawnEvent;	// イベントによる生成フラグ(データ作成後オブジェクト作成前にtrueに設定)
		bool isObjectExists;			// オブジェクト存在フラグ（オブジェクト作成破棄に合わせて設定）				
		bool wasAutoDetroy;				// 自動削除フラグ

		//~EnemyData() { SAFE_DELETE_ARRAY(patrolRoute); }
		void zeroClear() { ZeroMemory(this, sizeof(EnemyData)); }
		void setUp()
		{
			state = defaultState;
			position = defaultPosition;
			direction = defaultDirection;
			hp = type >= 0 && type < TYPE_MAX ? ENEMY_HP_MAX[type] : 0;
			deadTime = 10000.0;
			isGeneratedBySpawnEvent = false;
			isAlive = true;
		}
	};

	//----------------------------------------------
	// コンストラクタ引数に渡す初期化用データセット
	//----------------------------------------------
	struct ConstructionPackage
	{
		StaticMesh* staticMesh;
		EnemyData* enemyData;
		GameMaster* gameMaster;
		Player* player;
		LPD3DXMESH	attractorMesh;
		D3DXMATRIX*	attractorMatrix;
		//EnemyChaseMark* markRenderer;
		//StaticMeshRenderer* tigerBulletRender;
	};

	//----------------------
	// 死亡エフェクトクラス
	//----------------------
	class DeathEffect :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3 * syncPosition;
		DeathEffect(D3DXVECTOR3* sync, int _effectNo)
		{
			syncPosition = sync;
			effectNo = _effectNo;
		}
		virtual void update()
		{
			position = *syncPosition;
			Instance::update();
		};
	};

	//----------------------------------------------------------------
	// EnemyPartsクラスはエネミーパーツアニメの各パーツである
	// エネミーである自分自身のポインタとメッシュ情報のポインタを持つ
	//----------------------------------------------------------------
	class EnemyParts: public Object
	{
	private:
		Enemy* enemy;					// エネミー
		StaticMeshRenderer* renderer;	// 描画オブジェクト
		float cntDestroyTime = 0.0f;	// パーツが破壊される時間をカウント

	public:
		int durability;					// 耐久度
		D3DXVECTOR3 jointPosition;		// 接合部座標（= オフセット * パーツのワールドマトリクス）
		bool wasEffectPlayed;			// エフェクト再生されたか

		Enemy* getEnemy() { return enemy; }
		void setEnemy(Enemy* setting) { enemy = setting; }
		StaticMeshRenderer* getRenderer() { return renderer; }
		void setRenderer(StaticMeshRenderer* setting) { renderer = setting; }

		// 耐久度へのダメージ処理
		void damage(int value)
		{
			durability -= value;

			if (durability < 0)
			{
				durability = 0;
			}
		}

		// 耐久度チェック
		void checkDurability(float frameTime)
		{
			if (durability != 0) return;

			if (wasEffectPlayed == false)
			{
				// エフェクト
				DeathEffect* effect = new DeathEffect(&jointPosition, effekseerNS::ENEMY_DEATH);
				effect->scale *= 5.0f;//エフェクトサイズ
				effekseerNS::play(0, effect);
				wasEffectPlayed = true;
			}

			cntDestroyTime += frameTime;
			if (cntDestroyTime > 2.0f)
			{
				// 描画と衝突判定から取り除く（インスタンス破棄自体はエネミーのデストラクタ）
				renderer->unRegisterObjectByID(this->id);
				treeCell.remove();
			}
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
// Enemyクラス定義
//=============================================================================
#pragma region [Class]
class Enemy: public Object
{
protected:
#ifdef _DEBUG
public:
#endif
	enemyNS::EnemyData* enemyData;					// エネミーデータ
	static int numOfEnemy;							// エネミーの総数
	StaticMesh* staticMesh;							// メッシュ情報
	GameMaster* gameMaster;							// ゲーム管理オブジェクト
	Player* player;									// プレイヤー
	LPD3DXMESH	attractorMesh;						// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;					// 重力（引力）発生オブジェクトマトリックス
	StateMachine stateMachine;						// ステートマシン
	PartsAnimationManager* animationManager;		// アニメーションマネージャ

	// ブラックボード
	bool isNoticingPlayer[gameMasterNS::PLAYER_NUM];// プレイヤー認識フラグ
	float noticedTimePlayer[gameMasterNS::PLAYER_NUM];// プレイヤー認識時間
	int chasingPlayer;								// 追跡プレイヤー
	D3DXVECTOR3* movingTarget;						// 移動ターゲット
	Object* attackTarget;							// 攻撃対象（プレイヤー,ツリー）
	D3DXVECTOR3 attentionDirection;					// 注目方向
	bool isPayingNewAttention;						// 新規注目フラグ
	int nextIndexOfRoute;							// 警戒ステート時の次の巡回座標（リングバッファ添え字）
	float tacticsTime;								// 戦術時間（AIの先頭行動分岐に汎用的に使用）
	int tacticsPersonality;							// 戦術個性

	// センサー
	bool canSense;									// センサー実行可能フラグ
	float cntSensorInterval;						// センサー再実行間隔カウンタ
	bool isSensingPlayer[gameMasterNS::PLAYER_NUM];	// プレイヤー感知フラグ
	float distanceToPlayer[gameMasterNS::PLAYER_NUM];// プレイヤーとの距離

	// 経路探索
	bool canSearch;									// 経路探索実行可能フラグ
	float cntPathSearchInterval;					// 経路探索再実行間隔カウンタ
	NavigationMesh* naviMesh;						// ナビメッシュ
	LinkedList<meshDataNS::Index2>* edgeList;		// エッジリスト
	DWORD naviFaceIndex;							// パス検索時のナビメッシュ設置面インデックス

	// 攻撃
	bool canAttack;									// 攻撃可能フラグ
	float cntAttackInterval;						// 攻撃再実行間隔カウンタ
	bool isAttacking;								// 攻撃中フラグ
	float attackTime;								// 攻撃中時間（秒）
	D3DXVECTOR3 attackDirection;					// 攻撃方向ベクトル
	bool wasHittingTree;							// ツリーを攻撃した			
	float treeAttackTime;							// ツリー攻撃時間
	bool canDamageTree;								// ツリーにダメージが入るか
	float cntBeforeTreeCollision;					// ツリー攻撃ステートでツリーに接触するまでの時間
	int cntDestroyParts;							// 破壊されたパーツの数

	// 移動
	D3DXVECTOR3 destination;						// 目的地
	D3DXVECTOR3 moveDirection;						// 移動ベクトル
	D3DXQUATERNION	moveMotion;						// 移動モーション
	bool isArraved;									// 到着フラグ
	float movingTime;								// 計測移動時間
	bool isDestinationLost;							// 目的地を見失った
	bool onJump;									// ジャンプフラグ
	bool jumping;									// ジャンプ中フラグ

	// コリジョンと物理挙動
	Ray	ray;										// レイ
	BoundingSphere sphereCollider;					// バウンディングスフィア
	float difference;								// フィールド補正差分
	bool onGround;									// 接地判定
	bool onGroundBefore;							// 直前フレームの接地判定
	D3DXVECTOR3	groundNormal;						// 接地面法線
	bool isHitPlayer;								// プレイヤーと接触している
	float friction;									// 摩擦係数

	// 追跡マーク
	EnemyChaseMark* markRenderer;					// 追跡マークレンダラー
	enemyChaseMarkNS::StandardChaseMark* markFront;	// 追跡マークポインタ
	enemyChaseMarkNS::StandardChaseMark* markBack;	// 追跡マークポインタ
	D3DXVECTOR3 markDirection;						// ●追跡マークの方角

	// State
	float cntTimeDie;								// 死亡ステート時間カウンタ

	// サウンド
	PLAY_PARAMETERS playParameters;

	// エフェクト
	enemyNS::DeathEffect* deathEffect;				// 死亡エフェクト

	//エネミーを撃破したプレイヤー
	int playerNo;

	//------------------
	// アクションクエリ
	//------------------
	// センサー
	bool sensor();
	// 経路探索
	void searchPath();
	// 攻撃
	void attack();
	// 移動
	void move(float frameTime);
	//------------------
	// アクションの実装
	//------------------
	// 視覚センサー
	bool eyeSensor(int playerType);
	// 聴覚センサー
	bool earSensor(int playerType);
	// ステアリング
	void steering(float frameTime);
	// 攻撃中
	void attacking(float frameTime);
	//----------------
	// 事後処理の実装
	//----------------
	// 接地処理
	void grounding();
	// 壁ずり
	void wallScratch();
	// 物理挙動
	void physicalBehavior();
	// 物理の更新
	void updatePhysics(float frameTime);
	// 姿勢の更新
	void updatePosture(float frameTime);
	// 到着判定
	void checkArrival();
	// 記憶の更新
	void updataBlackBoard(float frameTime);
	//--------------
	// ステート処理
	//--------------
	// 追跡ステート
	virtual void chase(float frameTime);
	// 警戒ステート
	virtual void patrol(float frameTime);
	// 休憩ステート
	virtual void rest(float frameTime);
	// ツリー攻撃ステート
	virtual void attackTree(float frameTime);
	// 死亡ステート
	virtual void die(float frameTime);
	//----------
	// サウンド
	//----------
	// サウンドプレイパラメータの取得
	PLAY_PARAMETERS getPlayParameters(enemyNS::SE_3D soundType, int enemyType);
	// 足音の処理
	void footsteps(D3DXVECTOR3 playerPos, int playerID);
	//--------
	// その他
	//--------
	void reset();
	// 自動破棄を行うか確認
	void checkAutoDestruction();
	// 適切なプレイヤーを追跡ターゲットに設定する
	void setPlayerChaseTarget();
	// デバッグ用目的地設定
	void setDebugDestination();
	// 巡回路リングバッファの更新
	void updatePatrolRoute();
	// 追跡マークの作成
	void createMark();
	// 追跡マークの破棄
	void deleteMark();

public:
	//----------
	// 基本処理
	//----------
	Enemy(enemyNS::ConstructionPackage constructionPackage);
	~Enemy();
	// 更新処理
	virtual void update(float frameTime) = 0;
	// 事前処理
	virtual void preprocess(float frameTime);
	// 事後処理
	virtual void postprocess(float frameTime);
	//----------------------
	// ステート切り替え準備
	//----------------------
	// 追跡ステートの準備
	void prepareChase();
	// 警戒ステートの準備
	void preparePatrol();
	// 休憩ステートの準備
	void prepareRest();
	// ツリー攻撃ステートの準備
	void prepareAttackTree();
	// 死亡ステートの準備
	void prepareDie();
	//-----------------
	// Getter & Setter
	//-----------------
	Enemy* getAdr() { return this; }
	int getEnemyID();
	static int getNumOfEnemy();
	enemyNS::EnemyData* getEnemyData();
	BoundingSphere* getSphereCollider();
	LPD3DXMESH getMesh();
	bool getNoticedOfPlayer(int playerType);
	bool getIsAttacking();
	int getChasingPlayer();
	int getPlayerNo();
	bool getIsPayingNewAttention();
	bool getCanDamageTree();
	int getCntDestroyParts();
	// エネミーのオブジェクトの数を初期化
	static void resetNumOfEnemy();
	// ダメージ処理
	void damage(int _damage,int playerNo);
	// 攻撃中状態を止める
	void stopAttacking();
	// プレイヤーに気づく
	void setAttention(D3DXVECTOR3 setting);
	// 移動ターゲットを設定
	void setMovingTarget(D3DXVECTOR3* _target);
	// 攻撃ターゲットを設定
	void setAttackTarget(Object* _target);
	// 倒されたプレイヤーを設定
	void setPlayerNo(int playerNo);
	// ツリーに攻撃がヒットしたか設定
	void setTreeHit(bool setting);
	// ツリーに攻撃がヒットしたか設定
	void setCanDamageTree(bool setting);
	// フィールドワールドマトリクスを設定
	void setFieldMatrix(D3DXMATRIX *matrix);
	
#ifdef _DEBUG
	//-----------
	// Debug Use
	//-----------
	LPDIRECT3DDEVICE9 device;			// Direct3Dデバイス
	Camera*	camera;						// 操作するカメラへのポインタ
	Input* input;						// 入力処理
	enemyNS::OperationKeyTable keyTable;// 操作キーテーブル
	float reverseValueXAxis;			// 操作X軸
	float reverseValueYAxis;			// 操作Y軸
	static int debugEnemyID;			// デバッグするエネミーのID
	Ray eyeAngleRay[4];					// 視覚レイ表示用
	BoundingSphere hearingSphere[2];	// 聴覚距離表示用
	DWORD faceNumber;					// デバッグ用面番号

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
	void debugSensor();
	// ImGUIへの出力
	void outputGUI();
#endif//_DEBUG
};
#pragma endregion
