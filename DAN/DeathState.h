//===================================================================================================================================
//�yDeathState.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/26
// [�X�V��]2019/11/26
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Player.h"



//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace deathNS {

	//===================================================================================================================================
	//�y�ʏ��ԃN���X�z
	//===================================================================================================================================
	class DeathState : public AbstractState
	{
	private:
		Player* player;
	public:
		DeathState(Player* player);
		virtual void update(float frameTime);
		virtual void start();
		virtual void operation();
		virtual void physics();
		virtual void controlCamera();
		virtual AbstractState* transition();
		virtual void end();

	};
}