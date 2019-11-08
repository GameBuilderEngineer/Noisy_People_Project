//===================================================================================================================================
//【Reticle.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/07
// [更新日]2019/11/07
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Sprite.h"
#include "InstancingBillboard.h"
#include "Ray.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace reticleNS
{
	//レティクル情報の定義
	const int	NUM_U			= 5;
	const int	NUM_V			= 3;
	const float UNIT_U			= 1.0f/NUM_U;//単位U
	const float UNIT_V			= 1.0f/NUM_V;//単位V
	const int WIDTH				= 1250;//テクスチャ全体の幅
	const int HEIGHT			= 750;//テクスチャ全体の高さ	
	const int UNIT_WIDTH		= (int)(WIDTH/NUM_U);//単位幅
	const int UNIT_HEIGHT		= (int)(HEIGHT/NUM_V);//単位高さ

	const D3DXVECTOR3 POSITION1 = D3DXVECTOR3((float)WINDOW_WIDTH / 4, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DXVECTOR3 POSITION2 = D3DXVECTOR3((float)WINDOW_WIDTH / 4 *3, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);

	class Reticle3D :public InstancingBillboardNS::Instance
	{
	public:
		//コンストラクタ
		Reticle3D(D3DXVECTOR3 position,D3DXVECTOR2 uv);
		//更新
		virtual void update(float frameTime)override;
	};

}

//===================================================================================================================================
//【レティクルクラス】
//===================================================================================================================================
class Reticle :	public Base
{
private:
	D3DXVECTOR2				uv[reticleNS::NUM_U][reticleNS::NUM_V];
	Sprite*					reticle2D;
	InstancingBillboard*	billboard;

	//レティクルレイ
	D3DXVECTOR3*			cameraPosition;
	D3DXVECTOR3*			aimingPosition1;
	D3DXVECTOR3*			aimingPosition2;
	float					collideDistance;

	int currentU;
	int currentV;
	int type;

public:
	Reticle();
	~Reticle();
	
	void setCameraPosition(D3DXVECTOR3* position);
	void setAimingPosition1(D3DXVECTOR3* position);
	void setAimingPosition2(D3DXVECTOR3* position);
	void setAnime();

	void update(float frameTime);
	void render3D(int playerType,D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void render2D();
};

