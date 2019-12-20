//===================================================================================================================================
//【Marker.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/12/17
// [更新日]2019/12/17
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Marker.h"
#include "TextureLoader.h"
#include "UtilityFunction.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace MarkerNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Marker::Marker(int type)
{
	sprite = new Sprite;
		
	switch (type)
	{
	case PLAYER1:
		{
			using namespace PlayerMarkerNS;
			sprite->initialize(
				*textureNS::reference(textureNS::MARK_2D_MALE),		//テクスチャポインタ
				SpriteNS::CENTER,									//原点
				WIDTH,												//幅
				HEIGHT,												//高さ
				POSITION,											//位置
				ROTATION,											//回転
				COLOR												//色
			);
			break;
		}

	case PLAYER2:
		{
			using namespace PlayerMarkerNS;
			sprite->initialize(
				*textureNS::reference(textureNS::MARK_2D_FEMALE),	//テクスチャポインタ
				SpriteNS::CENTER,									//原点
				WIDTH,												//幅
				HEIGHT,												//高さ
				POSITION,											//位置
				ROTATION,											//回転
				COLOR												//色
			);
			break;
		}

	case BOSS_ENEMY:
		{
			using namespace BossEnemyMarkerNS;
			sprite->initialize(
				*textureNS::reference(textureNS::MARK_BOSS),		//テクスチャポインタ
				SpriteNS::CENTER,									//原点
				WIDTH,												//幅
				HEIGHT,												//高さ
				POSITION,											//位置
				ROTATION,											//回転
				COLOR												//色
			);
			break;
		}

	case ATTACKED_TREE:	
		{
			using namespace AttackedTreeNS;
			sprite->initialize(
				*textureNS::reference(textureNS::LIGHT_001),		//テクスチャポインタ
				SpriteNS::CENTER,									//原点
				WIDTH,												//幅
				HEIGHT,												//高さ
				POSITION,											//位置
				ROTATION,											//回転
				COLOR												//色
			);
			break;
		}
	}

	scale = D3DXVECTOR2(0.05f, 0.05f);


}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Marker::~Marker()
{
	SAFE_DELETE(sprite);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Marker::update(float frameTime)
{

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Marker::render(D3DXVECTOR3 position)
{
	sprite->setPosition(position);
	sprite->setVertex();
	sprite->render();
}



//レンダラー
//===================================================================================================================================
//【マーカーレンダラー：コンストラクタ】
//===================================================================================================================================
MarkerRenderer::MarkerRenderer()
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		playerPosition[i] = NULL;
	}
	bossEnemyPosition		= NULL;
	attackedTree			= NULL;


	//インスタンシングビルボードの初期化
	billboard = new InstancingBillboard();
	billboard->initialize(*textureNS::reference(textureNS::COLOR_SCALE),7,7);
	billboard->setRenderType(InstancingBillboardNS::FOREGROUND_PASS| InstancingBillboardNS::FIXED_SIZE_PASS);

	//スプライトの初期化
	for (int i = 0; i < MARKER_NUM; i++)
	{
		marker[i] = new Marker(i);
		billboard->generateInstance(marker[i]);//インスタントとして登録
	}
}

//===================================================================================================================================
//【マーカーレンダラー：デストラクタ】
//===================================================================================================================================
MarkerRenderer::~MarkerRenderer()
{
	for (int i = 0; i < MARKER_NUM; i++)
	{
		SAFE_DELETE(marker[i]);
	}
	SAFE_DELETE(billboard);
}

//===================================================================================================================================
//【マーカーレンダラー：更新】
//===================================================================================================================================
void MarkerRenderer::update(float frameTime)
{
	//マーカースプライトの更新
	for (int i = 0; i < MARKER_NUM; i++)
	{
		marker[i]->update(frameTime);
	}

	//インスタンシングビルボードの更新
	billboard->update(frameTime);
}

