//-----------------------------------------------------------------------------
// ベアークラス [Bear.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include"Enemy.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace bearNS
{
	enum PARTS_TYPE
	{
		BODY,		// 胴体
		ARM_L,		// 左腕
		ARM_R,		// 右腕
		WAIST,		// 腰
		LEG_L,		// 左足
		LEG_R,		// 右足
		PARTS_MAX	// パーツの数
	};

	const D3DXVECTOR3 PARTS_OFFSET_POS[PARTS_MAX] =
	{
		D3DXVECTOR3(0.0f, 21.06f, 0.0f),	// 胴体
		D3DXVECTOR3(-6.46f, 28.71f, -1.45f),// 左腕
		D3DXVECTOR3(6.46f, 28.71f, -1.45f),	// 右腕
		D3DXVECTOR3(0.0f, 21.06f, 0.0f),	// 腰
		D3DXVECTOR3(-1.99f, 16.01f, -1.05f),// 左足
		D3DXVECTOR3(1.99f, 16.01f, -1.05f),	// 右足
	};
}


//=============================================================================
//クラス定義
//=============================================================================
class Bear: public Enemy
{
private:
	Object* parts[bearNS::PARTS_MAX];			// パーツオブジェクト
	// ※パーツオブジェクトはObjectクラスの更新処理を行わない.
	// ※ワールド変換等の処理はアニメーションマネージャが代替する.

public:
	Bear(enemyNS::ConstructionPackage constructionPackage);
	~Bear();

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
