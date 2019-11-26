//============================
//[CrediTex.cpp]
//染矢晃介
//============================

//============================
//インクルード
//============================
#include "CreditTex.h"

//============================
//【using宣言】
//============================
using namespace creditTex;

//============================
//グローバル変数
//============================

//============================
//初期化
//============================
void CreditTex::initialize()
{
	for (int i = 0; i < CREDIT_MAX; i++)
	{
		credit[i] = new Sprite;
	}
	
	credit[0]->initialize(
						*textureNS::reference(textureNS::CREDIT)//テクスチャ
						,SpriteNS::CENTER						//中心
						,WIDTH									//横幅
						,HEIGHT									//縦幅
						,POSITION								//表示位置
						,ROTATION								//回転
						,COLOR);								//色

	credit[1]->initialize(
						*textureNS::reference(textureNS::CREDIT2)//テクスチャ
						, SpriteNS::CENTER						//中心
						, WIDTH									//横幅
						, HEIGHT								//縦幅
						, POSITION_2							//表示位置
						, ROTATION								//回転
						, COLOR);								//色
						
	pos[0] = POSITION;
	pos[1] = POSITION_2;
	secneState = 4;
	alpha = 255.0f;
}

//============================
//描画
//============================
void CreditTex::render()
{
	for (int i = 0; i < CREDIT_MAX; i++)
	{
		credit[i]->render();
	}
}

//============================
//更新
//============================
void  CreditTex::update()
{
	if (pos[1].y >= MOVE_MAX)
	{
		moveCredit();
	}

	if (pos[1].y <= -540)
	{
		fadeOut();
	}
}

//============================
//終了処理
//============================
void CreditTex::uninitialize()
{
	for (int i = 0; i < CREDIT_MAX; i++)
	{
		delete credit[i];
	}
}

//===========================
// 上に動かす処理
//===========================
void CreditTex::moveCredit()
{
	pos[0].y -= MOVE_SPEED;
	pos[1].y -= MOVE_SPEED;

	credit[0]->setPosition(pos[0]);
	credit[1]->setPosition(pos[1]);
	credit[0]->setVertex();
	credit[1]->setVertex();
}

//===========================
//フェードアウト処理
//===========================
void CreditTex::fadeOut()
{
	alpha -= 0.5f;
	credit[1]->setAlphaAllVertex(alpha);
	if (alpha <= 0)
	{
		secneState = 2;
	}
}

//===========================
//シーン遷移管理用
//===========================
int CreditTex::getSceneState()
{
	return secneState;
}