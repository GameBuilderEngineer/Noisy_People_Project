//-----------------------------------------------------------------------------
// ウルフクラス [Wolf.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include"Enemy.h"

namespace wolfNS
{
	enum PARTS_TYPE
	{
		BODY,		// 胴体
		ARM,		// 腕
		PARTS_MAX	// パーツの数
	};

	const D3DXVECTOR3 PARTS_OFFSET_POS[PARTS_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 胴体
		D3DXVECTOR3(0.0f, 1.00f, -0.264f),	// 腕
	};

}


//=============================================================================
// クラス定義
//=============================================================================
class Wolf: public Enemy
{
private:
	Object* parts[wolfNS::PARTS_MAX];			// パーツオブジェクト
// ※パーツオブジェクトはObjectクラスの更新処理を行わない.
// ※ワールド変換等の処理はアニメーションマネージャが代替する.
	// Sound
	PLAY_PARAMETERS playParmeters[2];

public:
	Wolf(enemyNS::ConstructionPackage constructionPackage);
	~Wolf();

	void update(float frameTime);				// 更新
	void chase(float frameTime) override;		// 追跡ステート
	void patrol(float frameTime) override;		// 警戒ステート
	void rest(float frameTime) override;		// 休憩ステート
	void attackTree(float frameTime) override;	// ツリー攻撃ステート
	void die(float frameTime) override;			// 死亡ステート

	// Getter
	Object* getParts(int type);

	// Setter
};
