//-----------------------------------------------------------------------------
// タイガークラス [Tiger.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include "Enemy.h"
#include "TigerBullet.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace tigerNS
{
	// パーツの種類
	enum PARTS_TYPE
	{
		BODY,		// 胴体
		GUN,		// 銃口
		LEG_L,		// 左足
		LEG_R,		// 右足
		PARTS_MAX	// パーツの数
	};	

	// パーツ関係のオフセット座標
	const D3DXVECTOR3 PARTS_OFFSET_POS[PARTS_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 胴体
		D3DXVECTOR3(0.0f, 2.71f, -0.94f),	// 銃
		D3DXVECTOR3(0.85f, 1.84f, -0.62f),	// 左足
		D3DXVECTOR3(-0.85f, 1.84f, -0.62f),	// 右足
	};
	//const D3DXVECTOR3 MUZZLE_POSITION = D3DXVECTOR3(0.0f, 3.01f, -2.47f);
	const D3DXVECTOR3 MUZZLE_POSITION = D3DXVECTOR3(0.0f, 0.28f, 3.38f);
}


//=============================================================================
//クラス定義
//=============================================================================
class Tiger: public Enemy
{
private:
	enemyNS::EnemyParts* parts[tigerNS::PARTS_MAX];
	// ※パーツオブジェクトはObjectクラスの更新処理を行わない.
	// ※ワールド変換等の処理はアニメーションマネージャが代替する.

	TigerBulletManager* bulletManager;			// バレットマネージャ

public:
	Tiger(enemyNS::ConstructionPackage constructionPackage);
	~Tiger();

	void update(float frameTime);				// 更新
	void chase(float frameTime) override;		// 追跡ステート
	void patrol(float frameTime) override;		// 警戒ステート
	void rest(float frameTime) override;		// 休憩ステート
	void attackTree(float frameTime) override;	// ツリー攻撃ステート
	void die(float frameTime) override;			// 死亡ステート

	void shot();

	// Getter
	enemyNS::EnemyParts* getParts(int type);
	TigerBulletManager* getBulletMangaer();

	// Setter
};
