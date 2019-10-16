//===================================================================================================================================
//yDeadTree.hz
// [์ฌา]HALGP12A332 11 ์ ๗
// [์ฌ๚]2019/10/16
// [XV๚]2019/10/16
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//yCN[hz
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshObject.h"

//===================================================================================================================================
//yฮNXz
//===================================================================================================================================
class DeadTree : public Base
{
private:
	StaticMeshObject* renderer;
	bool needUpdate;
public:
	DeadTree();
	~DeadTree();
	void initialize();
	void update();
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generate(D3DXVECTOR3 position);
};

