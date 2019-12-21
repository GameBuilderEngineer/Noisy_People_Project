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
	const int NUM_WIND = 5;				// 風オブジェクトの数

	// 座標
	const D3DXVECTOR3 POSITION[NUM_WIND] =
	{
		D3DXVECTOR3(234.0f, 13.0f, -178.0f),	// 東の台地の穴の中
		D3DXVECTOR3(179.0f, 13.0f, -26.0f),		// 東のジャンプするとこ
		D3DXVECTOR3(58.0f, 34.0f, 158.0f),		// 北のジャンプするとこ
		D3DXVECTOR3(-114.0f, 60.0f, 187.0f),	// 北の台地にあがるとこ
		D3DXVECTOR3(369.0f, 81.0f, -102.0f),	// 東の小島
	};

	// 衝突判定球の大きさ（= オブジェクトの大きさ）
	const float SCALE[NUM_WIND] =
	{
		3.0f,
		3.0f,
		3.0f,
		3.0f,
		3.0f
	};

	// 風の方向
	const D3DXVECTOR3 DIRECTION[NUM_WIND] =
	{
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
	};

	// 風の飛距離
	const float DISTANCE[NUM_WIND] =
	{
		8.0f, 
		6.0f,
		4.0f,
		6.0f,
		8.0f,
	};

	// 風の速度
	const float SPEED[NUM_WIND]
	{
		2.0f * 60,
		1.0f * 60,
		1.0f * 60,
		1.0f * 60,
		1.5f * 60,
	};

	const float EFFECT_RENDERING_DISTANCE = 200.0f;	// エフェクト描画距離
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
		effekseerNS::Instance* effect;
		BoundingSphere sphere;
	};

	// 風に当たっているかのフラグ
	bool isPlayerWindReceive[gameMasterNS::PLAYER_NUM];

	Wind wind[windNS::NUM_WIND];		// 風オブジェクト
	Ray ray;							// 共用レイ
	Player* player;						// プレイヤー


public:
	void initialize(Player* _player);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void windCollision(Player* _player);
};