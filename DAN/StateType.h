//-----------------------------------------------------------------------------
// ƒXƒe[ƒgí—Ş [StateType.h]
// AuthorFGP12A332 32 ’†˜a‹P
// ì¬“úF2019/11/3
//-----------------------------------------------------------------------------
#pragma once
#include "State.h"


//=============================================================================
// ’ÇÕ
//=============================================================================
class ChaseState: public stateMachineNS::State
{
private:
	ChaseState() { number = stateMachineNS::CHASE; }

public:
	static State* getInstance(void)
	{
		static ChaseState instance;
		return &instance;
	}
	stateMachineNS::State* transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy);
};


//=============================================================================
// Œx‰ú
//=============================================================================
class PatrolState : public stateMachineNS::State
{
private:
	PatrolState() { number = stateMachineNS::PATROL; }

public:
	static State* getInstance(void)
	{
		static PatrolState instance;
		return &instance;
	}
	stateMachineNS::State* transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy);
};


//=============================================================================
// ‹xŒe
//=============================================================================
class RestState : public stateMachineNS::State
{
private:
	RestState() { number = stateMachineNS::REST; }

public:
	static State* getInstance(void)
	{
		static RestState instance;
		return &instance;
	}
	stateMachineNS::State* transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy);
};


//=============================================================================
// ƒcƒŠ[UŒ‚
//=============================================================================
class AttackTree : public stateMachineNS::State
{
private:
	AttackTree() { number = stateMachineNS::REST; }

public:
	static State* getInstance(void)
	{
		static AttackTree instance;
		return &instance;
	}
	stateMachineNS::State* transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy);
};


//=============================================================================
// €–S
//=============================================================================
class DieState : public stateMachineNS::State
{
private:
	DieState() { number = stateMachineNS::DIE; }

public:
	static State* getInstance(void)
	{
		static DieState instance;
		return &instance;
	}
	stateMachineNS::State* transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy);
};
