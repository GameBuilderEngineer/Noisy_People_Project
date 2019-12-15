//-----------------------------------------------------------------------------
// タイガーバレット [TigerBullet.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/10
//-----------------------------------------------------------------------------
#include "TigerBullet.h"

//=============================================================================
// コンストラクタ
//=============================================================================
TigerBullet::TigerBullet(Ray shootingRay)
{
	launchPosition = shootingRay.start;
	bulletSpeed = shootingRay.direction * SPEED;
	//initialCollide = launchPosition + shootingRay.direction * shootingRay.distance;

	// 弾道の初期化
	ballisticRay.initialize(launchPosition, shootingRay.direction);
	ballisticRay.color = D3DXCOLOR(0, 255, 120, 255);

	{// オブジェクトタイプと衝突対象の指定
		using namespace ObjectType;
		treeCell.type = BULLET;
		treeCell.target = PLAYER;
	}

	postureControl(axisZ.direction, shootingRay.direction, 1.0f);	// モデルを進行方向へ姿勢制御する
	existenceTimer = EXIST_TIME;									// 存在時間
	endPoint = launchPosition + bulletSpeed * EXIST_TIME;			// 何にも衝突しなかった場合の終着位置
	isHit = false;													// 衝突フラグをオフ
	Object::initialize(&launchPosition);
}


//=============================================================================
// デストラクタ
//=============================================================================
TigerBullet::~TigerBullet()
{

}


//=============================================================================
// 更新
//=============================================================================
void TigerBullet::update(float frameTime)
{
	if (existenceTimer <= 0)
	{
		return;
	}
	existenceTimer -= frameTime;

	// 位置更新
	D3DXVec3Lerp(&position, &ballisticRay.start, &endPoint, 1.0f - existenceTimer / EXIST_TIME);
	Object::update();

	// 前フレームの衝突判定で衝突していればtrue
	if (isHit)
	{
		hit();
	}
}


//=============================================================================
// 描画
//=============================================================================
void TigerBullet::render()
{
	//float length = Base::between2VectorLength(ballisticRay.start, position);
	//ballisticRay.render(length);
	//debugRender();
}


//=============================================================================
// 衝突判定
//=============================================================================
bool TigerBullet::collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix)
{
	// 弾道レイの書き換え防止
	Ray ray;
	ray.initialize(ballisticRay.start, ballisticRay.direction);

	// レイ判定
	bool hit = ray.rayIntersect(targetMesh, targetMatrix);

	if (hit)
	{
		// 距離判定
		hit = (Base::between2VectorLength(ballisticRay.start, position) > ray.distance);
	}

	return hit;
}


//=============================================================================
// 削除
//=============================================================================
void TigerBullet::hit()
{
	existenceTimer = 0.0f;

	//サウンドの再生
	PLAY_PARAMETERS hitSE;
	hitSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_HitBulletTree, false ,NULL,false,NULL };
	SoundInterface::SE->playSound(&hitSE);
	//エフェクトの再生
	effekseerNS::Instance* instance = new effekseerNS::Instance();
	instance->position = position;
	effekseerNS::play(0, instance);
}


//=============================================================================
// 消滅
//=============================================================================
//void TigerBullet::isHitBeforeDisappear()
//{
//
//}


//=============================================================================
// Getter
//=============================================================================


//=============================================================================
// Setter
////=============================================================================


///////////////////////////////////////////////////////////////////////////////
//////////////////////////// バレットマネージャー /////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//=============================================================================
// コンストラクタ
//=============================================================================
TigerBulletManager::TigerBulletManager(StaticMeshRenderer* _renderer)
{
	renderer = _renderer;
	remaining = MAGAZINE_NUM;
	intervalTimer = 0.0f;
	isShot = false;
}


//=============================================================================
// デストラクタ
//=============================================================================
TigerBulletManager::~TigerBulletManager()
{
	for (int i = 0; i < bulletList.nodeNum; i++)
	{
		destroy(*bulletList.getValue(i), i);
	}

	// バレットリストの終了処理
	bulletList.terminate();
}


//=============================================================================
// 更新
//=============================================================================
void TigerBulletManager::update(float frameTime)
{
	// 発射間隔時間の更新
	if (intervalTimer > 0.0f)
	{
		intervalTimer -= frameTime;
	}

	// バレットの更新
	for (int i = 0; i < bulletList.nodeNum; i++)
	{
		TigerBullet* bullet = *bulletList.getValue(i);
		bullet->update(frameTime);
	}

	// レンダラーの更新
	renderer->updateAccessList();

	// バレットの削除
	for (int i = 0; i < bulletList.nodeNum; i++)
	{
		TigerBullet* bullet = *bulletList.getValue(i);

		if (bullet->existenceTimer < 0.0f || bullet->getIsHit())
		{
			destroy(bullet, i);
		}
	}

	// リストの更新
	bulletList.listUpdate();
}


//=============================================================================
// 描画
//=============================================================================
void TigerBulletManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{

}


//=============================================================================
// 発射
//=============================================================================
bool TigerBulletManager::shoot(Ray shootingRay)
{
	// 発射間隔をあける
	if (intervalTimer > 0.0f)
	{
		return false;
	}

	// 残弾数と発射間隔の設定
	if (remaining <= 0)
	{
		remaining = MAGAZINE_NUM;
		intervalTimer = RELOAD_TIME;
		return false;
	}
	else
	{
		remaining--;
		intervalTimer = INTERVAL_TIME;
	}

	// バレットの作成と描画登録
	TigerBullet* bullet = new TigerBullet(shootingRay);
	bulletList.insertFront(bullet);
	bulletList.listUpdate();
	renderer->registerObject(bullet);
	renderer->updateAccessList();
}


//=============================================================================
// 弾の破棄
//=============================================================================
void TigerBulletManager::destroy(TigerBullet* bullet, int nodeNumber)
{
	// 描画を切る
	renderer->unRegisterObjectByID(bulletList.getNode(nodeNumber)->value->id);

	// ダブルポインタを渡してポインタノードを破棄
	bulletList.remove(bulletList.getNode(nodeNumber));

	// バレットを破棄
	SAFE_DELETE(bullet);
}