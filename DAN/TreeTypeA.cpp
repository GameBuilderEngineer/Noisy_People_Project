//===================================================================================================================================
//�yTreeTypeA.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/10/14
// [�X�V��]2019/10/14
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "TreeTypeA.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
TreeTypeA::TreeTypeA(){
	renderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_001));
	initialize();
};

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
TreeTypeA::~TreeTypeA(){
	SAFE_DELETE(renderer);
};

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void TreeTypeA::initialize() {
	D3DXVECTOR3 position;
	for (int i = 0; i < 400; i++)
	{
		position.x = (float)((rand() % 2000) - 1000);
		position.y = 0.0f;
		position.z = (float)((rand() % 2000) - 1000);
		generate(position);
	}
	needUpdate = true;
};

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void TreeTypeA::update()
{
	if (!needUpdate)return;
	renderer->update();
	needUpdate = false;
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void TreeTypeA::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
}

//===================================================================================================================================
//�y�����z
//===================================================================================================================================
void TreeTypeA::generate(D3DXVECTOR3 position)
{
	Object* object = new Object();
	renderer->generateObject(object);
	object->initialize(&position);
}