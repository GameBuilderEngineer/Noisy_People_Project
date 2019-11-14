//-----------------------------------------------------------------------------
// ステートマシン[StateMachine.h]
// Author：GP12A332 32 中込和輝
// 作成日：2019/11/3
//-----------------------------------------------------------------------------
#pragma once

namespace stateMachineNS
{
	// ステートの種類
	enum ENEMY_STATE
	{
		CHASE,		// 追跡ステート
		PATROL,		// 警戒ステート
		REST,		// 休憩ステート
		ATTACK_TREE,// ツリー攻撃ステート
		DIE,			// 死亡ステート
		STATE_MAX,	// ステートの数
	};

	// 遷移時間チェック対象
	const bool WHETHER_IS_CHECK_TARGET[STATE_MAX] =
	{
		true,		// 追跡ステート
		true,		// 警戒ステート
		true,		// 休憩ステート
		false,		// 死亡ステート
	};

	// 遷移に必要な経過時間
	const float NECESSARY_ELAPSED_TIME[STATE_MAX] =
	{
		2.0f,		// 追跡ステート
		2.0f,		// 警戒ステート
		5.0f,		// 休憩ステート
		0.0f,		// 死亡ステート
	};

	// 遷移時間チェック構造体
	struct TransitionTimeChecker
	{
		bool	isCheckTarget;			// チェック対象か
		float	necesarryElapsedTime;	// 必要経過時間（参照時間）
		float	elapsedTime;			// 経過時間
		bool	executable;				// 実行可能
	};
}

// 前方宣言
class State;

//=============================================================================
// クラス定義
//=============================================================================
class StateMachine
{
private:
	State*		current;				// 現在のステート
	State*		old;					// 一つ前のステート

	// 遷移時間チェックオブジェクト
	stateMachineNS::TransitionTimeChecker	checker[stateMachineNS::STATE_MAX];

public:
	StateMachine();
	// 初期化
	void initialize(int stateNumber);
	// ステートマシンの実行
	int run(float frameTime, int enemyType);
};
