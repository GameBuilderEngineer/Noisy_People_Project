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
#define SE_PATH_LIST_TAIL(num)			SEPathList##[num]

//===================================================================================================================================
//【列挙型定数】
//===================================================================================================================================
enum SE_LIST
{
	SE_Cursor,
	SE_Decision,
	SE_Cancel,
	SE_AddTree,
	SE_Score,
	SE_Shot,
	SE_Reload,
	SE_Getlem,
	SE_RecoverPower,
	SE_BanOperation,
	SE_HitBulletTree,
	SE_Convert,
	SE_Greening,
	SE_EnemyActive,
	SE_EnemyAttack,
	SE_EnemyAttackHit,
	SE_EnemyDefeated,
	SE_StartGame,
	SE_AnnounceTelop,
	SE_Telop,
	SE_TimeUp,
	SE_BIRD_0,
	SE_BIRD_1,
	SE_WIND,
	SE_Result,
	SE_HurryUp,
	SE_CountDown,
	SE_BossSpawning,
	SE_Score2,
	SE_TUTORIAL_CLEAR,
	SE_TUTORIAL_END,
	SE_TUTORIAL_STEP_CLEAR,
	SE_GetPowerUpItem,
	SE_Green_Down,
	SE_SetPowerUpItem,
	SE_MAX
};

//===================================================================================================================================
//【定数定義】
//===================================================================================================================================
static const char *SEPathList[SE_LIST::SE_MAX] = {
	"SE_Cursor.wav","SE_Decision.wav" ,"SE_Cancel.wav",
	"SE_AddTree.wav","SE_Score.wav",
	"SE_Shot.wav","SE_Reload.wav" ,"SE_Getlem.wav","SE_RecoverPower.wav",
	"SE_BanOperation.wav",
	"SE_HitBulletTree.wav",
	"SE_Convert.wav","SE_Greening.wav","SE_EnemyActive.wav",
	"SE_EnemyAttack.wav","SE_EnemyAttackHit.wav","SE_EnemyDefeated.wav",
	"SE_StartGame.wav","SE_AnnounceTelop.wav","SE_Telop.wav","SE_TimeUp.wav",
	"SE_Bird0.wav","SE_Bird1.wav","SE_Wind.wav","SE_Result.wav","SE_HurryUp.wav",
	"SE_CountDown.wav", "SE_BossSpawning.wav","SE_Score2.wav",
	"Tutorial_Clear.wav","Tutorial_End.wav","Tutorial_Step_Clear.wav",
	"SE_GetPowerUpItem.wav","SE_Green_Down.wav","SE_SetPowerUpItem.wav"
};

//===================================================================================================================================
//【サウンド(XAudio2)】
//サウンドのSEクラス
//===================================================================================================================================
class SEManager : public SoundBase
{
public:
	SEManager() { loadBuffer(); };
	~SEManager() {};


#if _DEBUG
	void	 outputGUI(void)override;				//ImGUIへの出力
#endif

private:
	//関数
	void loadBuffer()override;	//必要なサウンドバッファを用意する

};
