//===========================================
//�p�[�Z���g
//����W��
//===========================================

//===========================================
//�C���N���[�h
//===========================================
#include "Persent.h"

//===========================================
// using�錾
//===========================================
using namespace persentNS;

//===========================================
//�R���X�g���N�^
//===========================================
Persent::Persent()
{
	persent = new Sprite;
}

//===========================================
//�f�X�g���N�^
//===========================================
Persent::~Persent()
{
	delete persent;
}

//===========================================
//������
//===========================================
void Persent::initialize()
{
	BasicUI::assingSize(WIDTH_PERSENT, HEIGHT_PERSENT);
	BasicUI::assingPosition(PERSNT_POSITION);
	BasicUI::initialize(persent, textureNS::reference(textureNS::RESULT_PERSENT));
}

//===========================================
//�`��
//===========================================
void Persent::render()
{
	persent->render();
}

//===========================================
//�X�V
//===========================================
void Persent::update()
{

}