//-----------------------------------------------------------------------------
// エネミー管理クラス [Enemy.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "Enemy.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace enemyNS
{
	enum ENEMY_TYPE
	{
		WOLF,
		TIGER,
		BEAR,
		ENMEY_TYPE_MAX
	};
	
	const int INITIAL_RESERVE = 30;				// エネミーポインタの初期確保数
}


//=============================================================================
//クラス定義
//=============================================================================
class EnemyManager
{
private:
	std::vector<enemyNS::EnemyData> enemyDataList;		// エネミーデータリスト
	std::vector<Enemy*> enemyList;						// エネミーポインタリスト

public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createEnemy(enemyNS::EnemyData* enemyData);
	void destroyEnemy(int _id);
	void outputGUI();
};
