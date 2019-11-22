//===================================================================================================================================
//【Bullet.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/05
// [更新日]2019/11/11
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Bullet.h"
#include "UtilityFunction.h"
#include "EffekseerManager.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace bulletNS;

//////////////////////////////////////////////////////////////////////////////////////////////////////
// [バレットマネージャ]
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region Bullet
//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Bullet::Bullet(Ray shootingRay)
{
	//パラメータの初期化
	digitalPower = DIGITAL_POWER;

	//引数の代入
	this->launchPosition	= shootingRay.start;												//発射位置
	this->speed				= shootingRay.direction*SPEED;										//速度
	this->initialCollide	= launchPosition + shootingRay.direction * shootingRay.distance;	//初期衝突位置

	//弾道の初期化
	ballisticRay.initialize(launchPosition, shootingRay.direction);
	ballisticRay.color = D3DXCOLOR(0, 255, 120, 255);



	{//オブジェクトタイプと衝突対象の指定
		using namespace ObjectType;
		treeCell.type = BULLET;
		treeCell.target = ENEMY | TREE;
	}

	Object::initialize(&launchPosition);							//バレットモデルの初期化
	postureControl(axisZ.direction, shootingRay.direction,1.0f);	//モデルを進行方向へ姿勢制御する
	Object::update();
	existenceTimer = EXIST_TIME;
	endPoint = launchPosition + speed * EXIST_TIME;					//何にも衝突しなかった場合の終着位置
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Bullet::~Bullet()
{

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Bullet::update(float frameTime)
{
	if (existenceTimer <= 0)return;
	existenceTimer -= frameTime;
	//位置更新
	D3DXVec3Lerp(&position, &ballisticRay.start, &endPoint,1.0f - existenceTimer/EXIST_TIME);
	Object::update();
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Bullet::render()
{
	float length = Base::between2VectorLength(ballisticRay.start, position);
	ballisticRay.render(length);
	debugRender();
}

//===================================================================================================================================
//【衝突判定】
//===================================================================================================================================
bool Bullet::collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix)
{
	bool hit = ballisticRay.rayIntersect(targetMesh, targetMatrix);
	
	if (hit)
	{
		hit = (Base::between2VectorLength(ballisticRay.start, position)
		> ballisticRay.distance);
	}

	return hit;
}
//===================================================================================================================================
//【getter】
//===================================================================================================================================
int Bullet::getDigitalPower(){return digitalPower;}
bool Bullet::isCollideInitial() { 
	float initial = Base::between2VectorLength(ballisticRay.start, initialCollide);
	float now = Base::between2VectorLength(ballisticRay.start, position);
	return now >= initial; 
}
D3DXVECTOR3 Bullet::getBulletSpeed() { return this->speed; }


//===================================================================================================================================
//【削除】
//===================================================================================================================================
void Bullet::destroy()
{
	existenceTimer = 0.0f;
	//サウンドの再生
	FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	PLAY_PARAMETERS hitSE;
	hitSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_HitBulletTree, false ,NULL,false,NULL, true, filterParameters };
	SoundInterface::SE->playSound(&hitSE);
	//エフェクトの再生
	effekseerNS::Instance* instance = new effekseerNS::Instance();
	instance->position = position;
	effekseerNS::play(instance);

}
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////
// [バレットマネージャ]
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region BulletManager
//===================================================================================================================================
//【コンストラクタ：バレットマネージャー】
//===================================================================================================================================
BulletManager::BulletManager()
{
	bulletList		= new LinkedList<Bullet*>;
	renderer		= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
	remaining		= MAGAZINE_NUM;
	intervalTimer	= 0.0f;
	reloadTimer		= 0.0f;
	reloading		= false;
	isLaunched		= false;
	launchFactTime	= 0.0f;

	//サウンドの設定
	FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	shotSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Shot, false ,NULL,false,NULL, true, filterParameters };
	reroadSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Reload, false ,NULL,false,NULL, true, filterParameters };
}

//===================================================================================================================================
//【デストラクタ：バレットマネージャー】
//===================================================================================================================================
BulletManager::~BulletManager()
{
	//レンダラーの解除/終了
	renderer->allUnRegister();
	SAFE_DELETE(renderer);
	//バレットリストの削除
	bulletList->terminate();
	SAFE_DELETE(bulletList);
}

