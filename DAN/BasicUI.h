//================================
// 基礎UIクラスの定義[BasicUI.h]
// 染矢　晃介
//================================
#pragma once

//================================
// インクルード
//================================
#include"TextureLoader.h"
#include "Sprite.h"

//================================
// 名前空間
//================================
namespace basicUiNS
{
	//移動の種類
	enum MOVE_TYPE
	{
		slideRight,
		slideLeft,
		slideUp,
		slideDown,
	};

	const float SLIDE_SPEED = 20.0f;		//スライドインの速度
	const int	CHANGE_SIZE_SPEED = 20;		//サイズ変更の速度
	const int   CHANGE_ALPHA_SPEED = 10;		//α値の変更速度
}

//================================
//クラス定義
//================================
class BasicUI
{
public://メンバー変数
	D3DXVECTOR3 position;	//表示位置
	D3DCOLOR	color;		//色
	int widthSize;			//幅のサイズ
	int heightSize;			//高さサイズ
	float alphaValue;		//α値
	// uv座標用の変数
	D3DXVECTOR2 uvCoord01;
	D3DXVECTOR2 uvCoord02;
	D3DXVECTOR2 uvCoord03;
	D3DXVECTOR2 uvCoord04;

private:
public://メンバー関数
	BasicUI();				//コンストラクタ
	~BasicUI();				//デストラクタ
	void	changePostion(int movetype);//位置変更	
	void	increaseAlpha();//透明度増加
	void	decreaseAlpha();//透明度減少
	void	expansionWidthSize();//横サイズの拡大
	void	expansionHeightSize();//縦サイズの拡大
	void	reductionWidthSize();//横サイズの縮小
	void	reductionHeightSize();//縦サイズの縮小

private:
};