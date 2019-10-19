//===================================================================================================================================
//�yStone.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/10/14
// [�X�V��]2019/10/14
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshObject.h"

//===================================================================================================================================
//�y�΃N���X�z
//===================================================================================================================================
class Stone :	public Base
{
private:
	StaticMeshObject* renderer;
	bool needUpdate;
public:
	Stone();
	~Stone();
	void initialize();
	void update();
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generate(D3DXVECTOR3 position);
};
