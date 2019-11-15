//-----------------------------------------------------------------------------
// ベアークラス [Bear.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include"Enemy.h"

//=============================================================================
//クラス定義
//=============================================================================
class Bear: public Enemy
{
private:

public:
	Bear(StaticMesh* _staticMesh, enemyNS::EnemyData* _enemyData);
	~Bear();

	void update(float frameTime);			// 更新
	void chase(float frameTime) override;	// 追跡ステート
	void patrol(float frameTime) override;	// 警戒ステート
	void rest(float frameTime) override;	// 休憩ステート
	void die(float frameTime) override;		// 死亡ステート

	// Getter

	// Setter
};
