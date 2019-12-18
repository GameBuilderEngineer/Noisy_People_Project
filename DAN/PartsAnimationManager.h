//-----------------------------------------------------------------------------
// パーツアニメーションマネージャ [PartsAnimationManager.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/12
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "PartsAnimation.h"

//=============================================================================
//クラス定義
//=============================================================================
class PartsAnimationManager: public Base
{
protected:
	int partsMax;				// パーツの数
	Object* parent;				// 親オブジェクト
	Object** parts;				// パーツのポインタ配列(パーツの数に合わせて動的に作成される)
	D3DXVECTOR3* rot;			// 回転ベクトル配列(パーツの数に合わせて動的に作成される)
	int animatinMax;			// アニメーションの数
	DWORD flagState;			// フラグステート(アニメーションの実行を管理する)
	PartsAnimation** animation;	// アニメーションオブジェクト配列（派生クラスで動的に作成⇔破棄）

public:

	// 3Dサウンド
	bool wasTimingCame;			// 移動音再生タイミングをコントロールするフラグ
	bool canPlayMoveSound;		// 移動音再生フラグ

	//-------------------------------------------------------------------------
	// コンストラクタにパーツの数、親オブジェクトのポインタ、パーツオブジェクト
	// のポインタを渡すことでマネージャーを初期化する.
	// パーツは全パーツのポインタ配列を用意して渡すことに注意する.
	//-------------------------------------------------------------------------
	// コンストラクタ
	PartsAnimationManager(int _partsMax, Object* _parent, Object** _parts);
	// デストラクタ
	~PartsAnimationManager();
	// 更新
	virtual void update(float frameTime) = 0;
	// パーツのマトリクスを計算する
	virtual void culcPartsMatrix() = 0;
	// アニメーション起動
	virtual void activate(PartsAnimation* animation);
	// アニメーション停止
	virtual void inactivate(PartsAnimation* animation);
	// アニメーション全停止
	void inactivateAll();
	// アニメーションを取得
	PartsAnimation* getAnimation(int type);

	//--------------------------------------------
	// 共通アニメーションは以下の関数をコールする
	//--------------------------------------------
	// デフォルトアニメーションに切り替え
	virtual void switchDefault() = 0;
	// 移動アニメーションに切り替え
	virtual void switchMove() = 0;
	// 攻撃アニメーションに切り替え
	virtual void switchAttack() = 0;
	// 死亡アニメーションに切り替え
	virtual void switchDead() = 0;
};
