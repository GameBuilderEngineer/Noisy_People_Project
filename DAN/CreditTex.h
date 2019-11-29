//============================
// [CreditTex.h]
// 染矢　晃介
//============================

//============================
//インクルード
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "VirtualController.h"

//============================
//名前空間 定数
//============================
namespace creditTex
{
	const int WIDTH = 1920;								//横幅
	const int HEIGHT = 3240;							//縦幅
	const D3DXVECTOR3 POSITION = D3DXVECTOR3(WINDOW_WIDTH/2, WINDOW_HEIGHT+540,0);//位置
	const  D3DXVECTOR3 POSITION_2 = D3DXVECTOR3( WINDOW_WIDTH / 2, WINDOW_HEIGHT+3780, 0);//位置
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0,0,0);			//回転
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//色
	const float MOVE_MAX = -540;			//上方向への移動の限界値
	const float MOVE_SPEED = 1.5f;			//上に動くスピード
	const int CREDIT_MAX = 2;
}
//============================
//クラス定義
//============================
class CreditTex
{
public: //メンバー変数
	Sprite *credit[creditTex::CREDIT_MAX];
	D3DXVECTOR3 pos[creditTex::CREDIT_MAX];
	float       alpha;
	int         secneState;

public: //メンバー関数
	void initialize();		//初期
	void uninitialize();	//終了
	void render();			//描画
	void update();			//更新
	void moveCredit();		//上に動かす処理
	void fadeOut();			//フェードアウト
	int getSceneState();

};