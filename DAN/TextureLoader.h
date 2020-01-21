//===================================================================================================================================
//【TextureLoader.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/11/02
//===================================================================================================================================
//===================================================================================================================================
//課題
//シーン別に読込リソースを分割し読み込む
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace textureNS
{
	enum TEXTURE_NUMBER
	{
		// Splash
		SPLASH,

		// Tutorial
		TUTORIAL1,
		TUTORIAL2,
		TUTORIAL3,
		TUTORIAL_UI_0,
		TUTORIAL_UI_1,
		TUTORIAL_UI_PLANE,
		TUTORIAL_STANDBY_1,
		TUTORIAL_STANDBY_2,

		// Title
		TITLE_LOGO,
		TITLE_MENU,
		TITLE_INFO_TUTO,
		TITLE_INFO_GAME,
		TITLE_INFO_CREDIT,
		TITLE_INFO_EXIT,
		TITLE_COPYRIGHT,
		TITLE_BAR,
		TITLE_PRESS,

		// Credit
		CREDIT,
		CREDIT2,

		// Game
		UI_INFO_10,
		UI_INFO_30,
		UI_INFO_50,
		UI_INFO_ASSULT,
		UI_INFO_ASSULT2,
		UI_INFO_LIMIT1,
		UI_INFO_LIMIT2,
		UI_INFO_MISSION,
		UI_INFO_BOSS,
		UI_INFO_BOSS2,
		UI_INFO_WITHER,
		UI_INFO_WITHER2,
		UI_INFO_POWERUP,
		UI_INFO_BAR,
		UI_MAIN_RETICLE,
		UI_SUB_RETICLE,
		UI_RELOAD,
		UI_MAIN_DIGITAL_RETICLE,
		UI_SUB_DIGITAL_RETICLE,
		UI_TIMERFLAME,
		UI_BUTTON_SHOT,
		UI_BUTTON_JUMP,
		UI_BUTTON_VISION,
		UI_BUTTON_SV,
		UI_BUTTON_SHIFT,
		UI_BUTTON_BOX,
		UI_STATUSFRAME_P1,
		UI_STATUSFRAME_P2,
		UI_HP_GUAGE_P1,
		UI_HP_GUAGE_P2,
		UI_HP_GUAGE_EMP_P1,
		UI_HP_GUAGE_EMP_P2,
		UI_HP_GUAGE_ENEMY,
		UI_ICON_P1,
		UI_ICON_P2,
		UI_EN_GUAGE_EMP_P1,
		UI_EN_GUAGE_EMP_P2,
		UI_EN_GUAGE_P1,
		UI_EN_GUAGE_P2,
		UI_EN_TANK,
		UI_TREE_MARKER,
		UI_TREE_SIGN,
		UI_TIMER_NUMBER,
		UI_CAHSE_MARK,
		UI_TIME_UP,
		UI_COUNT_NUM,
		UI_GAME_START,
		UI_START_COUNT,
		OP_ANNOUNCEMENT,
		UI_EN_MARK,
		UI_ENERGY_EMPTY_RETICLE,
		MARK_BOSS,
		MARK_ATTAKED_TREE,
		MARK_2D_MALE,
		MARK_2D_FEMALE,
		MARK_3D_ALL,
		UI_DAMAGE,

		// Result
		RESULT,
		RESULT_LINE,
		RESULT_FLAME,
		RESULT_CHARA_A,
		RESULT_CHARA_B,
		RESULT_PLAYER,
		RESULT_GREENIG,
		RESULT_RYOKUKA,
		RESULT_PERSENT,
		RESULT_NEXT,
		RESULT_BG,
		RESULT_GREEN_NUM,
		RESULT_RYOKUKA_KAZU,
		RESULT_FLAME02,
		RESULT_TO_TITLE,
		//RESULT_NUMBER,
		RESULT_DEFEAT,
		RESULT_GEKITAI,
		RESULT_EXCELLENT,
		RESULT_GREAT,
		RESULT_CLEARE,
		RESULT_FAILED,

		// etc
		NUMBER,
		TARGET,
		DATE_ALPHA,
		OCEAN,
		OCEAN_BUMP,

		//Finale
		FIN,

		MAX_TEXTURE,
	};

	//テクスチャ外部参照関数
	LPDIRECT3DTEXTURE9* reference(int textureNo);

}

//===================================================================================================================================
//【テクスチャ読込クラス】
//===================================================================================================================================
class TextureLoader : public Base
{
private:
	const char* textureName[textureNS::MAX_TEXTURE];
	LPDIRECT3DTEXTURE9 texture[textureNS::MAX_TEXTURE];
public:
	TextureLoader();
	~TextureLoader();

	void load(LPDIRECT3DDEVICE9 device);
	void release();

	LPDIRECT3DTEXTURE9* getTexture(int textureNo) { return &texture[textureNo]; }
};

