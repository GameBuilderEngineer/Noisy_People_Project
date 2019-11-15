//===================================================================================================================================
//【BoundingSphere.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/24
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//【バウンディングスフィアクラス：コライダー】
//===================================================================================================================================
class BoundingSphere : public Base
{
private:
	//Data
	D3DXVECTOR3*	position;	//位置
	float			radius;		//半径
	D3DXVECTOR3		center;		//中心
	float			scale;		//スケール
	LPD3DXMESH		mesh;		//メッシュ

public:
	//Method
	BoundingSphere();
	BoundingSphere(D3DXVECTOR3*	position,float radius);
	~BoundingSphere();
	void initialize(D3DXVECTOR3* position, LPD3DXMESH ownerMesh);
	void initialize(D3DXVECTOR3* position, float _raidus);
	void render(D3DXMATRIX owner);
	void render();
	bool collide(D3DXVECTOR3 targetCenter,float targetRadius, D3DXMATRIX ownerMatrix, D3DXMATRIX targetMatrix);

	//setter
	void setScale(float value);

	//getter
	float getRadius();
	D3DXVECTOR3 getCenter();
	LPD3DXMESH	getMesh();
};