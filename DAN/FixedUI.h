//=================================
// 固定されているUI[FixedUI]
// 染矢晃介
//=================================

//=================================
// インクルード
//=================================
#include "Sprite.h"
#include "TextureLoader.h"

//=================================
// 名前空間
//=================================
namespace fixedUiNS
{
	const int WIDTH_TIMERFLAME = (1920);	//タイマーフレームの横幅
	const int HEIGHT_TIMERFLAME = (169);	//タイマーフレームの縦幅
	const D3DXVECTOR3 POSITION_TIMERFLAME = D3DXVECTOR3(960, 84, 0);	//タイマーフレームの表示位置(仮)
	const D3DXVECTOR3 ROTATION_TIMERFLAME = D3DXVECTOR3(0, 0, 0);		//回転
	const D3DCOLOR COLOR_TIMERFLAME = D3DCOLOR_RGBA(255, 255, 255, 255);//色

	const int WIDTH_BUTTON = (1920);	//タイマーフレームの横幅
}

//=================================
//クラス定義
//=================================
class FixedUI
{
public://メンバー変数
private:
	Sprite * fixedUI;

public://メンバー関数
	FixedUI();
	~FixedUI();
	void initialize();
	void render();
	void update();
	void uninitialize();
private:
};