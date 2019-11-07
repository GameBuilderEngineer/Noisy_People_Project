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
	alphaValue += CHANGE_ALPHA_SPEED;
	color = D3DCOLOR_RGBA(255, 255, 255, (int)alphaValue);
}

//=================================
//透明度の減少
//=================================
void BasicUI::decreaseAlpha()
{
	alphaValue -= CHANGE_ALPHA_SPEED;
	color = D3DCOLOR_RGBA(255, 255, 255, (int)alphaValue);
}

//=================================
//横サイズの拡大
//=================================
void BasicUI::expansionWidthSize()
{
	widthSize += CHANGE_SIZE_SPEED;
}

//=================================
//縦サイズの変更
//=================================
void BasicUI::expansionHeightSize()
{
	widthSize -= CHANGE_SIZE_SPEED;
}

//=================================
//横サイズの拡大
//=================================
void BasicUI::reductionWidthSize()
{
	heightSize += CHANGE_SIZE_SPEED;
}

//=================================
//縦サイズの縮小
//=================================
void BasicUI::reductionHeightSize()
{
	heightSize -= CHANGE_SIZE_SPEED;
}

