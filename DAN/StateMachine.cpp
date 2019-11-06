//-----------------------------------------------------------------------------
// ステートマシン[StateMachine.h]
// Author：GP12A332 32 中込和輝
// 作成日：2019/11/3
//-----------------------------------------------------------------------------
#include "StateType.h"
#include "StateMachine.h"
using namespace stateMachineNS;

//=============================================================================
// コンストラクタ
//=============================================================================
StateMachine::StateMachine(void)
{
	// 遷移時間チェックを初期化
	for (int i = 0; i < STATE_MAX; i++)
	{
		checker[i].isCheckTarget = WHETHER_IS_CHECK_TARGET[i];
		checker[i].necesarryElapsedTime = NECESSARY_ELAPSED_TIME[i];
		checker[i].elapsedTime = 0.0f;
		checker[i].executable = true;
	}
}


//=============================================================================
// 初期化
//=============================================================================
void StateMachine::initialize(int stateNumber)
{
	switch (stateNumber)
	{
	case CHASE:		current = ChaseState::getInstance();	break;
	case PATROL:	current = PatrolState::getInstance();	break;
	case REST:		current = RestState::getInstance();		break;
	case DIE:		current = DieState::getInstance();		break;
	}
}


//=============================================================================
// 実行
//=============================================================================
int StateMachine::run(float frameTime, int enemyType)
{
	old = current;
	current = old->transition(checker, enemyType);

	// 遷移前のステートへの再遷移に時間制限を掛ける（遷移時間チェック）
	// 短時間でステートが循環し続け行動が定まらなくなる現象を避けるため
	if (current != old && checker[old->getNumber()].isCheckTarget)
	{
        checker[old->getNumber()].elapsedTime = 0.0f;
		checker[old->getNumber()].executable = false;
	}
	for (int i = 0; i < stateMachineNS::STATE_MAX; i++)
	{
		if (checker[old->getNumber()].isCheckTarget == false) continue;
		if (checker[i].executable) continue;

		checker[i].elapsedTime += frameTime;
		if (checker[i].elapsedTime > checker[i].necesarryElapsedTime)
		{
			checker[i].elapsedTime = 0.0f;
			checker[i].executable = true;
		}
	}

	return current->getNumber();
}