//===================================================================================================================================
//【更新：バレットマネージャー】
//===================================================================================================================================
void BulletManager::update(float frameTime)
{
	//インターバル時間
	if(intervalTimer > 0)intervalTimer -= frameTime;


	//各バレットの更新
	for (int i = 0; i < bulletList->nodeNum; i++)
	{
		//バレットのポインタ取得
		Bullet* bullet = (*bulletList->getValue(i));

		//更新
		bullet->update(frameTime);

		//生存時間切れ
		if (bullet->existenceTimer <= 0)
		{
			destroy(bullet, i);
		}
		//初期衝突位置
		else if(bullet->isCollideInitial())
		{
			bullet->destroy();
			destroy(bullet, i);
		}
	}
	//リストの更新[削除による対応]
	bulletList->listUpdate();


	//レンダラーの更新
	renderer->update();


	//リロード処理
	if (reloadTimer > 0)
	{
		reloadTimer -= frameTime;
	}
	else {
		if (reloading)
		{
			reloading = false;			//リロード終了
			remaining = MAGAZINE_NUM;	//装填
		}
	}

	//LAUNCH_FACT_TIME秒で発射事実を消す
	if (isLaunched)
	{
		launchFactTime += frameTime;
		if (launchFactTime > LAUNCH_FACT_TIME)
		{
			isLaunched = false;
		}
	}
}

//===================================================================================================================================
//【描画：バレットマネージャー】
//===================================================================================================================================
void BulletManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//弾道レイの描画
	for (int i = 0; i < bulletList->nodeNum; i++)
	{
		(*bulletList->getValue(i))->render();
	}
	renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}

//===================================================================================================================================
//【発射：バレットマネージャー】
//===================================================================================================================================
bool BulletManager::launch(Ray shootingRay)
{
	//インターバル中：発射しない
	if (intervalTimer > 0)return false;	
	
	//リロード中：発射しない
	if (reloading)return false;
	
	//残段数が0：自動リロード
	if (remaining <= 0)
	{
		reload();
		return false;
	}

	//バレットリストへ新たに生成
	Bullet* newBullet = new Bullet(shootingRay);

	//リストへ追加
	bulletList->insertFront(newBullet);
	
	//リストの更新[追加による対応]
	bulletList->listUpdate();

	//レンダラーへの登録
	renderer->registerObject(newBullet);

	//残段数の減算
	remaining--;

	//インターバル時間のセット
	intervalTimer = INTERVAL_TIME;

	//サウンドの再生
	SoundInterface::SE->playSound(&shotSE);

	//エフェクトの再生
	//effekseerNS::Instance* instance = new effekseerNS::Instance();
	//instance->position = newBullet->position;
	//effekseerNS::play(instance);

	//ゲーム中に発射事実を残す
	isLaunched = true;
	launchFactTime = 0.0f;

	return true;
}

//===================================================================================================================================
//【リロード：バレットマネージャー】
//===================================================================================================================================
void BulletManager::reload()
{
	//残段数が最大：リロードしない
	if (remaining >= MAGAZINE_NUM)return;
	//リロード中：リロードしない
	if (reloading)return;
	reloading = true;							//リロード開始
	reloadTimer = RELOAD_TIME;					//リロードタイムの設定
	SoundInterface::SE->playSound(&reroadSE);	//サウンドの再生

}

//===================================================================================================================================
//【弾削除：バレットマネージャー】
//===================================================================================================================================
void BulletManager::destroy(Bullet* bullet,int nodeNumber)
{
	//削除
	renderer->unRegisterObjectByID(bullet->id);//削除前にレンダラー解除
	SAFE_DELETE(bullet);
	bulletList->remove(bulletList->getNode(nodeNumber));
}

//===================================================================================================================================
//【getter：バレットマネージャー】
//===================================================================================================================================
int BulletManager::getRemaining(){return remaining;}
float BulletManager::getReloadTime() { return reloadTimer; }
Bullet* BulletManager::getBullet(int i) { return *bulletList->getValue(i); }
int BulletManager::getNum() { return bulletList->nodeNum; }
bool BulletManager::getIsLaunched() { return isLaunched; }
#pragma endregion
