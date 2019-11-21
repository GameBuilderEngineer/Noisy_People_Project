#pragma once
//==================================
//電力ゲージクラス[electGuage.h]
//染矢晃介
//==================================

//========================================================================================================
//要素
//・タンクがある
//・ゲージがマックスになるとタンクが1増加
//・時間経過で徐々にゲージ回復
//・アイテムを拾うと回復
//・ヴィジョン使用中はゲージを消費
//・ということは電力を参照してゲージを動かすと便利？
//========================================================================================================

//===================================
//インクルード
//===================================
#include "BasicUI.h"

//===================================
//名前空間
//===================================
namespace electGuageNS
{
	const int WIDTH_EN_GUAGE = (360 * WINDOW_WIDTH / 1920);	//電力ゲージの横幅
	const int HEIGHT_EN_GUAGE = (30 * WINDOW_HEIGHT / 1080);//電力ゲージの縦幅
	const D3DXVECTOR3 POSITION_EN_GUAGE = D3DXVECTOR3(102 * WINDOW_WIDTH / 1920, 35 * WINDOW_HEIGHT / 1080, 0);	//電力ゲージの表示位置(仮)
	const D3DXVECTOR3 POSITION_EN_GUAGE_P2 = D3DXVECTOR3(1872 * WINDOW_WIDTH / 1920, 35 * WINDOW_HEIGHT / 1080, 0);	//電力ゲージの表示位置(仮)
}

//===================================
//クラス定義
//===================================
class ElectGuage :public BasicUI
{
public://メンバー変数
	Sprite * electGuage;
	int width;
private:
public://メンバー関数
	ElectGuage();
	~ElectGuage();
	void initialize(int playerType);
	void render();
	void update();
	void changeGuage(int elect);//ゲージの変化処理
private:

};
	