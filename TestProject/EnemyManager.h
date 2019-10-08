//-----------------------------------------------------------------------------
// �G�l�~�[�Ǘ��N���X [Enemy.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "Enemy.h"


//=============================================================================
// ���O���
//=============================================================================
namespace enemyNS
{
	enum ENEMY_TYPE
	{
		WOLF,
		TIGER,
		BEAR,
		ENMEY_TYPE_MAX
	};
	
	const int INITIAL_RESERVE = 30;				// �G�l�~�[�|�C���^�̏����m�ې�
}


//=============================================================================
//�N���X��`
//=============================================================================
class EnemyManager
{
private:
	std::vector<Enemy*> enemyList;				// �G�l�~�[���X�g

public:
	void initialize();
	void uninitialize();
	void update();
	void render();
	void createEnemy(enemyNS::EnemyData enemyData);
	void destroyEnemy(int id);
	void outputGUI();
};