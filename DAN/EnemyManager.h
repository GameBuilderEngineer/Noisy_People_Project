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
#include "Marker.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace enemyNS
{
	// 同時に存在してよいエネミーの最大数
	const int ENEMY_OBJECT_MAX = 30;

	// オブジェクト作成基準の近距離(2乗)
	const float NEAR_DISTANCE2 = 22500.0f;	// 150 * 150

	// オブジェクト破棄基準の遠距離(2乗)
	const float FAR_DISTANCE2 = 28900.0f;	// 170 * 170

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
	bool canUpdate;										// アップデート実行フラグ

public:
	// 描画オブジェクト
	static StaticMeshRenderer* wolfBodyRenderer;		// WOLFの胴体
	static StaticMeshRenderer* wolfArmRenderer;			// WOLFの腕
	static StaticMeshRenderer* tigerBodyRenderer;		// TIGERの胴体
	static StaticMeshRenderer* tigerGunRenderer;		// TIGERの銃口
	static StaticMeshRenderer* tigerLegLRenderer;		// TIGERの左足
	static StaticMeshRenderer* tigerLegRRenderer;		// TIGERの右足
	static StaticMeshRenderer* bearBodyRenderer;		// BEARの胴体
	static StaticMeshRenderer* bearArmLRenderer;		// BEARの左腕
	static StaticMeshRenderer* bearArmRRenderer;		// BEARの右腕
	static StaticMeshRenderer* bearWaistRenderer;		// BEARの腰
	static StaticMeshRenderer* bearLegLRenderer;		// BEARの左足
	static StaticMeshRenderer* bearLegRRenderer;		// BEARの右足
	static EnemyChaseMark* markRenderer;				// 追跡マーク描画
	MarkerRenderer*	markerRenderer;						// マーカー（BEAR位置表示用）

	void initialize(std::string _sceneName, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix, GameMaster* _gameMaster, Player* _player, MarkerRenderer*	_markerRenderer);
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
	void updatePartsRenderer();
	int issueNewEnemyID();
	void outputGUI();
	void relocateEnemyAccordingToFile();

	// Getter
	LinkedList<enemyNS::EnemyData>* getEnemyDataList();
	std::vector<Enemy*>& getEnemyList();
	int getNextID();

	// Setter
	void setUpdate(bool setting);

#ifdef _DEBUG
	Camera* camera;
	void setDebugEnvironment(Camera* _camera, Player* _player)
	{
		camera = _camera;
		player = _player;
	}
#endif
};
