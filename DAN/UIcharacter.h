//============================
//[UIcharacter.h]
// 染矢　晃介
//============================

//============================
//インクルード
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"
#include "VirtualController.h"

//============================
//名前空間 定数
//============================
namespace uiCharacterNS
{
	//UIの種類
	enum UICHARACTER_TYPE
	{
		RESULT,
		PLAYER1,
		PLAYER2,
		GREENIG,
		RYOKUKA,
		PERSENT,
		DEFEAT,
		GEKITAI,
		UICHARACTER_MAX,	//UI(文字)の総数
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

	//リザルトロゴ
	const int WIDTH_RESULT = (330 * WINDOW_WIDTH / 1920);			//幅
	const int HEIGHT_RESULT = (170 * WINDOW_HEIGHT / 1080);		//高さ
	const D3DXVECTOR3 POSITION_RESULT = D3DXVECTOR3(2100, 70 * WINDOW_HEIGHT / 1080, 0);//初期位置
	const float	END_POS_RESULT =(215.0f * WINDOW_WIDTH / 1920);	//最終位置のx座標

	//プレイヤー
	const int WIDTH_PLAYER = (345 * WINDOW_WIDTH / 1920);			//幅
	const int HEIGHT_PLAYER = (90 * WINDOW_HEIGHT / 1080);			//高さ
	const D3DXVECTOR3 POSITION_PLAYER = D3DXVECTOR3(3800, 250 * WINDOW_HEIGHT / 1080, 0);//初期位置
	const float END_POS_PLAYER_01 = (645.0f* WINDOW_WIDTH / 1920);//プレイヤー１の最終位置のx座標
	const float END_POS_PLAYER_02 = (1605 * WINDOW_WIDTH / 1920);	 //プレイヤー２の最終位置のx座標
	//uv座標
	const D3DXVECTOR2 PLAYER1_UV_VERTEX01 = D3DXVECTOR2(0.0, 0.0);
	const D3DXVECTOR2 PLAYER1_UV_VERTEX02 = D3DXVECTOR2(1.0, 0.0);
	const D3DXVECTOR2 PLAYER1_UV_VERTEX03 = D3DXVECTOR2(0.0, 0.5);
	const D3DXVECTOR2 PLAYER1_UV_VERTEX04 = D3DXVECTOR2(1.0, 0.5);
	const D3DXVECTOR2 PLAYER2_UV_VERTEX01 = D3DXVECTOR2(0.0, 0.5);
	const D3DXVECTOR2 PLAYER2_UV_VERTEX02 = D3DXVECTOR2(1.0, 0.5);
	const D3DXVECTOR2 PLAYER2_UV_VERTEX03 = D3DXVECTOR2(0.0, 1.0);
	const D3DXVECTOR2 PLAYER2_UV_VERTEX04 = D3DXVECTOR2(1.0, 1.0);

	//緑化(英語)
	const int WIDTH_GREENIG = (334 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_GREENIG = (53 * WINDOW_HEIGHT / 1080);		//高さ
	const D3DXVECTOR3 POSITION_GREENIG_01 = D3DXVECTOR3(248 * WINDOW_WIDTH / 1920, 3500, 0);//プレイヤー１用初期位置
	const D3DXVECTOR3 POSITION_GREENIG_02 = D3DXVECTOR3(1208 * WINDOW_WIDTH / 1920, 3500, 0);//プレイヤー２用初期位置
	const float END_POS_GREENING = (353.0f* WINDOW_HEIGHT / 1080);//最終位置のy座標

	//緑化
	const int WIDTH_RYOKUKA = (256 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_RYOKUKA = (86 * WINDOW_HEIGHT / 1080);		//高さ
	const D3DXVECTOR3 POSITION_RYOKUKA_01 = D3DXVECTOR3(358 * WINDOW_WIDTH / 1920, 4500, 0);//プレイヤー１用初期位置
	const D3DXVECTOR3 POSITION_RYOKUKA_02 = D3DXVECTOR3(1318 * WINDOW_WIDTH / 1920, 4500, 0);//プレイヤー２用初期位置
	const float END_POS_RYOKUKA = (425.0f * WINDOW_HEIGHT / 1080);	//最終位置のy座標

	//パーセント
	const int WIDTH_PERSENT = (115 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_PERSENT = (124 * WINDOW_HEIGHT / 108);		//高さ
	const D3DXVECTOR3 POSITION_PERSENT_01 = D3DXVECTOR3(845 * WINDOW_WIDTH / 1920, 4500, 0);//プレイヤー１用初期位置
	const D3DXVECTOR3 POSITION_PERSENT_02 = D3DXVECTOR3(1805 * WINDOW_WIDTH / 1920, 4500, 0);//プレイヤー２用初期位置
	const float END_POS_PERSENT = (429.0f* WINDOW_HEIGHT / 108);	//最終位置のy座標

	//撃退(英語)
	const int WIDTH_DEFEAT = (390 * WINDOW_WIDTH / 1920);			//幅
	const int HEIGHT_DEFEAT = (53 * WINDOW_HEIGHT / 108);			//高さ
	const D3DXVECTOR3 POSITION_DEFEAT_01 = D3DXVECTOR3(273 * WINDOW_WIDTH / 1920, 5500, 0);//プレイヤー１用初期位置
	const D3DXVECTOR3 POSITION_DEFEAT_02 = D3DXVECTOR3(1318 * WINDOW_WIDTH / 1920, 5500, 0);//プレイヤー２用初期位置
	const float END_POS_DEFEAT = (523.0f * WINDOW_HEIGHT / 108);	//最終位置のy座標

	//撃退
	const int WIDTH_GEKITAI = (255 * WINDOW_WIDTH / 1920);		//幅
	const int HEIGHT_GEKITAI = (86 * WINDOW_HEIGHT / 108);		//高さ
	const D3DXVECTOR3 POSITION_GEKITAI_01 = D3DXVECTOR3(358 * WINDOW_WIDTH / 1920, 6500, 0);//プレイヤー１用初期位置
	const D3DXVECTOR3 POSITION_GEKITAI_02 = D3DXVECTOR3(1318 * WINDOW_WIDTH / 1920, 6500, 0);//プレイヤー２用初期位置
	const float END_POS_GEKITAI = (595.0f* WINDOW_HEIGHT / 108);	//最終位置のy座標

	 //共通
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0, 0, 0);			//回転
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//色
	const float SLIDE_SPEED = 20.0f;							//スライドインの速度
}
//============================
//クラス定義
//============================
class UIcharacter
{
public: //メンバー変数
	Sprite * uiCharacter[uiCharacterNS::UICHARACTER_MAX];	//スプライト
	D3DXVECTOR3 pos[uiCharacterNS::UICHARACTER_MAX];		//表示位置管理用

public: //メンバー関数
	void initialize(int playerType);						//初期化
	void render(int resultPhase);							//描画
	void update(int resultPhase);							//更新
	void uninitialize();									//終了
	void slideRight(int uiType);							//右へスライドイン関数
	void slideLeft(int uiType);								//左へスライドイン関数
	void slideUp(int uiType);								//上へスライドイン関数
	void assignPos(int uiType,int playerType);				//表示位置の代入関数
	void setUV();											//uv座標の設定関数
};


