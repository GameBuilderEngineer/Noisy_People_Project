//-----------------------------------------------------------------------------
// パワーアップクラス [Powerup.h]
// Author : 新里　将士
// 作成開始日 : 2020/ 01/14  
//-----------------------------------------------------------------------------
#include "Powerup.h"
#include "ImguiManager.h"
using namespace itemNS;

//=============================================================================
// コンストラクタ
//=============================================================================
Powerup::Powerup(StaticMesh* _staticMesh, ItemData _itemData): Item(_staticMesh, _itemData)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
Powerup::~Powerup()
{
}

//初期化
void Powerup::initialize()
{
	flag = true;
	ray.initialize(position, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	{
		fieldFacePosition = position + ray.direction * ray.distance;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Powerup::update(float frameTime)
{
	// 
	Item::update(frameTime);
	if (flag == false) { return; }

	if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	{

		if (position.y > fieldFacePosition.y)
		{
			position += D3DXVECTOR3(0.0f, -1.0f, 0.0f) * 30.0f * frameTime;
		}
		else
		{
			position = fieldFacePosition;
			flag = false;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Powerup::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{

}