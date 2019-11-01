//-----------------------------------------------------------------------------
// エネミー管理クラス [Enemy.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"
#include "Camera.h"
#include "Player.h"
#include <vector>
#include "StaticMeshRenderer.h"
#include "LinkedList.h"
#include "EnemyTools.h"
#include "Enemy.h"
#include "Wolf.h"
#include "Tiger.h"
#include "Bear.h"

#include "Sound.h"
#include "SoundBase.h"


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
	StaticMeshRenderer* wolfRenderer;					// 描画オブジェクト
	StaticMeshRenderer* tigerRenderer;					// 描画オブジェクト
	StaticMeshRenderer* bearRenderer;					// 描画オブジェクト
	int nextID;											// 次回エネミー発行ID
	LPD3DXMESH	attractorMesh;							// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;							// 重力（引力）発生オブジェクトマトリックス

public:
	void initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	enemyNS::EnemyData* createEnemyData(enemyNS::ENEMYSET enemySetting);
	void createEnemy(enemyNS::EnemyData* enemyData);
	void destroyEnemyData(int _enemyID);
	void destroyAllEnemyData();
	void destroyEnemy(int _enemyID);
	void destroyAllEnemy();
	void assertDestructionOrder();
	int issueNewEnemyID();
	void outputGUI();
	void relocateEnemyAccordingToFile();

	//サウンド
	PLAY_PARAMETERS *playParameters;
	void uninitializeSound();
	void footsteps(D3DXVECTOR3 playerPos, int playerID);

	// Getter
	LinkedList<enemyNS::EnemyData>* getEnemyDataList();
	std::vector<Enemy*>& getEnemyList();
	int getNextID();

#ifdef _DEBUG
	Camera* camera;
	Player* player;
	void setDebugEnvironment(Camera* _camera, Player* _player)
	{
		camera = _camera;
		player = _player;
	}
#endif
};
