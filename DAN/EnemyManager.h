//-----------------------------------------------------------------------------
// エネミー管理クラス [Enemy.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "EnemyTools.h"
#include "Enemy.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace enemyNS
{
	// 初期化時はデータリストに存在せずゲーム途中から作成されるエネミー(追加エネミー)
	// その数を見越して事前に確保しておくポインタの余剰個数
	const int DYNAMIC_SPAWN_MAX = 20;

	const int ENEMY_OBJECT_MAX = 30;	// 同時に存在してよいエネミーの最大数
}


//=============================================================================
//クラス定義
//=============================================================================
class EnemyManager
{
private:
	std::vector<enemyNS::EnemyData> enemyDataList;		// エネミーデータリスト
	std::vector<Enemy*> enemyList;						// エネミーポインタリスト
	StaticMeshObject* wolfRenderer;
	StaticMeshObject* tigerRenderer;
	StaticMeshObject* bearRenderer;
	int nextID;

public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createEnemy(enemyNS::EnemyData* enemyData);
	void destroyEnemy(int _id);
	void destroyAllEnemy();
	int issueNewID();
	void outputGUI();
};
