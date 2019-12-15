//==================================
// 基礎UIクラス[UImanager.cpp]
// 染矢　晃介
//==================================

//==================================
// インクルード
//==================================
#include "BasicUI.h"

//==================================
// [using宣言]
//==================================
using namespace basicUiNS;

//==================================
// コンストラクタ
//==================================
BasicUI::BasicUI()
{
	//各数値の初期化
	position = D3DXVECTOR3(0, 0, 0);			//表示位置
	rotation = D3DXVECTOR3(0, 0, 0);			//回転
	color = D3DCOLOR_RGBA(255,255, 255, 255);	//色
	widthSize = 0;								//横幅
	heightSize = 0;								//高さ
	pivot = SpriteNS::CENTER;
	//テクスチャ座標
	uvCoord01 = D3DXVECTOR2(0.0, 0.0);
	uvCoord02 = D3DXVECTOR2(1.0, 0.0);
	uvCoord03 = D3DXVECTOR2(0.0, 1.0);
	uvCoord04 = D3DXVECTOR2(1.0, 1.0);
}

//==================================
//デストラクタ
//==================================
BasicUI::~BasicUI()
{

}

//==================================
//初期化
//==================================
void BasicUI::initialize(Sprite *sprite, LPDIRECT3DTEXTURE9 *fileName)
{
	sprite->initialize(
		*fileName					//テクスチャ
		, pivot						//中心
		, widthSize					//横幅
		, heightSize				//縦幅
		, position					//表示位置
		, rotation 					//回転
		, color);					//色
}

//==================================
//表示位置の代入
//==================================
void BasicUI::assingPosition(D3DXVECTOR3 pos)
{
	position = pos;
}

//==================================
//回転の代入
//==================================
void BasicUI::assingRotation(D3DXVECTOR3 rot)
{
	rotation = rot;
}

//==================================
//色の代入
//==================================
void BasicUI::assingColor(D3DCOLOR col)
{
	color = col;
}

//==================================
//サイズの代入
//==================================
void BasicUI::assingSize(int widh, int height)
{
	widthSize = widh;
	heightSize = height;
}

//==================================
//UV座標の代入
//==================================
void BasicUI::assingUV(D3DXVECTOR2 uvCoord1, D3DXVECTOR2 uvCoord2, D3DXVECTOR2 uvCoord3, D3DXVECTOR2 uvCoord4)
{
	uvCoord01 = uvCoord1;
	uvCoord02 = uvCoord2;
	uvCoord03 = uvCoord3;
	uvCoord04 = uvCoord4;
}

//==================================
// 中心位置の代入
//==================================
void BasicUI::changePivot(int pivotType)
{
	switch (pivotType)
	{
	case SpriteNS::TOP_LEFT:
		pivot = SpriteNS::TOP_LEFT;
		break;
	case SpriteNS::TOP_RIGHT:
		pivot = SpriteNS::TOP_RIGHT;
		break;
	default:
		pivot = SpriteNS::TOP_CENTER;
		break;
	}
	
}

//==================================
//座標の変更
//引数：移動の種類
//==================================
void BasicUI::changePostion(Sprite *sprite,int movetype)
{
	switch (movetype)
	{
	case slideRight:
		position.x += SLIDE_SPEED;
		sprite->setPosition(position);
		sprite->setVertex();
		sprite->render();
		break;

	case slideLeft:
		position.x -= SLIDE_SPEED;
		sprite->setPosition(position);
		sprite->setVertex();
		sprite->render();
		break;

	case slideUp:
		position.y -= SLIDE_SPEED;
		sprite->setPosition(position);
		sprite->setVertex();
		sprite->render();
		break;

	case slideDown:
		position.y += SLIDE_SPEED;
		sprite->setPosition(position);
		sprite->setVertex();
		sprite->render();
		break;

	default:
		break;
	}
}

//=================================
//透明度の変更
//=================================
void BasicUI::changeAlpha(Sprite *sprite,float alpha)
{

	alphaValue = alpha;
	color = D3DCOLOR_RGBA(255, 255, 255, (int)alphaValue);
	sprite->setAlphaAllVertex(alphaValue);
	sprite->render();
	
}

//=================================
//透明度の減少
//=================================
void BasicUI::decreaseAlpha(Sprite *sprite,float alpha)
{
	
}

//=================================
//横サイズの拡大
//引数:サイズの変化速度
//=================================
void BasicUI::expansionWidthSize(Sprite *sprite,int speed)
{
	widthSize += speed;
	sprite->setSize(widthSize, heightSize);
	sprite->setVertex();
	sprite->render();
}

//=================================
//縦サイズの変更
//引数:サイズの変化速度
//=================================
void BasicUI::expansionHeightSize(Sprite *sprite,int speed)
{
	heightSize -= speed;
	sprite->setSize(widthSize, heightSize);
	sprite->setVertex();
	sprite->render();
}

//=================================
//横サイズの拡大
//引数:サイズの変化速度
//=================================
void BasicUI::reductionWidthSize(Sprite *sprite,int speed)
{
	widthSize -= speed;
	sprite->setSize(widthSize, heightSize);
	sprite->setVertex();
	sprite->render();
}

//=================================
//縦サイズの縮小
//引数:サイズの変化速度
//=================================
void BasicUI::reductionHeightSize(Sprite *sprite,int speed)
{
	heightSize -= speed;
	sprite->setSize(widthSize, heightSize);
	sprite->setVertex();
	sprite->render();
}

//=================================
//UV座標の変化
//=================================
void BasicUI::changeUV(Sprite *sprite,D3DXVECTOR2 uv01, D3DXVECTOR2 uv02, D3DXVECTOR2 uv03, D3DXVECTOR2 uv04)
{
	
	sprite->setUVCoord(uv01, uv02, uv03, uv04);
	sprite->setVertex();
	sprite->render();
}

//=================================
//横サイズの変化
//=================================
void BasicUI::changeWhidthSize(Sprite *sprite, int size)
{
	widthSize = size;
	sprite->setSize(widthSize, heightSize);
	sprite->setVertex();
	sprite->render();
}

//==================================
//色の変更
//==================================
void BasicUI::changeColor(Sprite *sprite, D3DCOLOR col)
{
	color = col;
	sprite->setColorAllVertex(color);
	sprite->setVertex();
	sprite->render();
}