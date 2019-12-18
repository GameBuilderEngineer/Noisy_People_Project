//-----------------------------------------------------------------------------
// 風クラス [Wind.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/11/11
//-----------------------------------------------------------------------------
#include "Wind.h"
#include "Input.h"
using namespace windNS;


//=============================================================================
// 初期化
//=============================================================================
void WindManager::initialize(std::string _sceneName, LPD3DXMESH _attractorMesh,
	D3DXMATRIX* _attractorMatrix)
{
	// 風オブジェクトを初期化
	for (int i = 0; i < NUM_WIND; i++)
	{
		wind[i].windDirection = DIRECTION[i];
		wind[i].windSpeed = SPEED[i];
		// オブジェクトのマトリクスを衝突判定に使う
		// wind自体のスケールと球のスケールをそろえるとデバッグ描画と衝突判定が一致する
		wind[i].scale *= SCALE[i];
		wind[i].sphere.initialize(&wind[i].position, 1.0f);
		wind[i].sphere.setScale(SCALE[i]);
		wind[i].initialize((D3DXVECTOR3*)&POSITION[i]);

		// エフェクトを起動しておく
		wind[i].effect = new effekseerNS::Instance(0, effekseerNS::WIND);
		wind[i].effect->position = wind[i].position;
		wind[i].effect->scale = D3DXVECTOR3(3.5f, 7.0f, 3.5f);
		effekseerNS::play(0, wind[i].effect);
	}
}


//=============================================================================
// 終了処理
//=============================================================================
void WindManager::uninitialize()
{
}


//=============================================================================
// 更新処理
//=============================================================================
void WindManager::update(float frameTime)
{

}


//=============================================================================
// 描画処理
//=============================================================================
void WindManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#ifdef _DEBUG
	for (int i = 0; i < NUM_WIND; i++)
	{
		wind[i].sphere.render();
	}
#endif
}


//=============================================================================
// 風との衝突処理
//=============================================================================
void  WindManager::windCollision(Player* player)
{
	float distance = 0.0f;

	for (int i = 0; i < NUM_WIND; i++)
	{
#if 0 // この処理を行うと他のエフェクトでバグる.何故か分からない.
		// エフェクトを描画するか判断する
		float temp1 = between2VectorLength(wind[i].position, player[0].center);
		float temp2 = between2VectorLength(wind[i].position, player[1].center);
		if (temp1 > EFFECT_RENDERING_DISTANCE && temp2 > EFFECT_RENDERING_DISTANCE)
		{
			wind[i].effect->setShown(false);
			continue;
		}
		else
		{
			wind[i].effect->setShown(true);
		}
#endif

		// 衝突判定
		for (int k = 0; k < playerNS::NUM_PLAYER; k++)
		{
			distance = between2VectorLength(wind[i].position, player[k].center);
			if (distance > DISTANCE[i]) { continue; }

			ray.update(player[k].center, -wind[i].windDirection);
			if (ray.rayIntersect(wind[i].sphere.getMesh(), wind[i].matrixWorld))
			{
				player[k].jump();
				player[k].addSpeed(wind[i].windDirection * wind[i].windSpeed);
			}
		}
	}

}
