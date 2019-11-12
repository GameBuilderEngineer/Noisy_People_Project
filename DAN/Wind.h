//-----------------------------------------------------------------------------
// 風クラス [Wind.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/11/11
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"
#include "StaticMeshRenderer.h"
#include "Player.h"

namespace windNS
{
	const int NUM_WIND = 3;				// 風オブジェクトの数

	// 座標
	const D3DXVECTOR3 POSITION[NUM_WIND] =
	{
		D3DXVECTOR3(235.0f, 85.0f, -117.0f),
		D3DXVECTOR3(158.0f, 27.640f, 27.783f),
		D3DXVECTOR3(-64.0f, 11.145f, -190.0f),
	};

	// 風の方向
	const D3DXVECTOR3 DIRECTION[NUM_WIND] =
	{
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
	};

	// 風の飛距離
	const float DISTANCE[NUM_WIND] =
	{
		10.0f,
		10.0f,
		10.0f,
	};

	// 風の速度
	const float SPEED[NUM_WIND]
	{
		1.0f,
		1.0f,
		1.0f
	};
}


//=============================================================================
//クラス定義
//=============================================================================
class WindManager: public Base
{
private:

	// 風クラス
	class Wind: public Object
	{
	public:
		D3DXVECTOR3 windDirection;
		float windSpeed;
	};

	Wind wind[windNS::NUM_WIND];		// 風オブジェクト
	Ray ray;							// 共用レイ
	StaticMeshRenderer* windRenderer;	// 描画オブジェクト

public:
	void initialize(std::string _sceneName, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void windCollision(Player* player);
};