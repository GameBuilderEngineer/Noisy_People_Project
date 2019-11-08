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
		, SpriteNS::CENTER			//中心
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
//座標の変更
//引数：移動の種類
//==================================
void BasicUI::changePostion(int movetype)
{
	switch (movetype)
	{
	case slideRight:
		position.x += SLIDE_SPEED;
		break;

	case slideLeft:
		position.x -= SLIDE_SPEED;
		break;

	case slideUp:
		position.y -= SLIDE_SPEED;
		break;

	case slideDown:
		position.y += SLIDE_SPEED;
		break;

	default:
		break;
	}
}

//=================================
//透明度の増加
//=================================
void BasicUI::increaseAlpha()
{
	if (alphaValue >= 255)
	{
		alphaValue += CHANGE_ALPHA_SPEED;
		color = D3DCOLOR_RGBA(255, 255, 255, (int)alphaValue);
	}
}

//=================================
//透明度の減少
//=================================
void BasicUI::decreaseAlpha()
{
	if (alphaValue >= 0)
	{
		alphaValue -= CHANGE_ALPHA_SPEED;
		color = D3DCOLOR_RGBA(255, 255, 255, (int)alphaValue);
	}
}

//=================================
//横サイズの拡大
//引数:サイズの変化速度
//=================================
void BasicUI::expansionWidthSize(int speed)
{
	widthSize += speed;
}

//=================================
//縦サイズの変更
//引数:サイズの変化速度
//=================================
void BasicUI::expansionHeightSize(int speed)
{
	widthSize -= speed;
}

//=================================
//横サイズの拡大
//引数:サイズの変化速度
//=================================
void BasicUI::reductionWidthSize(int speed)
{
	heightSize += speed;
}

//=================================
//縦サイズの縮小
//引数:サイズの変化速度
//=================================
void BasicUI::reductionHeightSize(int speed)
{
	heightSize -= speed;
}

