//===================================================================================================================================
//【TextureLoader.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/22
// [更新日]2019/11/02
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
	if (pointerTextureLoader == NULL)pointerTextureLoader = this;

	// Splash
	textureName[SPLASH] = { "Splash.png" };

	// Tutorial
	textureName[TUTORIAL1] = { "Slide_01.png" };
	textureName[TUTORIAL2] = { "Slide_02.png" };
	textureName[TUTORIAL_UI_0] = { "steplist.png" };
	textureName[TUTORIAL_UI_1] = { "check.png" };
	textureName[TUTORIAL_UI_PLANE] = { "comment.png" };

	// Title
	textureName[TITLE_LOGO] = { "DAN_Logo.png" };
	textureName[TITLE_MENU] = { "Title_Menu.png" };
	textureName[TITLE_INFO_TUTO] = { "Tutorial_info.png" };
	textureName[TITLE_INFO_GAME] = { "GameStart_info.png" };
	textureName[TITLE_INFO_CREDIT] = { "Credits_info.png" };
	textureName[TITLE_INFO_EXIT] = { "Exit_info.png" };
	textureName[TITLE_COPYRIGHT] = { "Copyrights.png" };
	textureName[TITLE_BAR] = { "Select_Bar.png" };
	textureName[TITLE_PRESS] = { "Press_Any_Button.png" };

	// Credit
	textureName[CREDIT] = { "staffroll_1.png" };
	textureName[CREDIT2] = { "staffroll_2.png" };

	// Game
	textureName[UI_INFO_10] = { "Info_10.png" };
	textureName[UI_INFO_30] = { "Info_30.png" };
	textureName[UI_INFO_50] = { "Info_50.png" };
	textureName[UI_INFO_ASSULT] = { "Info_Assult.png" };
	textureName[UI_INFO_LIMIT1] = { "Info_Limit1.png" };
	textureName[UI_INFO_LIMIT2] = { "Info_Limit2.png" };
	textureName[UI_INFO_MISSION] = { "Info_Mission.png" };
	textureName[UI_INFO_BOSS] = { "Info_Boss.png" };
	textureName[UI_INFO_BOSS2] = { "Info_Boss_2.png" };
	textureName[UI_INFO_BOSS3] = { "Info_Boss_3.png" };
	textureName[UI_INFO_BAR] = { "Info_Bar.png" };
	textureName[UI_MAIN_RETICLE] = { "Reticle_Target.png" };
	textureName[UI_SUB_RETICLE] = { "Reticle.png" };
	textureName[UI_RELOAD] = { "Reload.png" };
	textureName[UI_MAIN_DIGITAL_RETICLE] = { "Digi_Reticle_Target.png" };
	textureName[UI_SUB_DIGITAL_RETICLE] = { "Digi_Reticle.png" };
	textureName[UI_TIMERFLAME] = { "Time_Frame.png" };
	textureName[UI_BUTTON_SHOT] = { "shot.png" };
	textureName[UI_BUTTON_JUMP] = { "jump.png" };
	textureName[UI_BUTTON_VISION] = { "vision.png" };
	textureName[UI_BUTTON_SV] = { "Sky_Vision.png" };
	textureName[UI_BUTTON_SHIFT] = { "shift.png" };
	textureName[UI_BUTTON_BOX] = { "CommandBox.png" };
	textureName[UI_STATUSFRAME_P1] = { "Status_Frame_1.png" };
	textureName[UI_STATUSFRAME_P2] = { "Status_Frame_2.png" };
	textureName[UI_HP_GUAGE_P1] = { "HPGuage1.png" };
	textureName[UI_HP_GUAGE_P2] = { "HPGuage2.png" };
	textureName[UI_HP_GUAGE_EMP_P1]={ "HPGuage_Emp1.png" };
	textureName[UI_HP_GUAGE_EMP_P2] = { "HPGuage_Emp2.png" };
	textureName[UI_ICON_P1] = { "Chara_1.png" };
	textureName[UI_ICON_P2] = { "Chara_2.png" };
	textureName[UI_EN_GUAGE_EMP_P1] = { "EnGuage_Empty_1.png" };
	textureName[UI_EN_GUAGE_EMP_P2] = { "EnGuage_Empty_2.png" };
	textureName[UI_EN_GUAGE_P1] = { "EnGuage_NoMark_1.png" };
	textureName[UI_EN_GUAGE_P2] = { "EnGuage_NoMark_2.png" };
	textureName[UI_EN_TANK] = { "Greening.png"};
	textureName[UI_TREE_MARKER] = { "Dot.png" };
	textureName[UI_TREE_SIGN] = { "Digit_Tree.png" };
	textureName[UI_TIMER_NUMBER] = { "num_white.png" };
	textureName[UI_TIME_UP] = { "TimeUP!.png" };
	textureName[UI_COUNT_NUM] = { "num_count.png" };
	textureName[UI_GAME_START] = { "Mission_Start.png" };
	textureName[UI_START_COUNT] = { "Num_Start.png" };
	textureName[UI_CAHSE_MARK] = { "ChaseMark.png" };
	textureName[OP_ANNOUNCEMENT] = { "Mission_Term.png" };
	textureName[UI_EN_MARK] = { "Shiftable_Mark.png" };
	textureName[UI_ENERGY_EMPTY_RETICLE] = { "Energy_Empty.png" };
	textureName[MARK_BOSS] = { "icon_Boss.png" };
	textureName[MARK_ATTAKED_TREE] = { "icon_Raid.png" };
	textureName[MARK_2D_MALE] = { "Icon_M.png" };
	textureName[MARK_2D_FEMALE] = { "Icon_F.png" };
	textureName[MARK_3D_ALL] = { "Icon_All.png" };
	textureName[ALLOW_MALE] = { "Arrow_M.png" };
	textureName[ALLOW_FEMALE] = { "Arrow_M.png" };
	textureName[UI_DAMAGE] = { "Damage.png" };

	// ResultUIの文字や記号
	textureName[RESULT] = { "Result.png" };
	textureName[RESULT_PLAYER] = { "Player_No.png" };
	textureName[RESULT_GREENIG] = { "Greening_Rate.png" };
	textureName[RESULT_RYOKUKA] = { "Ryokuka_Ritsu.png" };
	textureName[RESULT_PERSENT] = { "Percent.png" };
	textureName[RESULT_DEFEAT] = { "Defeated_Counts.png" };
	textureName[RESULT_GEKITAI] = { "Gekitai_Su.png" };
	textureName[RESULT_GREEN_NUM] = { "Greening_Num.png" };
	textureName[RESULT_RYOKUKA_KAZU] = { "Ryokuka_Honsuu.png" };

	//ResultUIのテクスチャ
	textureName[RESULT_LINE] = { "Line.png" };
	textureName[RESULT_FLAME] = { "Flame_1.png" };
	textureName[RESULT_FLAME02] = { "Flame_2.png" };
	textureName[RESULT_CHARA_A] = { "Chara_A.png" };
	textureName[RESULT_CHARA_B] = { "Chara_B.png" };
	textureName[RESULT_NEXT] = { "Next.png" };
	textureName[RESULT_TO_TITLE] = { "Back_to_Title.png" };

	//ResultUIのランク
	textureName[RESULT_EXCELLENT] = { "Excellent.png" };
	textureName[RESULT_GREAT] = { "Great.png" };
	textureName[RESULT_CLEARE] = { "Cleared.png" };
	textureName[RESULT_FAILED] = { "Failed.png" };

	//ResultBG
	textureName[RESULT_BG] = { "Result_BG.png" };

	// etc
	textureName[NUMBER]			= { "num.png" };
	textureName[TOON_OUT_LINE]	= { "Outline.bmp" };
	textureName[TOON_SHADE]		= { "Shade.bmp" };
	textureName[LIGHT_001]		= { "Light_001.png" };
	textureName[TARGET]			= { "stencil.png" };
	textureName[SAMPLE_TREE]	= { "tree.png" };
	textureName[SAMPLE_GRASS]	= { "middleGrass.png" };
	textureName[NEKO]			= { "Chara_B.png" };
	textureName[COLOR_SCALE]	= { "colorScale.png" };
	textureName[DATE_ALPHA]		= { "DateAlpha.png" };
	textureName[OCEAN]			= { "sea.png" };
	//textureName[OCEAN_BUMP]		= { "NormalMap.bmp" };
	textureName[OCEAN_BUMP]		= { "ocean.png" };

	//Finale
	textureName[FIN] = { "Fin.png" };
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
	setTextureDirectory();
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
