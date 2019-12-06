//============================
//[UIrank.h]
// 染矢　晃介
//============================

//============================
//インクルード
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"
#include "VirtualController.h"
#include "BasicUI.h"

//============================
//名前空間 定数
//============================
namespace uiRankNS
{
	//ランクの種類
	enum UIRANK_TYPE
	{
		EXCELLENT,
		GREAT,
		CLEARE,
		FAILED,
		RANK_MAX,
	};

	//ランクの表示位置
	const int WIDTH_RANK = (2960 * WINDOW_WIDTH / 1920);	//初期の幅
	const int HEIGHT_RANK = (2551 * WINDOW_HEIGHT / 1080);	//初期の高さ
	const int END_WIDTH_RANK = (740 * WINDOW_WIDTH / 1920);		//最終の幅
	const int END_HEIGHT_RANK = (331 * WINDOW_HEIGHT / 1080);		//最終の高さ
	const D3DXVECTOR3 POSITION_RANK_P1 = D3DXVECTOR3(480 * WINDOW_WIDTH / 1920, 825 * WINDOW_HEIGHT / 1080, 0);//プレイヤー１用の表示位置
	const D3DXVECTOR3 POSITION_RANK_P2 = D3DXVECTOR3(1440 * WINDOW_WIDTH / 1920, 825 * WINDOW_HEIGHT / 1080, 0);//プレイヤー2用の表示位置
	
	//共通
	const D3DXVECTOR3 ROTATION_TEX = D3DXVECTOR3(0, 0, 0);			//回転
	const D3DCOLOR COLOR_TEX = D3DCOLOR_RGBA(255, 255, 255, 255);	//色
	const int SHRINK_SPEED = 20;									//縮小スピード
}
//============================
//クラス定義
//============================
class UIrank:public BasicUI
{
public: //メンバー変数
	Sprite * rank[basicUiNS::PLAYER_TYPE_MAX][uiRankNS::RANK_MAX];//スプライトの作成
	int widthSize;								//幅のサイズ管理用
	int heightSize;								//高さサイズ管理用
	bool playedSE;								//SE再生フラグ
public: //メンバー関数
	UIrank();//コンストラクタ
	~UIrank();//デストラクタ
	void initialize();//初期化
	void render(int rankType,int rankType2);//描画
	void update(int rankType1, int rankType2);//更新
};
