//==========================================
//�d�̓^���N����[electTank.cpp]
//����W��
//==========================================

//==========================================
//�C���N���[�h
//==========================================
#include "Green.h"

//==========================================
// using�錾
//==========================================
using namespace greenNS;

//==========================================
//�R���X�g���N�^
//==========================================
Green::Green()
{
		green = new Sprite;
}

//==========================================
//�f�X�g���N�^
//==========================================
Green::~Green()
{
		delete green;
}

//==========================================
//������
//==========================================
void Green::initialize(int playerType)
{
	BasicUI::assingSize(WIDTH_GREEN, HEIGHT_GREEN);
	switch (playerType)
	{
	case basicUiNS::P1:
			BasicUI::assingPosition(GREEN_POSTION);
			BasicUI::initialize(green, textureNS::reference(textureNS::UI_EN_TANK));
		break;
	case basicUiNS::P2:
		BasicUI::assingPosition(GREEN_POSTION_P2);
		BasicUI::initialize(green, textureNS::reference(textureNS::UI_EN_TANK));
		break;
	}
}

//==========================================
//�`�揈��
//==========================================
void Green::render(int state)
{
	switch (state)
	{
	case 2:
		green->render();
		break;
	case 3:
		green->render();
		break;
	}
	
}