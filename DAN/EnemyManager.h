//-----------------------------------------------------------------------------
// エネミー管理クラス [Enemy.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <string>
#include <vector>
#include "Base.h"
#include "Camera.h"
#include "Player.h"
#include "StaticMeshRenderer.h"
#include "LinkedList.h"
#include "EnemyTools.h"
#include "Enemy.h"
#include "Wolf.h"
#include "Tiger.h"
#include "Bear.h"
#include "GameMaster.h"
#include "Sound.h"
#include "SoundBase.h"
#include "EnemyChaseMark.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace enemyNS
{
	// 同時に存在してよいエネミーの最大数
	const int ENEMY_OBJECT_MAX = 30;

	// オブジェクト作成基準の近距離(2乗)
	const float NEAR_DISTANCE2 = 10000.0f;	// 100 * 100

	// オブジェクト破棄基準の遠距離(2乗)
	const float FAR_DISTANCE2 = 14400.0f;	// 120 * 120

	// エネミーデータリストのチェック頻度
	const float DATA_LIST_CHECK_INTERVAL = 0.5f;
}


//=============================================================================
//クラス定義
//=============================================================================
class EnemyManager :public Base
{
private:
	LinkedList<enemyNS::EnemyData> enemyDataList;		// エネミーデータリスト
	std::vector<Enemy*> enemyList;						// エネミーポインタリスト
	int nextID;											// 次回エネミー発行ID
	LPD3DXMESH	attractorMesh;							// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;						// 重力（引力）発生オブジェクトマトリックス
	GameMaster* gameMaster;								// ゲーム管理オブジェクト
	Player* player;										// プレイヤー
	float cntTimeDataList;								// エネミーデータリストのチェック時間カウンタ
	EnemyChaseMark* markRenderer;						// 追跡マーク描画

	// 描画オブジェクト
	StaticMeshRenderer* wolfRenderer;					// 描画オブジェクト
	StaticMeshRenderer* tigerRenderer;					// 描画オブジェクト
	StaticMeshRenderer* bearRenderer;					// 描画オブジェクト
	StaticMeshRenderer* tigerBodyRenderer;				// 描画オブジェクト
	StaticMeshRenderer* tigerBulletRenderer;


public:
	void initialize(std::string _sceneName, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix, GameMaster* _gameMaster, Player* _player);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	enemyNS::EnemyData* createEnemyData(enemyNS::ENEMYSET enemySetting);
	void createEnemy(enemyNS::EnemyData* enemyData);
	void destroyEnemyData(int _enemyID);
	void destroyAllEnemyData();
	void destroyEnemy(int _enemyID);
	void destroyEnemy(Enemy* enemy);
	void destroyAllEnemy();
	void assertDestructionOrder();
	enemyNS::EnemyData* findEnemyData(int _enemyID);
	Enemy* findEnemy(int _enemyID);
	int issueNewEnemyID();
	void outputGUI();
	void relocateEnemyAccordingToFile();

	// Getter
	LinkedList<enemyNS::EnemyData>* getEnemyDataList();
	std::vector<Enemy*>& getEnemyList();
	int getNextID();

#ifdef _DEBUG
	Camera* camera;
	void setDebugEnvironment(Camera* _camera, Player* _player)
	{
		camera = _camera;
		player = _player;
	}
#endif
};
