//============================
// [TitleUI.h]
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
namespace titleUiNS
{
	
	//ロゴ
	const int WIDTH_LOGO = (908);						//幅
	const int HEIGHT_LOGO = (376);						//高さ
	const D3DXVECTOR3 POSITION_LOGO = D3DXVECTOR3(960, 255, 0);//位置
	
	//選択肢
	const int WIDTH_MENU = (536);						//幅
	const int HEIGHT_MENU = (218);						//高さ
	const D3DXVECTOR3 POSITION_MENU = D3DXVECTOR3(1600, 895, 0);//位置

	//選択肢の説明
	const int WIDTH_MENU_INFO = (692);					//幅
	const int HEIGHT_MENU_INFO = (37);					//高さ
	const D3DXVECTOR3 POSITION_MENU_INFO = D3DXVECTOR3(340, 960, 0);//位置

	//制作者情報
	const int WIDTH_COPYRIGHT = (587);					//幅
	const int HEIGHT_COPYRIGHT = (34);					//高さ
	const D3DXVECTOR3 POSITION_COPYRIGHT = D3DXVECTOR3(340, 1005, 0);//位置

	//選択バー
	const int WIDTH_BAR = (737);						//幅
	const int HEIGHT_BAR = (43);						//高さ
	const D3DXVECTOR3 POSITION_BAR_TUTO = D3DXVECTOR3(1600, 810, 0);//位置
	const D3DXVECTOR3 POSITION_BAR_GAME = D3DXVECTOR3(1600, 865, 0);//位置
	const D3DXVECTOR3 POSITION_BAR_CREDIT = D3DXVECTOR3(1600, 925, 0);//位置
	const D3DXVECTOR3 POSITION_BAR_EXIT = D3DXVECTOR3(1600, 990, 0);//位置
	
	//共通
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0, 0, 0);			//回転
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//色
	
	//UIの種類
	enum UI_TYPE
	{
		UI_LOGO,			//ロゴ
		UI_MENU,			//選択肢
		UI_INFO_TUTO,		//チュートリアル選択肢の説明
		UI_INFO_GAME,		//ゲーム選択肢の説明
		UI_INFO_CREDIT,		//クレジット選択肢の説明
		UI_INFO_EXIT,		//終了選択肢の説明
		UI_COPYRIGHT,		//制作者情報
		UI_BAR,				//選択肢のバー
		UI_MAX,				//UIの総数
	};

	//メニュー選択肢
	enum MENU_TYPE
	{
		TUTORIAL,
		GAME,
		CREDIT,
		EXIT,
		MENU_MAX,
		CREATE
	};
}
//============================
//クラス定義
//============================
class TitleUI
{
public: //メンバー変数
	Sprite * UI[titleUiNS::UI_MAX];
	int selectState;

public: //メンバー関数
	void initialize();		//初期
	void uninitialize();	//終了
	void render();			//描画
	void update(Input *input);			//更新
	int getSelectState();	//選択肢状態の取得
	void moveBar();			//バーの移動

};
