//===================================================================================================================================
//【Reticle.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/07
// [更新日]2019/11/07
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Reticle.h"
#include "TextureLoader.h"
#include "GameMaster.h"
#include "Player.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace reticleNS;

//===================================================================================================================================
//【インスタンスクラス：コンストラクタ】
//===================================================================================================================================
Reticle3D::Reticle3D(D3DXVECTOR3 position,D3DXVECTOR2 uv):InstancingBillboardNS::Instance()
{
	//初期値の設定
	this->position	= position;
	this->rotation	= D3DXVECTOR3(0, 0, 0);
	this->scale		= D3DXVECTOR2(1.0f,1.0f);
	this->speed		= D3DXVECTOR3(0, 0, 0);
	this->lifeTimer	= 0.0f;
	this->limitTime = 1.0f;
	this->uv		= uv;
}

//===================================================================================================================================
//【インスタンスクラス：更新】
//===================================================================================================================================
void Reticle3D::update(float frameTime)
{

}

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Reticle::Reticle()
{
	//レティクル2D
	{using namespace Reticle2DNS;
		//テクスチャに対するUV値の設定
		//UV値の設定
		for (int u = 0; u < NUM_U; u++)
		{
			for (int v = 0; v < NUM_V; v++)
			{
				uv[u][v] = D3DXVECTOR2(u*UNIT_U, v*UNIT_V);
			}
		}
		currentU = 0;
		currentV = 0;

		reticle2D = new Sprite();
		//UI情報の設定
		reticle2D->initialize(
			*textureNS::reference(textureNS::UI_SUB_RETICLE),	//テクスチャポインタ
			SpriteNS::CENTER,								//原点
			(int)((float)UNIT_WIDTH),						//幅
			(int)((float)UNIT_HEIGHT),					//高さ
			POSITION1,										//位置
			ROTATION,										//回転
			COLOR											//色
		);
		//描画UV値を設定
		reticle2D->setUVCoord(
			uv[0][0] + D3DXVECTOR2(0.0, 0.0),
			uv[0][0] + D3DXVECTOR2(UNIT_U, 0.0),
			uv[0][0] + D3DXVECTOR2(0.0, UNIT_V),
			uv[0][0] + D3DXVECTOR2(UNIT_U, UNIT_V));
		//残弾数
		for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		{
			remainingBullet[i] = 0;
		}
	}

	//リロード2D
	{using namespace Reload2DNS;
		//テクスチャに対するUV値の設定
		//UV値の設定
		for (int u = 0; u < NUM_U; u++)
		{
			for (int v = 0; v < NUM_V; v++)
			{
				uvReload[u][v] = D3DXVECTOR2(u*UNIT_U, v*UNIT_V);
			}
		}
		reload2D = new Sprite();
		reload2D->initialize(
			*textureNS::reference(textureNS::UI_RELOAD),		//テクスチャポインタ
			SpriteNS::CENTER,									//原点
			(int)((float)UNIT_WIDTH),							//幅
			(int)((float)UNIT_HEIGHT),							//高さ
			POSITION1,											//位置
			ROTATION,											//回転
			COLOR												//色
		);
		//描画UV値を設定
		reticle2D->setUVCoord(
			uv[0][0] + D3DXVECTOR2(0.0, 0.0),
			uv[0][0] + D3DXVECTOR2(UNIT_U, 0.0),
			uv[0][0] + D3DXVECTOR2(0.0, UNIT_V),
			uv[0][0] + D3DXVECTOR2(UNIT_U, UNIT_V));
	}

	//レティクル3D
	{using namespace Reticle3DNS;
		billboard = new InstancingBillboard();
		//billboard->initialize(*textureNS::reference(textureNS::UI_MAIN_RETICLE),NUM_U-1,NUM_V-1);
		billboard->initialize(*textureNS::reference(textureNS::UI_MAIN_RETICLE));
		billboard->generateInstance(new Reticle3D(D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(UNIT_U * 0, UNIT_V * 0)));
		billboard->setRenderType(InstancingBillboardNS::FOREGROUND_PASS);
	}

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Reticle::~Reticle()
{
	SAFE_DELETE(reticle2D);
	SAFE_DELETE(reload2D);
	SAFE_DELETE(energyEmpty2D);
	SAFE_DELETE(billboard);
}

//===================================================================================================================================
//【テクスチャ位置のセット】
//===================================================================================================================================
void Reticle::setAnime(int playerState)
{
	switch(playerState)
	{
	case playerNS::STATE::VISION:
	case playerNS::STATE::SKY_VISION:
	case playerNS::STATE::DIGITAL_SHIFT:
		reticle2D->setUVCoord(
			D3DXVECTOR2(0.0, 0.0),
			D3DXVECTOR2(1.0f, 0.0),
			D3DXVECTOR2(0.0, 1.0f),
			D3DXVECTOR2(1.0f, 1.0f));

		break;
	case playerNS::STATE::NORMAL:
	case playerNS::STATE::DEATH:
		using namespace Reticle2DNS;
		reticle2D->setUVCoord(
			uv[currentU][currentV] + D3DXVECTOR2(0.0, 0.0),
			uv[currentU][currentV] + D3DXVECTOR2(UNIT_U, 0.0),
			uv[currentU][currentV] + D3DXVECTOR2(0.0, UNIT_V),
			uv[currentU][currentV] + D3DXVECTOR2(UNIT_U, UNIT_V));
		break;
	}
}

//===================================================================================================================================
//【残弾数をセット】
//===================================================================================================================================
void Reticle::setRemainingBullet(int num,int playerNo)
{
	remainingBullet[playerNo] = num;
}

//===================================================================================================================================
//【カメラポジションのポインタをセット】
//===================================================================================================================================
void Reticle::setCameraPosition(D3DXVECTOR3* position)
{
	cameraPosition = position;
}

//===================================================================================================================================
//【1P照準位置のポインタをセット】
//===================================================================================================================================
void Reticle::setAimingPosition1(D3DXVECTOR3* position)
{
	aimingPosition1 = position;
}

//===================================================================================================================================
//【2P照準位置のポインタをセット】
//===================================================================================================================================
void Reticle::setAimingPosition2(D3DXVECTOR3* position)
{
	aimingPosition2 = position;
}

//===================================================================================================================================
//【更新する】
//===================================================================================================================================
void Reticle::update(float frameTime)
{


	billboard->update(frameTime);
}

//===================================================================================================================================
//【３Ｄ情報の描画】
//===================================================================================================================================
void Reticle::render3D(int playerType,D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	
	//描画位置の設定
	switch (playerType)
	{
	case gameMasterNS::PLAYER_1P:(*billboard->getList().getFrontValue())->position = *aimingPosition1; break;
	case gameMasterNS::PLAYER_2P:(*billboard->getList().getFrontValue())->position = *aimingPosition2; break;
	}
	billboard->update(0);
	billboard->render(view, projection, cameraPositon);				//描画


}

//===================================================================================================================================
//【２Ｄ情報の描画】
//===================================================================================================================================
void Reticle::render2D(Player* player)
{
	//プレイヤー番号
	int playerNo = player->getInfomation()->playerType;

	//リロード中判定
	bool onReload = false;

	//描画するテクスチャの切り替え
	switch (player->getState())
	{
	case playerNS::STATE::VISION: 
	case playerNS::STATE::SKY_VISION:
	case playerNS::STATE::DIGITAL_SHIFT:
		//シフトできるかどうかのレティクルへの反映
		if (player->whetherValidOperation(playerNS::ENABLE_SHIFT))
		{
			reticle2D->setTexturePointer(*textureNS::reference(textureNS::UI_MAIN_DIGITAL_RETICLE));
		}
		else {
			reticle2D->setTexturePointer(*textureNS::reference(textureNS::UI_SUB_DIGITAL_RETICLE));
		}
		break;

	case playerNS::STATE::NORMAL:
	case playerNS::STATE::DEATH:
		{
			//2Dレティクル
			{
				using namespace Reticle2DNS;
				int bulletNum = player->getBulletManager()->getRemaining();
				setRemainingBullet(bulletNum, playerNo);
				currentU = remainingBullet[playerNo] % NUM_U;
				currentV = remainingBullet[playerNo] / NUM_U;
				reticle2D->setTexturePointer(*textureNS::reference(textureNS::UI_SUB_RETICLE));
			}

			//リロード中表示
			{
				using namespace Reload2DNS;
				float reloadTime = player->getBulletManager()->getReloadTime();
				//リロード中
				if (reloadTime > 0) 
				{ 
					onReload = true; 
					float rate = 1.0f - reloadTime / bulletNS::RELOAD_TIME;
					if (rate > 0)
					{
						int a = 0;
					}
					int animeNum = (int)(rate * (float)NUM_U * (float)NUM_V);
					reloadU = animeNum % NUM_U;
					reloadV = animeNum / NUM_U;
					reload2D->setUVCoord(
						uvReload[reloadU][reloadV] + D3DXVECTOR2(0.0, 0.0),
						uvReload[reloadU][reloadV] + D3DXVECTOR2(UNIT_U, 0.0),
						uvReload[reloadU][reloadV] + D3DXVECTOR2(0.0, UNIT_V),
						uvReload[reloadU][reloadV] + D3DXVECTOR2(UNIT_U, UNIT_V));
				}
			}
			break;
		}
	}

	//UV値の設定
	setAnime(player->getState());

	//描画位置
	switch (player->getInfomation()->playerType)
	{
		using namespace Reticle2DNS;
	//1P
	case gameMasterNS::PLAYER_1P:
		reticle2D->setPosition(POSITION1);
		if (onReload)reload2D->setPosition(POSITION1);
		break;
	//2P
	case gameMasterNS::PLAYER_2P:
		reticle2D->setPosition(POSITION2);
		if (onReload)reload2D->setPosition(POSITION2);
		break;
	} 

	//描画
	reticle2D->setVertex();
	reticle2D->render();
	//描画
	if (onReload)
	{
		reload2D->setVertex();
		reload2D->render();
	}
}

