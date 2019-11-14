//-----------------------------------------------------------------------------
// ステート [StateMachine.h]
// Author：GP12A332 32 中込和輝
// 作成日：2019/7/20
//-----------------------------------------------------------------------------
#pragma once
#include "StateMachine.h"


namespace stateMachineNS
{
	class State
	{
	protected:
		int number;									// ステート番号

	public:
		int getNumber(void) { return number; }		// ステート番号を取得
													// ステート遷移
		virtual State* transition(stateMachineNS::TransitionTimeChecker* chekcer, Enemy* enemy) = 0;
	};
}
