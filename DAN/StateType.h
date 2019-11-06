//-----------------------------------------------------------------------------
// ƒXƒe[ƒgí—Ş [StateType.h]
// AuthorFGP12A332 32 ’†˜a‹P
// ì¬“úF2019/11/3
//-----------------------------------------------------------------------------
#pragma once
#include "State.h"
#include "StateMachine.h"



//=============================================================================
// ’ÇÕ
//=============================================================================
class ChaseState: public State
{
private:
	ChaseState() { number = stateMachineNS::CHASE; }

public:
	static State* getInstance(void)
	{
		static ChaseState instance;
		return &instance;
	}
	State* transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType);
};


//=============================================================================
// Œx‰ú
//=============================================================================
class PatrolState : public State
{
private:
	PatrolState() { number = stateMachineNS::PATROL; }

public:
	static State* getInstance(void)
	{
		static PatrolState instance;
		return &instance;
	}
	State* transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType);
};


//=============================================================================
// ‹xŒe
//=============================================================================
class RestState : public State
{
private:
	RestState() { number = stateMachineNS::REST; }

public:
	static State* getInstance(void)
	{
		static RestState instance;
		return &instance;
	}
	State* transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType);
};


//=============================================================================
// €–S
//=============================================================================
class DieState : public State
{
private:
	DieState() { number = stateMachineNS::DIE; }

public:
	static State* getInstance(void)
	{
		static DieState instance;
		return &instance;
	}
	State* transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType);
};
