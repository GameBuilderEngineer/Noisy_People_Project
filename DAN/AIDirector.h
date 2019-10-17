//-----------------------------------------------------------------------------
// メタAIクラス [AIDirector.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once
#include "Sensor.h"
#include "EventMaker.h"
#include "OperationGenerator.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace aiNS
{
	// プレイヤー解析データ
	struct PlayerAnalyticalData
	{

	};
	typedef PlayerAnalyticalData PLAYERAD;

	// エネミー解析データ
	struct EnemyAnalyticalData
	{

	};
	typedef EnemyAnalyticalData ENEMYAD;

	// ツリー解析データ
	struct TreeAnalyticalData
	{
		int numBeingAttackedTree;			// 襲撃されている木の数

	};
	typedef TreeAnalyticalData TREEAD;

	// アイテム解析データ
	struct ItemAnalyticalData
	{

	};
	typedef ItemAnalyticalData ITEMAD;

	// イベントリスト
	enum EVENT_LIST
	{
		RESPAWN_ENEMY_AROUND_PLAYER,
		RESPAWN_ITEM,
		ENEMY_ATTACKS_TREE,
	};
}


//=============================================================================
//クラス定義
//=============================================================================
// AIディレクターは３つのモジュールを操作しゲーム中のイベントを統括する
// センサーはゲームオブジェクトのパラメータを取集する
// イベントメーカーはパラメータから発生させるイベントを決定する
// オペレーションジェネレータが各オブジェクト管理クラスにイベント実行を指示する
class AIDirector
{
private:
	aiNS::PLAYERAD playerAD;			// プレイヤー解析データ
	aiNS::ENEMYAD enemyAD;				// エネミー解析データ
	aiNS::TREEAD treeAD;				// ツリー解析データ
	aiNS::ITEMAD itemAD;				// アイテム解析データ

	Sensor sensor;						// センサーモジュール
	EventMaker eventMaker;				// イベント生成モジュール
	OperationGenerator opeGenerator;	// イベント実行モジュール
	int frameCnt;						// フレームカウンタ

public:
	void run();							// 実行
	void initialize();					// 初期化
	void outputGUI();					// ImGuiに表示
};
