//-----------------------------------------------------------------------------
// メタAIイベント実行クラス [OperationGenerator.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once

//=============================================================================
// 名前空間
//=============================================================================
namespace aiNS
{
	// 前方宣言
	struct	PlayerAnalyticalData;
	typedef PlayerAnalyticalData PLAYERAD;
	struct	EnemyAnalyticalData;
	typedef EnemyAnalyticalData	ENEMYAD;
	struct	TreeAnalyticalData;
	typedef TreeAnalyticalData TREEAD;
	struct	ItemAnalyticalData;
	typedef ItemAnalyticalData ITEMAD;
}


//=============================================================================
//クラス定義
//=============================================================================
class OperationGenerator
{
private:
	aiNS::PLAYERAD* playerAD;
	aiNS::ENEMYAD* enemyAD;
	aiNS::TREEAD* treeAD;
	aiNS::ITEMAD* itemAD;

public:
	void initialize(aiNS::PLAYERAD* _playerAD, aiNS::ENEMYAD* _enemyAD,
		aiNS::TREEAD* _treeAD, aiNS::ITEMAD* _itemAD);
	void uninitialize();
	void update();
};
