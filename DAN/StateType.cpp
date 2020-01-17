//-----------------------------------------------------------------------------
// ƒXƒe[ƒgí—Ş [StateType.h]
// AuthorFGP12A332 32 ’†˜a‹P
// ì¬“úF2019/11/3
//-----------------------------------------------------------------------------
#include "StateType.h"
#include "Enemy.h"	// ƒCƒ“ƒNƒ‹[ƒh‚µ‚È‚¢‚Æ‘O•ûéŒ¾‚Ì‚İ‚Ì‚½‚ßƒƒ“ƒo‚ÉƒAƒNƒZƒX‚Å‚«‚È‚¢
using namespace stateMachineNS;

//=============================================================================
// ’ÇÕó‘Ô‚©‚ç‚Ì‘JˆÚ
//=============================================================================
State* ChaseState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	if (enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_1P) == false &&
		enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_2P) == false &&
		checker[PATROL].executable)
	{
		enemy->preparePatrol();
		return PatrolState::getInstance();
	}

	if (enemy->getEnemyData()->hp == 0)
	{
		enemy->prepareDie();
		return DieState::getInstance();
	}

	return this;
}


//=============================================================================
// Œx‰úó‘Ô‚©‚ç‚Ì‘JˆÚ
//=============================================================================
State* PatrolState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		if (checker[CHASE].executable && enemy->getNoticedOfPlayer(i))
		{
			enemy->prepareChase();
			return ChaseState::getInstance();
		}
	}

	if (enemy->getEnemyData()->hp == 0)
	{
		enemy->prepareDie();
		return DieState::getInstance();
	}

	return this;
}


//=============================================================================
// ‹xŒeó‘Ô‚©‚ç‚Ì‘JˆÚ
//=============================================================================
State* RestState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	//for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	//{
	//	if (checker[CHASE].executable && enemy->getNoticedOfPlayer(i))
	//	{
	//		enemy->prepareChase();
	//		return ChaseState::getInstance();
	//	}
	//}

	//if (enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_1P) == false &&
	//	enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_2P) == false &&
	//	enemy->getIsPayingNewAttention() &&
	//	checker[PATROL].executable)
	//{
	//	enemy->preparePatrol();
	//	return PatrolState::getInstance();
	//}

	if (enemy->getEnemyData()->hp == 0)
	{
		enemy->prepareDie();
		return DieState::getInstance();
	}

	return this;
}


//=============================================================================
// ƒcƒŠ[UŒ‚ó‘Ô‚©‚ç‚Ì‘JˆÚ
//=============================================================================
State* AttackTree::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		if (checker[CHASE].executable && enemy->getNoticedOfPlayer(i))
		{
			enemy->prepareChase();
			return ChaseState::getInstance();
		}
	}

	if (enemy->getEnemyData()->targetTree == NULL &&
		enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_1P) == false &&
		enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_2P) == false &&
		checker[PATROL].executable)
	{
		enemy->preparePatrol();
		return PatrolState::getInstance();
	}

	if (enemy->getEnemyData()->targetTree == NULL)
	{
		int unko = 0;
	}

	if (enemy->getEnemyData()->hp == 0)
	{
		return DieState::getInstance();
	}

	return this;
}


//=============================================================================
// €–Só‘Ô‚©‚ç‚Ì‘JˆÚ
//=============================================================================
State* DieState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	return this;
}
