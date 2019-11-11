//===================================================================================================================================
//【SE.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/04
// [更新日]2019/10/04
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "LinkedList.h"
#include "SoundBase.h"

//===================================================================================================================================
//【マクロ定義】
//===================================================================================================================================
#define SE_PATH_LIST_TAIL(scene,num)			scene##SEPathList##num

//===================================================================================================================================
//【列挙型定数】
//===================================================================================================================================
enum SPLASH_SE_LIST
{
	SPLASH_SE_MAX
};

enum TITLE_SE_LIST
{
	SE_TITLE_Cursor,
	SE_TITLE_Decision,
	SE_TITLE_Cancel,
	TITLE_SE_MAX
};

enum CREDIT_SE_LIST
{
	SE_CREDIT_Cursor,
	SE_CREDIT_Decision,
	SE_CREDIT_Cancel,
	CREDIT_SE_MAX
};

enum GAME_SE_LIST
{
	SE_GAME_Cursor,
	SE_GAME_Decision,
	SE_GAME_Cancel,
	SE_GAME_Shot,
	SE_GAME_Reload,
	SE_GAME_Getlem,
	SE_GAME_RecoverPower,
	SE_GAME_BanOperation,
	SE_GAME_VisionStart,
	SE_GAME_VisionFinish,
	SE_GAME_Vision,
	SE_GAME_SkyVisionStart,
	SE_GAME_SkyVisionFinish,
	SE_GAME_ShiftStart,
	SE_GAME_ShiftFinish,
	SE_GAME_HitBulletTree,
	SE_GAME_Convert,
	SE_GAME_Greening,
	SE_GAME_EnemyActive,
	SE_GAME_EnemyAttack,
	SE_GAME_EnemyAttackHit,
	SE_GAME_EnemyDefeated,
	SE_GAME_StartGame,
	SE_GAME_AnnounceTelop,
	SE_GAME_Telop,
	SE_GAME_TimeUp,
	SE_GAME_BIRD_0,
	SE_GAME_BIRD_1,
	SE_GAME_WIND,
	GAME_SE_MAX
};

enum TUTORIAL_SE_LIST
{
	SE_TUTORIAL_Cursor,
	SE_TUTORIAL_Decision,
	SE_TUTORIAL_Cancel,
	TUTORIAL_SE_MAX
};

enum CREATE_SE_LIST
{
	CREATE_SE_MAX
};

enum RESULT_SE_LIST
{
	SE_RESULT_Cursor,
	SE_RESULT_Decision,
	SE_RESULT_Cancel,
	SE_RESULT_AddTree,
	SE_RESULT_Score,
	RESULT_SE_MAX
};

//===================================================================================================================================
//【定数定義】
//===================================================================================================================================
static const char *titleSEPathList[TITLE_SE_LIST::TITLE_SE_MAX] = {
	"SE_Cursor.wav","SE_Decision.wav" ,"SE_Cancel.wav" };

static const char *creditSEPathList[CREDIT_SE_LIST::CREDIT_SE_MAX] = {
	"SE_Cursor.wav","SE_Decision.wav" ,"SE_Cancel.wav" };

static const char *gameSEPathList[GAME_SE_LIST::GAME_SE_MAX] = {
	"SE_Cursor.wav","SE_Decision.wav" ,"SE_Cancel.wav",
	"SE_Shot.wav","SE_Reload.wav" ,"SE_Getlem.wav","SE_RecoverPower.wav",
	"SE_BanOperation.wav","SE_VisionStart.wav" ,"SE_VisionFinish.wav",
	"SE_Vision.wav","SE_SkyVisionStart.wav","SE_VisionFinish.wav",
	"SE_ShiftStart.wav", "SE_ShiftFinish.wav","SE_HitBulletTree.wav",
	"SE_Convert.wav","SE_Greening.wav","SE_EnemyActive.wav",
	"SE_EnemyAttack.wav","SE_EnemyAttackHit.wav","SE_EnemyDefeated.wav",
	"SE_StartGame.wav","SE_AnnounceTelop.wav","SE_Telop.wav","SE_TimeUp.wav",
	"SE_Bird0.wav","SE_Bird1.wav","SE_Wind.wav" };

static const char *tutorialSEPathList[TUTORIAL_SE_LIST::TUTORIAL_SE_MAX] = {
	"SE_Cursor.wav","SE_Decision.wav" ,"SE_Cancel.wav" };

static const char *resultSEPathList[RESULT_SE_LIST::RESULT_SE_MAX] = {
	"SE_Cursor.wav","SE_Decision.wav" ,"SE_Cancel.wav",
	"SE_AddTree.wav","SE_Score.wav" };


//===================================================================================================================================
//【サウンド(XAudio2)】
//サウンドのSEクラス
//===================================================================================================================================
class SEManager : public SoundBase
{
public:
	SEManager() {};
	~SEManager() {};

	//関数
	void	 SwitchAudioBuffer(int scene)override;	//ステージ遷移に合わせて必要なサウンドバッファを用意する

		//debug用
#if _DEBUG
	void	 outputGUI(void)override;				//ImGUIへの出力
#endif
};
