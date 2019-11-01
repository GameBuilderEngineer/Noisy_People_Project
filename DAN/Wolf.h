//-----------------------------------------------------------------------------
// ウルフクラス [Wolf.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include"Enemy.h"

//=============================================================================
//クラス定義
//=============================================================================
class Wolf: public Enemy
{
private:

public:
	Wolf(StaticMesh* _staticMesh, enemyNS::EnemyData* _enemyData);
	~Wolf();

	void update(float frameTime);	// 更新
	void chase() override;			// 追跡ステート
	void patrol() override;			// 警戒ステート
	void rest() override;			// 休憩ステート
	void die() override;			// 死亡ステート

	// Getter

	// Setter
};
