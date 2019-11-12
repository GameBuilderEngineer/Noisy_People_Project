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
	const int WIDTH_RANK = (2960);	//初期の幅
	const int HEIGHT_RANK = (2551);	//初期の高さ
	const int END_WIDTH_RANK = (740);		//最終の幅
	const int END_HEIGHT_RANK = (331);		//最終の高さ
	const D3DXVECTOR3 POSITION_RANK_P1 = D3DXVECTOR3(480, 825, 0);//プレイヤー１用の表示位置
	const D3DXVECTOR3 POSITION_RANK_P2 = D3DXVECTOR3(1440, 825, 0);//プレイヤー2用の表示位置
	
	//共通
	const D3DXVECTOR3 ROTATION_TEX = D3DXVECTOR3(0, 0, 0);			//回転
	const D3DCOLOR COLOR_TEX = D3DCOLOR_RGBA(255, 255, 255, 255);	//色
	const int SHRINK_SPEED = 20;									//縮小スピード
}
//============================
//クラス定義
//============================
class UIrank
{
public: //メンバー変数
	Sprite * rank[uiRankNS::RANK_MAX];			//スプライトの作成
	int widthSize;								//幅のサイズ管理用
	int heightSize;								//高さサイズ管理用
	bool playedSE;								//SE再生フラグ
public: //メンバー関数
	void initialize(int rankType, int playerType);
	void render(int rankType);
	void update(int rankType);
	void uninitialize(int rankType);
	void changeSize(int rankType);		//拡大縮小関数
};
