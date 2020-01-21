//==========================================
//リザルト文字処理
//染矢晃介
//==========================================

//==========================================
//インクルード
//==========================================
#include "Result_chara.h"

//==========================================
// using宣言
//==========================================
using namespace result_CharaNS;
using namespace basicUiNS;

//==========================================
//コンストラクタ
//==========================================
Result_Chara::Result_Chara()
{
	result = new Sprite;
}

//==========================================
//デストラクタ
//==========================================
Result_Chara::~Result_Chara()
{
	delete result;
}

//==========================================
//初期化
//==========================================
void Result_Chara::initialize()
{
	BasicUI::assingSize(WIDTH_RESULT, HEIGHT_RESULT);
	BasicUI::assingPosition(POSITION_RESULT);
	BasicUI::initialize(result, textureNS::reference(textureNS::RESULT));
}

//==========================================
//描画
//==========================================
void Result_Chara::render()
{
	result->render();
}

//==========================================
//更新
//==========================================
void Result_Chara::update(float flametime)
{
	if (BasicUI::position.x > END_POS_RESULT)
	{
		BasicUI::changePostion(result, slideLeft,flametime);
	}
}