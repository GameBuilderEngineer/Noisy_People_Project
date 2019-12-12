//-----------------------------------------------------------------------------
// エネミー追跡マーク[EnemyChaseMark.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/10
//-----------------------------------------------------------------------------
#pragma once
#include "InstancingBillboard.h"

//=============================================================================
// インスタンスクラス
//=============================================================================
namespace enemyChaseMarkNS
{
	class StandardChaseMark :public InstancingBillboardNS::Instance
	{
	public:
		static int cntInstance;	// 一応カウント　使わないけど

		//コンストラクタ
		StandardChaseMark(D3DXVECTOR3 position);
		//更新
		virtual void update(float frameTime) override;
	};
}


//=============================================================================
// レンダラークラス
//=============================================================================
class EnemyChaseMark :public InstancingBillboard
{
private:

public:
	EnemyChaseMark();
	~EnemyChaseMark();
	void update(float frameTime);

	// インスタンス生成と削除のラッパー関数
	void generateMark(enemyChaseMarkNS::StandardChaseMark* mark);
	void deleteMark(enemyChaseMarkNS::StandardChaseMark* mark);
};
