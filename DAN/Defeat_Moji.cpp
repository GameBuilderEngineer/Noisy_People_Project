//==========================================
//撃退数文字処理
//染矢晃介
//==========================================

//==========================================
//インクルード
//==========================================
#include "Defeat_Moji.h"

//==========================================
// using宣言
//==========================================
using namespace defeat_MojiNS;
using namespace basicUiNS;

//==========================================
//コンストラクタ
//==========================================
Defeat_Moji::Defeat_Moji()
{
	defeat = new Sprite;
}

//==========================================
//デストラクタ
//==========================================
Defeat_Moji::~Defeat_Moji()
{
	delete defeat;
}

//==========================================
//初期化
//==========================================
void Defeat_Moji::initialize(int playerType)
{
	switch (playerType)
	{
	case P1:
		BasicUI::assingSize(WIDTH_GEKITAI, HEIGHT_GEKITAI);
		BasicUI::assingPosition(POSITION_GEKITAI_01);
		BasicUI::initialize(defeat, textureNS::reference(textureNS::RESULT_GEKITAI));
		break;

	case P2:
		BasicUI::assingSize(WIDTH_GEKITAI, HEIGHT_GEKITAI);
		BasicUI::assingPosition(POSITION_GEKITAI_02);
		BasicUI::initialize(defeat, textureNS::reference(textureNS::RESULT_GEKITAI));
		break;
	}

}

//==========================================
//描画
//==========================================
void Defeat_Moji::render()
{
	defeat->render();
}

//==========================================
//更新
//==========================================
void Defeat_Moji::update(int playerType)
{
	if (BasicUI::position.y > END_POS_GEKITAI)
	{
		BasicUI::changePostion(defeat, slideUp);
	}
}