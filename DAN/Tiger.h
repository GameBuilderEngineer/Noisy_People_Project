//-----------------------------------------------------------------------------
// �^�C�K�[�N���X [Tiger.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include"Enemy.h"

//=============================================================================
//�N���X��`
//=============================================================================
class Tiger: public Enemy
{
private:

public:
	Tiger(StaticMesh* _staticMesh, enemyNS::EnemyData* _enemyData);
	~Tiger();

	void update(float frameTime);	// �X�V
	void chase() override;			// �ǐՃX�e�[�g
	void patrol() override;			// �x���X�e�[�g
	void rest() override;			// �x�e�X�e�[�g
	void die() override;			// ���S�X�e�[�g

	// Getter

	// Setter
};