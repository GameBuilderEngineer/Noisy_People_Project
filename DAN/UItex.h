//============================
//[UItex.h]
// 染矢　晃介
//============================
#pragma once
//============================
//インクルード
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"
#include "VirtualController.h"
//#include "UIcharacter.h"

//============================
//名前空間 定数
//============================
namespace uiTexNS
{
	//UIの種類
	enum UITEX_TYPE
	{
		LINE,
		FLAME01,
		FLAME02,
		CHARA_A,
		CHARA_B,
		NEXT,
		UITEX_MAX,
	};

	//リザルトのフェイズ
	enum RESULT_PHASE
	{
		PHASE_01,		//リザルトロゴのスライドイン
		PHASE_02,		//島表示
		PHASE_03,		//結果表示(数字など)
		PHASE_04,		//タイトルに戻るフェイズ
		PHASE_05,
		PHASE_MAX,		//フェイズの総数
	};

	//ライン
	const int WIDTH_LINE = (1920 * WINDOW_WIDTH / 1920);			//幅
	const int HEIGHT_LINE = (15 * WINDOW_HEIGHT / 1080);			//高さ
	const D3DXVECTOR3 POSITION_LINE = D3DXVECTOR3(-1000, 150 * WINDOW_HEIGHT / 1080, 0);//初期位置
	const float END_POS_LINE = (960 * WINDOW_WIDTH / 1920);		//最終位置のx座標

	//フレーム
	const int WIDTH_FLAME = (1024 * WINDOW_WIDTH / 1920);			//幅
	const int HEIGHT_FLAME = (1024 * WINDOW_HEIGHT / 1080);			//高さ
	const D3DXVECTOR3 POSITION_FLAME_01 = D3DXVECTOR3(480 * WINDOW_WIDTH / 1920, 2000, 0);//位置
	const D3DXVECTOR3 POSITION_FLAME_02 = D3DXVECTOR3(1440 * WINDOW_WIDTH / 1920, 2000, 0);//位置
	const float END_POS_FLAME = (578 * WINDOW_HEIGHT / 1080);		//最終位置のy座標

	//キャラA
	const int WIDTH_CHARA_A = (512 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_CHARA_A = (1024 * WINDOW_HEIGHT / 1080);		//高さ
	const D3DXVECTOR3 POSITION_CHARA_A = D3DXVECTOR3(480 * WINDOW_WIDTH / 1920, 2500, 0);//位置
	const float END_POS_CHARA_A = (618 * WINDOW_HEIGHT / 1080);	//最終座標のy座標

	//キャラB
	const int WIDTH_CHARA_B = (512 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_CHARA_B = (1024 * WINDOW_HEIGHT / 1080);		//高さ
	const D3DXVECTOR3 POSITION_CHARA_B = D3DXVECTOR3(1440 * WINDOW_WIDTH / 1920,2500, 0);//位置
	const float END_POS_CHARA_B = (628 * WINDOW_HEIGHT / 1080);	//最終座標のy座標

	//NEXTテクスチャ
	const int WIDTH_NEXT = (512 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_NEXT = (64 * WINDOW_HEIGHT / 1080);		//高さ
	const D3DXVECTOR3 POSITION_NEXT = D3DXVECTOR3(4500, 1017, 0);//位置
	const float END_POS_NEXT = (1730 * WINDOW_WIDTH / 1920);	//最終座標のx座標

	//タイトルへ戻るボタン
	const int WIDTH_TO_TITLE = (512 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_TO_TITLET = (64 * WINDOW_HEIGHT / 1080);		//高さ

	//共通
	const D3DXVECTOR3 ROTATION_TEX = D3DXVECTOR3(0, 0, 0);			//回転
	const D3DCOLOR COLOR_TEX = D3DCOLOR_RGBA(255, 255, 255, 255);	//色
	const float SLIDE_SPEED_TEX = (20);

}
//============================
//クラス定義
//============================
class UItexture 
{
public: //メンバー変数
	Sprite * uiTexture[uiTexNS::UITEX_MAX];	//スプライト
	D3DXVECTOR3 pos[uiTexNS::UITEX_MAX];	//表示位置管理用

public: //メンバー関数
	void initialize();						//初期化
	void render(int resultPhase);			//描画
	void update(int resultPhase);			//更新
	void uninitialize();					//終了
	void slideRight(int uiType);			//右へスライドイン関数
	void slideLeft(int uiType);				//左へスライドイン関数
	void slideUp(int uiType);				//上へスライドイン関数
	void assignPos(int uiType);				//表示位置の代入関数
};



