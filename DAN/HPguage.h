//======================================
//HP�Q�[�W�N���X[HPguage.h]
//����W��
//======================================
#pragma once

//======================================
//�C���N���[�h
//======================================
#include "BasicUI.h"

//======================================
//���O���
//======================================
namespace hpGuageNS
{
	const int WIDTH_HP_GUAGE = (500);	//�^�C�}�[�t���[���̉���
	const int HEIGHT_HP_GUAGE = (30);	//�^�C�}�[�t���[���̏c��
	const D3DXVECTOR3 POSITION_HP_GUAGE = D3DXVECTOR3(102, 35, 0);	//�^�C�}�[�t���[���̕\���ʒu(��)
	
}

//======================================
//�N���X��`
//======================================
class HPguage:public BasicUI
{
public://�����o�[�ϐ�
	Sprite * hpGuage;
	int      hp;//HP�ۑ��p
	int		 width;
	//UV���W�Ǘ��p
	D3DXVECTOR2 uvCoord1;	
	D3DXVECTOR2 uvCoord2;
	D3DXVECTOR2 uvCoord3;
	D3DXVECTOR2 uvCoord4;

private:
public://�����o�[�֐�
	HPguage();
	~HPguage();
	void initialize();
	void render();
	void update(int hp);
	void changeGage(int hp);
};