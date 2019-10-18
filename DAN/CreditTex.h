//============================
// [CreditTex.h]
// 染矢　晃介
//============================

//============================
//インクルード
//============================
#include "Sprite.h"
#include "TextureLoader.h"

//============================
//名前空間 定数
//============================
namespace creditTex
{
	const int WIDTH = WINDOW_WIDTH;								//横幅
	const int HEIGHT = WINDOW_HEIGHT*2;							//縦幅
	const D3DXVECTOR3 POSITION = D3DXVECTOR3(WINDOW_WIDTH/2, WINDOW_HEIGHT/2,0);//位置
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0,0,0);			//回転
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//色
	const float MOVE_MAX = WINDOW_HEIGHT/15;					//上方向への移動の限界値
	const float MOVE_SPEED = 1.0f;								//上に動くスピード
}
//============================
//クラス定義
//============================
class CreditTex
{
public: //メンバー変数
	Sprite *credit;
	D3DXVECTOR3 pos;
	SpriteNS::Vertex2D	*vertexWk[SpriteNS::NUM_VERTEX];// 頂点情報格納ワーク

public: //メンバー関数
	void initialize();		//初期
	void uninitialize();	//終了
	void render();			//描画
	void update();			//更新
	void moveCredit();		//上に動かす処理


};