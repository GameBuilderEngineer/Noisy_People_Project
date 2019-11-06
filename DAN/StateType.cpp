//-----------------------------------------------------------------------------
// ƒXƒe[ƒgí—Ş [StateType.h]
// AuthorFGP12A332 32 ’†˜a‹P
// ì¬“úF2019/11/3
//-----------------------------------------------------------------------------
#include "StateMachine.h"
#include "StateType.h"
using namespace stateMachineNS;

//=============================================================================
// ’ÇÕó‘Ô‚©‚ç‚Ì‘JˆÚ
//=============================================================================
State* ChaseState::transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType)
{
	if (checker[PATROL].executable)
	{ 
		return PatrolState::getInstance();
	}

	return this;
}


//=============================================================================
// Œx‰úó‘Ô‚©‚ç‚Ì‘JˆÚ
//=============================================================================
State* PatrolState::transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType)
{
	if (checker[CHASE].executable)
	{
		return ChaseState::getInstance();
	}

	return this;
}


//=============================================================================
// ‹xŒeó‘Ô‚©‚ç‚Ì‘JˆÚ
//=============================================================================
State* RestState::transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType)
{
	return this;
}


//=============================================================================
// €–Só‘Ô‚©‚ç‚Ì‘JˆÚ
//=============================================================================
State* DieState::transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType)
{
	return this;
}
