//-----------------------------------------------------------------------------
// タイガークラス [Tiger.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include "Enemy.h"
#include "TigerAnimation.h"
#include "TigerBullet.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace tigerNS
{
	enum PARTS_TYPE
	{
		BODY,		// 胴体
		LEG_L,		// 左足
		LEG_R,		// 右足
		GUN,		// 銃口
		PARTS_MAX	// パーツの数
	};	
}


//=============================================================================
//クラス定義
//=============================================================================
class Tiger: public Enemy
{
private:
	Object* parts[tigerNS::PARTS_MAX];			// パーツオブジェクト
	TigerAnimationManager* animationManager;	// アニメーションマネージャ
	TigerBulletManager* bulletManager;			// バレットマネージャ
	D3DXVECTOR3 muzzlePosition;					// ●銃口ポジション

public:
	Tiger(enemyNS::ConstructionPackage constructionPackage);
	~Tiger();

	void update(float frameTime);				// 更新
	void chase(float frameTime) override;		// 追跡ステート
	void patrol(float frameTime) override;		// 警戒ステート
	void rest(float frameTime) override;		// 休憩ステート
	void attackTree(float frameTime) override;	// ツリー攻撃ステート
	void die(float frameTime) override;			// 死亡ステート

	void shot(Player* target);

	// Getter
	Object* getParts(int type);

	// Setter
};