//===================================================================================================================================
//【マーカーレンダラー：描画】
//===================================================================================================================================
void MarkerRenderer::render(int playerNo, Camera* camera)
{

	D3DXVECTOR3 center;
	float		radius;
	float length = sqrtf((float)(WINDOW_WIDTH*WINDOW_WIDTH + WINDOW_HEIGHT * WINDOW_HEIGHT));
	float xLimit = (float)(WINDOW_WIDTH / 4 - PlayerMarkerNS::WIDTH / 2);
	float yLimit = (float)(WINDOW_HEIGHT / 2 - PlayerMarkerNS::HEIGHT / 2);

	//プレイヤーマーカー（どちらか一方のみを描画）
	switch(playerNo)
	{
	case gameMasterNS::PLAYER_1P:
		//自身のビルボードマーカーは描画しない
		marker[PLAYER1]->color = D3DCOLOR_RGBA(255, 255, 255, 0);
		//ターゲットが存在するかどうか
		if (playerPosition[gameMasterNS::PLAYER_2P]) {
			center = *playerPosition[gameMasterNS::PLAYER_2P];
			radius = 1.0f;
		}
		else
		{
			marker[PLAYER2]->color = D3DCOLOR_RGBA(255, 255, 255, 0);	//ビルボード描画しない
			break;
		}

		//カリング判定
		if (UtilityFunction::culling(
			center, radius, camera->view, camera->fieldOfView,
			camera->nearZ, camera->farZ, camera->aspect))
		{
			//視野内：ビルボードによる描画
			marker[PLAYER2]->position = center+D3DXVECTOR3(0.0f,1.0f,0.0f);
			marker[PLAYER2]->color = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else {
			//視野外：スプライトによる描画

			//ビルボードとしては描画しない
			marker[PLAYER2]->color = D3DCOLOR_RGBA(255, 255, 255, 0);

			//描画位置を求める
			D3DXVECTOR3 renderPosition;
			renderPosition = CENTER1P;

			//3D空間上の方向を、2D平面座標へ変換する
			D3DXVECTOR3 addVector = conversion2D(camera, center);
			addVector *= length;
			addVector.x = UtilityFunction::clamp(addVector.x, -xLimit, xLimit);
			addVector.y = UtilityFunction::clamp(addVector.y, -yLimit, yLimit);

			//描画位置へ加算
			renderPosition += addVector;

			//描画
			marker[PLAYER2]->render(renderPosition);
		}
		break;
	case gameMasterNS::PLAYER_2P:
		//自身のビルボードマーカーは描画しない
		marker[PLAYER2]->color = D3DCOLOR_RGBA(255, 255, 255, 0);
		//ターゲットが存在するかどうか
		if (playerPosition[gameMasterNS::PLAYER_1P]) {
			center = *playerPosition[gameMasterNS::PLAYER_1P];
		}
		else
		{
			marker[PLAYER1]->color = D3DCOLOR_RGBA(255, 255, 255, 0);	//ビルボード描画しない
			break;
		}

		radius = 1.0f;
		//カリング判定
		if (UtilityFunction::culling(
			center, radius, camera->view, camera->fieldOfView,
			camera->nearZ, camera->farZ, camera->aspect))
		{
			//視野内：ビルボードによる描画
			marker[PLAYER1]->position = center;
			marker[PLAYER1]->color = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else {
			//視野外：スプライトによる描画

			//ビルボードとしては描画しない
			marker[PLAYER1]->color = D3DCOLOR_RGBA(255, 255, 255, 0);

			//カメラに対する対象の方向を求める
			D3DXVECTOR3 direction;
			Base::between2VectorDirection(&direction, camera->position, center);
			direction.y = 0;//Y成分削除
			D3DXVec3Normalize(&direction, &direction);//正規化

			//描画位置を求める
			D3DXVECTOR3 renderPosition;
			renderPosition = CENTER2P;

			//3D空間上の方向を、2D平面座標へ変換する
			D3DXVECTOR3 addVector = conversion2D(camera, center);
			addVector *= length;
			addVector.x = UtilityFunction::clamp(addVector.x, -xLimit,xLimit);
			addVector.y = UtilityFunction::clamp(addVector.y, -yLimit,yLimit);

			//描画位置へ加算
			renderPosition += addVector;
			
			//描画
			marker[PLAYER1]->render(renderPosition);
		}
		break;
	}

	//他のマーカー（どちらの画面にも描画）
	for (int i = BOSS_ENEMY; i < MARKER_NUM; i++)
	{
		//対象が存在するかどうか判定
		bool skip = false;
		switch (i)
		{
		case BOSS_ENEMY:
			if(bossEnemyPosition)	center = *bossEnemyPosition;	//位置をセット
			else skip = true;
			radius = 1.0f;
			break;
		case ATTACKED_TREE:
			if (attackedTree)		center = *attackedTree;			//位置をセット
			else skip = true;
			radius = 1.0f;
			break;
		}

		//マーク対象が存在しないのでスキップ
		if (skip)
		{
			marker[i]->color = D3DCOLOR_RGBA(255, 255, 255, 0);
			continue;
		}

		//カリング判定
		if (UtilityFunction::culling(
			center, radius, camera->view, camera->fieldOfView,
			camera->nearZ, camera->farZ, camera->aspect))
		{
			//視野内：ビルボードによる描画
			marker[i]->position = center;
			marker[i]->color = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else {
			//視野外：スプライトによる描画

			marker[i]->color = D3DCOLOR_RGBA(255, 255, 255, 0);
			//カメラに対する対象の方向を求める
			D3DXVECTOR3 direction;
			Base::between2VectorDirection(&direction, camera->position, center);
			direction.y = 0;//Y成分削除
			D3DXVec3Normalize(&direction, &direction);//正規化

			//描画位置を求める
			D3DXVECTOR3 renderPosition = D3DXVECTOR3(0,0,0);
			//
			switch (playerNo){
			case gameMasterNS::PLAYER_1P:	renderPosition = CENTER1P;	break;
			case gameMasterNS::PLAYER_2P:	renderPosition = CENTER2P;	break;
			}

			//3D空間上の方向を、2D平面座標へ変換する
			D3DXVECTOR3 addVector = conversion2D(camera, center);
			addVector *= length;
			addVector.x = UtilityFunction::clamp(addVector.x, -xLimit, xLimit);
			addVector.y = UtilityFunction::clamp(addVector.y, -yLimit, yLimit);
			renderPosition.x += direction.x * (WINDOW_WIDTH / 4 - PlayerMarkerNS::WIDTH / 2);
			renderPosition.y += -direction.z * (WINDOW_HEIGHT / 2 - PlayerMarkerNS::HEIGHT / 2);

			marker[i]->render(renderPosition);
		}
	}

	//インスタンシングビルボードの更新
	billboard->update(0.0f);
	//インスタンシングビルボード
	billboard->render(camera->view, camera->projection, camera->position);
}

D3DXVECTOR3  MarkerRenderer::conversion2D(Camera* camera,D3DXVECTOR3 target)
{

	//カメラに対する対象の方向を求める
	D3DXVECTOR3 direction;
	Base::between2VectorDirection(&direction, camera->position, target);
	direction.y = 0;//Y成分削除
	D3DXVec3Normalize(&direction, &direction);//正規化

	//カメラ前方向
	D3DXVECTOR3 front = camera->getDirectionZ();
	front.y = 0.0f;
	D3DXVec3Normalize(&front, &front);
	//右方向
	D3DXVECTOR3 right = front;
	Base::anyAxisRotation(&right, D3DXVECTOR3(0, 1, 0), 90);
	//左方向
	D3DXVECTOR3 left = front;
	Base::anyAxisRotation(&left, D3DXVECTOR3(0, 1, 0), -90);

	//左か右か
	float rightDot = D3DXVec3Dot(&right, &direction);
	bool isRight = (rightDot > 0);
	float leftDot = D3DXVec3Dot(&left, &direction);
	bool isLeft = (leftDot > 0);

	//回転角を求める
	float angle;
	if (Base::formedRadianAngle(&angle, front, direction))
	{
		angle = D3DXToDegree(angle);
		if (isRight)	angle *= 1.0f;
		else if (isLeft)angle *= -1.0f;
	}
	else {
		angle = 180.0f;
	}

	//描画位置を求める
	D3DXVECTOR3 renderPosition;
	renderPosition = CENTER1P;

	D3DXVECTOR3 addVector = D3DXVECTOR3(0, 0, 1);
	Base::anyAxisRotation(&addVector, D3DXVECTOR3(0, 1, 0), angle);
	addVector.y = -addVector.z;
	addVector.z = 0.0f;

	return addVector;
}