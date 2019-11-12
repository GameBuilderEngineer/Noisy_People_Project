//-----------------------------------------------------------------------------
// 風クラス [Wind.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/11/11
//-----------------------------------------------------------------------------
#include "Wind.h"
using namespace windNS;


//=============================================================================
// 初期化
//=============================================================================
void WindManager::initialize(std::string _sceneName, LPD3DXMESH _attractorMesh,
	D3DXMATRIX* _attractorMatrix)
{
	// 描画オブジェクトの作成
	windRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON_X10));

	// 風オブジェクトを初期化
	for (int i = 0; i < NUM_WIND; i++)
	{
		wind[i].initialize((D3DXVECTOR3*)&POSITION[i]);
		wind[i].windDirection = DIRECTION[i];
		wind[i].windSpeed = SPEED[i];
		windRenderer->registerObject(&wind[i]);
		wind[i].scale *= 0.2f;
	}
}


//=============================================================================
// 終了処理
//=============================================================================
void WindManager::uninitialize()
{
	SAFE_DELETE(windRenderer);
}


//=============================================================================
// 更新処理
//=============================================================================
void WindManager::update(float frameTime)
{
	for (int i = 0; i < NUM_WIND; i++)
	{
		wind[i].update();
	}

	windRenderer->update();
}


//=============================================================================
// 描画処理
//=============================================================================
void WindManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	windRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// 風との衝突処理
//=============================================================================
void  WindManager::windCollision(Player* player)
{
	float distance = 0.0f;

	for (int i = 0; i < NUM_WIND; i++)
	{
		distance = between2VectorLength(wind[i].position, *player->getCentralPosition());
		if (distance > DISTANCE[i]) { continue; }

		ray.update(player->position, -wind[i].windDirection);
		if (ray.rayIntersect(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON_X10)->mesh,
			wind[i].matrixWorld))
		{
			player->addSpeed(wind[i].windDirection * wind[i].windSpeed);
		}
	}
}
