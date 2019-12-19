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
#include "Player.h"
#include "Camera.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace reticleNS
{
	//2Dレティクル情報の定義
	namespace Reticle2DNS{
		const int	NUM_U			= 5;
		const int	NUM_V			= 2;
		const float UNIT_U			= 1.0f/NUM_U;//単位U
		const float UNIT_V			= 1.0f/NUM_V;//単位V
		const int WIDTH				= 640;//テクスチャ全体の幅
		const int HEIGHT			= 256;//テクスチャ全体の高さ	
		const int UNIT_WIDTH		= (int)(WIDTH/NUM_U);//単位幅
		const int UNIT_HEIGHT		= (int)(HEIGHT/NUM_V);//単位高さ
		const D3DXVECTOR3 POSITION1 = D3DXVECTOR3((float)WINDOW_WIDTH / 4, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 POSITION2 = D3DXVECTOR3((float)WINDOW_WIDTH / 4 *3, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 196);
	}

	//2Dリロード表示情報の定義
	namespace Reload2DNS{
		const int	NUM_U			= 10;
		const int	NUM_V			= 5;
		const float UNIT_U			= 1.0f/NUM_U;//単位U
		const float UNIT_V			= 1.0f/NUM_V;//単位V
		const int WIDTH				= 1280;//テクスチャ全体の幅
		const int HEIGHT			= 640;//テクスチャ全体の高さ	
		const int UNIT_WIDTH		= (int)(WIDTH/NUM_U);//単位幅
		const int UNIT_HEIGHT		= (int)(HEIGHT/NUM_V);//単位高さ
		const D3DXVECTOR3 POSITION1 = D3DXVECTOR3((float)WINDOW_WIDTH / 4, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 POSITION2 = D3DXVECTOR3((float)WINDOW_WIDTH / 4 *3, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 ROTATION	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		const D3DCOLOR COLOR		= D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//2D電力不足表示情報の定義
	namespace EnergyEmpty2DNS{
		const int	NUM_U			= 1;
		const int	NUM_V			= 1;
		const float UNIT_U			= 1.0f/NUM_U;//単位U
		const float UNIT_V			= 1.0f/NUM_V;//単位V
		const int WIDTH				= 256;//テクスチャ全体の幅
		const int HEIGHT			= 128;//テクスチャ全体の高さ	
		const int UNIT_WIDTH		= (int)(WIDTH/NUM_U);//単位幅
		const int UNIT_HEIGHT		= (int)(HEIGHT/NUM_V);//単位高さ
		const D3DXVECTOR3 POSITION1 = D3DXVECTOR3((float)WINDOW_WIDTH / 4, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 POSITION2 = D3DXVECTOR3((float)WINDOW_WIDTH / 4 *3, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 ROTATION	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		const D3DCOLOR COLOR		= D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//3Dレティクル情報
	namespace Reticle3DNS {
		const int	NUM_U			= 1;
		const int	NUM_V			= 1;
		const float UNIT_U			= 1.0f / NUM_U;//単位U
		const float UNIT_V			= 1.0f / NUM_V;//単位V
		const int WIDTH				= 128;//テクスチャ全体の幅
		const int HEIGHT			= 128;//テクスチャ全体の高さ	
		const int UNIT_WIDTH		= (int)(WIDTH / NUM_U);//単位幅
		const int UNIT_HEIGHT		= (int)(HEIGHT / NUM_V);//単位高さ
	}



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
	//2D
	//レティクル
	D3DXVECTOR2				uv[reticleNS::Reticle2DNS::NUM_U][reticleNS::Reticle2DNS::NUM_V];
	Sprite*					reticle2D;
	int currentU;
	int currentV;
	int type;
	int remainingBullet[gameMasterNS::PLAYER_NUM];//残弾数

	//リロード
	D3DXVECTOR2				uvReload[reticleNS::Reload2DNS::NUM_U][reticleNS::Reload2DNS::NUM_V];
	Sprite*					reload2D;
	int						reloadU;
	int						reloadV;


	//電力不足表示
	Sprite*					energyEmpty2D;



	//3D
	InstancingBillboard*	billboard;
	//レティクルレイ
	D3DXVECTOR3*			cameraPosition;
	D3DXVECTOR3*			aimingPosition1;
	D3DXVECTOR3*			aimingPosition2;
	float					collideDistance;


public:
	Reticle();
	~Reticle();
	
	void setCameraPosition(D3DXVECTOR3* position);
	void setAimingPosition1(D3DXVECTOR3* position);
	void setAimingPosition2(D3DXVECTOR3* position);
	void setAnime(int playerState);
	void setRemainingBullet(int num, int playerNo);//残弾数

	void update(float frameTime);
	void render3D(int playerType,Camera* camera);
	void render2D(Player* player);
};
