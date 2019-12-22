//=========================================================
//フィナーレテクスチャクラス[Finale.h]
//染矢晃介
//=========================================================

//=========================================================
//インクルード
//=========================================================
#include "BasicUI.h"

//=========================================================
//名前空間
//=========================================================
namespace finaleTexNS
{
	const int WIDTH_FIN = (512 * WINDOW_WIDTH / 1920);//横幅
	const int HEIGHT_FIN = (256 * WINDOW_HEIGHT / 1080);//高さ
	const D3DXVECTOR3 POSITION_FIN = D3DXVECTOR3(1700 * WINDOW_WIDTH / 1920, 900 * WINDOW_HEIGHT / 1080, 0);//表示位置
	const D3DCOLOR COLOR_FIN = D3DCOLOR_RGBA(255, 255, 255, 0);	//色
	const float ALPHA_SPEED = 3.0f;
}

//=========================================================
//クラス定義
//=========================================================
class FinaleTex :public BasicUI
{
private://メンバー変数
	Sprite * fin;
	int alpha;
public://メンバー関数
	FinaleTex();
	~FinaleTex();
	void initialize();
	void render();
	void update();
};