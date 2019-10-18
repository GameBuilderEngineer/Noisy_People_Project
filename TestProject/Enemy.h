//-----------------------------------------------------------------------------
// エネミー基底クラス [Enemy.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshObject.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace enemyNS
{
	struct EnemyData
	{
		int type;
		int state;
		int position;
	};

}


//=============================================================================
//クラス定義
//=============================================================================
class Enemy: public StaticMeshObject
{
private:
	int id;							// 識別番号（0..*）
	int type;						// エネミータイプ
	static int cntID;				// 識別番号カウンタ
	static int numOfEnemy;			// エネミーの総数

public:
	Enemy();
	~Enemy();
	virtual void update();
	virtual void render();

	void setEnemyData(enemyNS::EnemyData enemyData);


	// Getter
	int getID();
	int getType();
	static int getNumOfEnemy();
};
