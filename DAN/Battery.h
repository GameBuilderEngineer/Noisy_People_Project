//-----------------------------------------------------------------------------
// バッテリークラス [Battery.h]
// Author : 
// 作成開始日 : 2019/  /  
//-----------------------------------------------------------------------------
#pragma once
#include"Item.h"


//=============================================================================
//クラス定義
//=============================================================================
class Battery: public Item
{
private:
	static StaticMesh* staticMesh;

public:
	Battery(StaticMesh* _staticMesh, itemNS::ItemData* _itemData);
	static void initialize(StaticMesh* staticMesh);
	virtual void update(float frameTime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	//bool sphereCollide(D3DXVECTOR3 targetCenter, float targetRadius, D3DXMATRIX targetMatrix) override;

	// Getter

	// Setter
};
