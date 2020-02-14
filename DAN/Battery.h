//-----------------------------------------------------------------------------
// バッテリークラス [Battery.h]
// Author : 新里
// 作成開始日 : 2019/10/15  
//-----------------------------------------------------------------------------
#pragma once
#include"Item.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace batteryNS
{
	const int RECOVERY_POWER = 85;			//電力回復量
}

//=============================================================================
//クラス定義
//=============================================================================
class Battery: public Item
{
private:
	static StaticMesh* staticMesh;

public:
	Battery(StaticMesh* _staticMesh, itemNS::ItemData _itemData);
	static void initialize(StaticMesh* staticMesh);
	virtual void update(float frameTime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	// Getter

	// Setter
};
