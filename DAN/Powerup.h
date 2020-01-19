//-----------------------------------------------------------------------------
// パワーアップクラス [Powerup.h]
// Author : 新里　将士
// 作成開始日 : 2020/  01/14  
//-----------------------------------------------------------------------------
#pragma once
#include "Item.h"

class PowUpEffect :public effekseerNS::Instance
{
public:
	D3DXVECTOR3 * syncPosition;
	PowUpEffect(D3DXVECTOR3* sync, int _effectNo)
	{
		syncPosition = sync;
		effectNo = _effectNo;
	}
	virtual void update()
	{
		position = *syncPosition;
		Instance::update();
	};
};

class Powerup:public Item
{
	bool flag;
	Ray ray;
	D3DXVECTOR3 fieldFacePosition;
public:
	PowUpEffect* powUpEffect;
	Powerup(StaticMesh* _staticMesh, itemNS::ItemData _itemData);
	~Powerup();
	static void initialize(StaticMesh* staticMesh);
	void initialize();
	virtual void update(float frameTime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
};

