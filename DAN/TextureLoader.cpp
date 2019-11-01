//===================================================================================================================================
//【TextureLoader.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/22
// [更新日]2019/10/17
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "TextureLoader.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
using namespace textureNS;

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
TextureLoader* pointerTextureLoader = NULL;	//外部参照用

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
TextureLoader::TextureLoader()
{
	//グローバルポインタへ自身のポインタを代入
	if(pointerTextureLoader==NULL)pointerTextureLoader = this;

	// Splash
	textureName[SPLASH] = { "Splash.png" };

	// Tutorial
	textureName[TUTORIAL1] = { "tutorial01.png" };
	textureName[TUTORIAL2] = { "tutorial02.png" };

	// Title
	textureName[TITLE_LOGO] = { "DAN_Logo.png" };
	textureName[TITLE_MENU] = { "Title_Menu.png" };
	textureName[TITLE_INFO_TUTO] = { "Tutorial_info.png" };
	textureName[TITLE_INFO_GAME] = { "GameStart_info.png" };
	textureName[TITLE_INFO_CREDIT] = { "Credits_info.png" };
	textureName[TITLE_INFO_EXIT] = { "Exit_info.png" };
	textureName[TITLE_COPYRIGHT] = { "Copyrights.png" };
	textureName[TITLE_BAR] = {"Select_Bar.png"};


	// Operation
	textureName[OPERATION_GAMEPAD] = { "Operation_GamePad.png" };
	textureName[OPERATION_KEYBOARD] = { "Operation_KeyBoard.png" };

	// Credit
	textureName[CREDIT] = { "Credit_01.png" };

	// Game
	textureName[UI_COUNT_DOWN_ONE] = { "Game_CountDown_One.png" };

	//effect
	textureName[EFFECT_DAMAGE] = { "Game_Damage_Effect.png" };

	// ResultUIの文字や記号
	textureName[RESULT] = { "Result.png" };
	textureName[RESULT_PLAYER] = { "Player_No.png" };
	textureName[RESULT_GREENIG] = { "Greening_Rate.png" };
	textureName[RESULT_RYOKUKA] = { "Ryokuka_Ritsu.png" };
	textureName[RESULT_PERSENT] = { "Percent.png" };
	textureName[RESULT_DEFEAT] = { "Defeated_Counts.png" };
	textureName[RESULT_GEKITAI] = { "Gekitai_Su.png" };

	//ResultUIのテクスチャ
	textureName[RESULT_LINE] = { "Line.png" };
	textureName[RESULT_FLAME] = { "Flame.png" };
	textureName[RESULT_CHARA_A] = { "Chara_A.png" };
	textureName[RESULT_CHARA_B] = { "Chara_B.png" };
	textureName[RESULT_NEXT] = { "Next.png" };

	//ResultUIのランク
	textureName[RESULT_EXCELLENT] = { "Excellent.png" };
	textureName[RESULT_GREAT] = { "Great.png" };
	textureName[RESULT_CLEARE] = { "Cleared.png" };
	textureName[RESULT_FAILED] = { "Failed.png" };

	// etc
	textureName[NUMBER] = { "num.png" };
	textureName[TOON_OUT_LINE] = { "Outline.bmp" };
	textureName[TOON_SHADE] = { "Shade.bmp" };
	textureName[LIGHT_001] = { "Light_001.png" };
	textureName[TARGET] = { "stencil.png" };
	textureName[SAMPLE_TREE] = { "tree.png" };
	textureName[SAMPLE_GRASS] = { "middleGrass.png" };
	textureName[NEKO] = { "Chara_B.png" };

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
TextureLoader::~TextureLoader()
{
	pointerTextureLoader = NULL;
	release();
}

//===================================================================================================================================
//【ロード】
//===================================================================================================================================
void TextureLoader::load(LPDIRECT3DDEVICE9 device)
{
	setVisualDirectory();
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(device,textureName[i],&texture[i]);
	}
}

//===================================================================================================================================
//【解放】
//===================================================================================================================================
void TextureLoader::release()
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (texture[i] != NULL)
		{//テクスチャの解放
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

//===================================================================================================================================
//【テクスチャ外部参照関数】
//===================================================================================================================================
LPDIRECT3DTEXTURE9* textureNS::reference(int textureNo)
{
	return pointerTextureLoader->getTexture(textureNo);
}
