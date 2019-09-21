//===================================================================================================================================
//yRay.hz
// [ì¬Ò]HALGP12A332 11 ì ÷
// [ì¬ú]2019/09/20
// [XVú]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//yCN[hz
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//yCNXFRC_[z
//===================================================================================================================================
class Ray : public Base
{
public:
	//Data
	D3DXVECTOR3 start;		//n_
	D3DXVECTOR3 direction;	//ûü

	float distance;			//ÕËÎÛÆÌ£
	D3DXVECTOR3 normal;		//ÕËÎÛÌ@ü
#ifdef _DEBUG
	D3DXCOLOR color;		//`æF
#endif // _DEBUG

	//Method
	Ray();
	~Ray();
	void initialize(D3DXVECTOR3 _start,D3DXVECTOR3 _direction);
	void update(D3DXVECTOR3 _start,D3DXVECTOR3 _direction);
	void render(LPDIRECT3DDEVICE9 device,float length);
	
	bool rayIntersect(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix);
	D3DXVECTOR3 slip(D3DXVECTOR3 L, D3DXVECTOR3 N);
	//HRESULT findVerticesOnPoly(LPD3DXMESH, DWORD, D3DXVECTOR3*);
};

