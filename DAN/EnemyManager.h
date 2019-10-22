//-----------------------------------------------------------------------------
// エネミー管理クラス [Enemy.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"
#include <vector>
#include "LinkedList.h"
#include "EnemyTools.h"
#include "Enemy.h"
#include "Wolf.h"
#include "Tiger.h"
#include "Bear.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace enemyNS
{
	// 同時に存在してよいエネミーの最大数
	const int ENEMY_OBJECT_MAX = 30;
}


//=============================================================================
//クラス定義
//=============================================================================
class EnemyManager
{
private:
	LinkedList<enemyNS::EnemyData> enemyDataList;		// エネミーデータリスト
	std::vector<Enemy*> enemyList;						// エネミーポインタリスト
	StaticMeshObject* wolfRenderer;						// 描画オブジェクト
	StaticMeshObject* tigerRenderer;					// 描画オブジェクト
	StaticMeshObject* bearRenderer;						// 描画オブジェクト
	int nextID;											// 次回エネミー発行ID
	LPD3DXMESH	attractorMesh;							// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;						// 重力（引力）発生オブジェクトマトリックス

public:
	void initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	enemyNS::EnemyData* createEnemyData(enemyNS::ENEMYSET enemySetting);
	void createEnemy(enemyNS::EnemyData* enemyData);
	void destroyEnemyData(int _id);
	void destroyAllEnemyData();
	void destroyEnemy(int _id);
	void destroyAllEnemy();
	void assertDestructionOrder();
	int issueNewID();
	void outputGUI();
	void relocateEnemyAccordingToFile();

	// Getter
	LinkedList<enemyNS::EnemyData>* getEnemyDataList();
	std::vector<Enemy*>& getEnemyList();
};
