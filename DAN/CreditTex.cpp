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
	credit = new Sprite;

	//開始位置の初期化
	pos = POSITION;

	credit->initialize(
						*textureNS::reference(textureNS::CREDIT)//テクスチャ
						,SpriteNS::CENTER						//中心
						,WIDTH									//横幅
						,HEIGHT									//縦幅
						,POSITION								//表示位置
						,ROTATION								//回転
						,COLOR);								//色

}

//============================
//描画
//============================
void CreditTex::render()
{
	credit->render();
}

//============================
//更新
//============================
void CreditTex::update()
{
	if (pos.y > MOVE_MAX)
	{
		moveCredit();
	}
}

//============================
//終了処理
//============================
void CreditTex::uninitialize()
{
	delete credit;
}

//===========================
// 上に動かす処理
//===========================
void CreditTex::moveCredit()
{

	pos.y -= MOVE_SPEED;

	credit->setPosition(pos);
	credit->setVertex();
	

}